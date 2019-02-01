/***************************************************************************
 *   Copyright (с) 2009 by Andrey Cherepanov <cas@altlinux.org>            *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "mainWindow.h"
#include <QFileDialog>
#include <QGraphicsPixmapItem>

MainWindow::MainWindow( QStringList *names )
	: QMainWindow( 0, 0 )
{
	// Set files list
	files = names;
	
	// Set initial values
	if( names->count() > 0 )
		sourceImageFile = names->at( 0 ); // Use first passed file name
	else
		sourceImageFile.clear();
	
	// Create configuration dialog
	config = new Settings( this );

	// Prepare temporary file for result
	QTemporaryFile tempFile;
	tempFile.setFileTemplate( QDir::tempPath() + QDir::separator() + QString( "cuneiform-result-XXXXXX.html" ) );
	tempFile.open();
	resultFile = tempFile.fileName();
	tempFile.close();
	config->setOutputFile( resultFile );

	// Set main window content
	form = new Ui_MainWindow();
	form->setupUi( this );

	// Set scene
	form->source->setScene( &scene );

	// Set up process
	process = new Backend( config );
	connect( process, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(onProcessFinish()) );
	connect( process, SIGNAL(readyReadStandardOutput()), this, SLOT(onProcessHasOutput()) );
	connect( process, SIGNAL(readyReadStandardError()), this, SLOT(onProcessHasErrors()) );
	
	// Set actions
	// TODO Disable action if image is not loaded
	
	// Open image
	connect( form->actionOpen, SIGNAL(triggered()), this, SLOT(OpenImage()) );
	// Recognize text
	connect( form->actionRecognizeText, SIGNAL(triggered()), this, SLOT(RecognizeText()) );
	// Save result text
	connect( form->actionSave, SIGNAL(triggered()), this, SLOT(SaveText()) );
	// Exit. TODO confirm exit without saving recognized text
	connect( form->actionExit, SIGNAL(triggered()), this, SLOT(onWindowClose()) );

	// Open configuration dialog
	connect( form->actionConfigure, SIGNAL(triggered()), config, SLOT(exec()) );
    connect( config, SIGNAL(accepted()), this, SLOT(onSettingsSave()) );
    connect( config, SIGNAL(rejected()), this, SLOT(onSettingsCancel()) );

	// About dialog
	connect( form->actionAbout, SIGNAL(triggered()), this, SLOT(About()) );
	
	// On window close
	connect( this, SIGNAL(destroyed()), this, SLOT(onWindowClose()) );
	
	// Process progress dialog
	progressDialog = new QMessageBox( this );
	progressDialog->setModal( true );
	progressDialog->setWindowTitle( tr("Process") );
	progressDialog->setText( tr("Recognition is in progress.\nPlease, wait...") );
	progressDialog->addButton( QMessageBox::Cancel );
	connect( progressDialog, SIGNAL(rejected()), this, SLOT(onProcessStop()) );

	// Set render text action disabled
	form->actionRecognizeText->setDisabled( true );

	// Render specified image file
	if( ! sourceImageFile.isEmpty() ) {
		renderImage();
	}
	
	
}

// Main window desctructor
MainWindow::~MainWindow()
{
	emit onWindowClose();
}

// On main window close
void MainWindow::onWindowClose() {
	
	//qDebug() << "onWindowClose():";
	// Stop recognition process
	if ( process->state() == QProcess::Running) {
        process->terminate();
        process->waitForFinished(3000);
    }	

	close();
	
}

// Select image file
void MainWindow::OpenImage() {

	// Open standart open dialog
	QFileDialog dialog( this );

	dialog.setNameFilter( tr("Images (*.png *.jpg *.jpeg *.bmp *.tif *.tiff)") + QString( ";;" ) + tr("All files (*)") );
	dialog.setWindowTitle( tr("Open image") );
	dialog.setAcceptMode( QFileDialog::AcceptOpen );
	
	QStringList fileNames;
	if (dialog.exec())
		fileNames = dialog.selectedFiles();
	
	if( fileNames.count() > 0 ) {
		sourceImageFile = fileNames.at( 0 );
		//QMessageBox::information( this, tr("Opened file"), sourceImageFile, QMessageBox::Ok );
	}
	
	// Render image
	if( ! sourceImageFile.isEmpty() ) {
		renderImage();
	}
	
}

// Recognize text from current opened image
void MainWindow::RecognizeText() {
	// Run recognition process
	emit onProcessStart();
}

// Save recognized text
void MainWindow::SaveText() {
	
	int save = true;
	int ret;

	form->statusBar->showMessage( tr("Saving result file...") );

	// Open standart save dialog
	QString savedFile( "" );
	QFileDialog dialog( this );
	
	// Different extensions for different formats
	QString defExt;
	QString defFilter;
	
	if( config->getFormat() == QString( "html" ) || config->getFormat() == QString( "hocr" ) ) {
		defFilter = tr("HTML documents (*.htm *.html)");
		defExt = QString( "html" );
	} else {
		if( config->getFormat() == QString( "rtf" ) ) {
			defFilter = tr("RTF documents (*.rtf)");
			defExt = QString( "rtf" );
		} else {
			defFilter = tr("Text files (*.txt)");
			defExt = QString( "txt" );
		}
	}

	dialog.setNameFilter( defFilter + QString( ";;" ) + tr("All files (*)") );
	dialog.setDefaultSuffix( defExt );
	dialog.setWindowTitle( tr("Save result") );
	dialog.setAcceptMode( QFileDialog::AcceptSave );
	dialog.setConfirmOverwrite( false );
	
	QStringList fileNames;
	if ( dialog.exec() )
		fileNames = dialog.selectedFiles();
	
	if( fileNames.count() > 0 ) {
		savedFile = fileNames.at( 0 );
	}
	
	//qDebug() << qPrintable( tr("Saved file: %1 (%2)").arg( savedFile ).arg( savedFile.isEmpty() ) );
	
	if( savedFile.isEmpty() ) {
		form->statusBar->showMessage( QString( "" ) );
		return;
	}
	
	// Check for existance anf confirm overwrite if needed
	QFile f( savedFile );
	if( f.exists() ) {
		ret = QMessageBox::question( this, tr("Overwrite file?"),
							  tr("File with name '%1' is exist.\n"
							  "Do you really overwrite this file?").arg( f.fileName() ), 
							  QMessageBox::Yes | QMessageBox::No );
		if( ret != QMessageBox::Yes ) 
			save = false;
		else
			f.remove();
	}
	
	// Really save to specified file
	if( ret ) {
		f.open( QFile::WriteOnly | QFile::Truncate );
		
		// Check for open error
		if( f.error() ) {
			QMessageBox::critical( this, tr("Error open"),
							  tr("Unable to open file '%1' for save.").arg( f.fileName() ), 
							  QMessageBox::Ok );
			form->statusBar->showMessage( tr("Unable to open file '%1' for save.").arg( f.fileName() ) );
			return;
		}
		
		// Get content from the result field
		QString content;
		if( config->getFormat() == QString( "html" ) || config->getFormat() == QString( "hocr" ) ) {
			content = form->result->toHtml();
		} else {
			content = form->result->toPlainText();
		}
		
		// Write to file
		QTextStream stream( &f );
		
		// Set UTF-8 codec for write stream
		stream.setCodec( QTextCodec::codecForName( "UTF-8" ) );
		
		stream << content;
	
		// Check for write error
		if( f.error() ) {
			QMessageBox::critical( this, tr("Error save"),
							  tr("Unable to save file '%1'.").arg( f.fileName() ), 
							  QMessageBox::Ok );
			form->statusBar->showMessage( tr("Unable to save file '%1'.").arg( f.fileName() ) );
			return;
		}
		f.close();
	}

	form->statusBar->showMessage( tr("File '%1' is saved successful.").arg( f.fileName() ) );

}

// Render specified image
void MainWindow::renderImage() {
	QPixmap img;
	QGraphicsPixmapItem *page;
	
	//qDebug() << "Load image" << qPrintable( sourceImageFile );
	scene.clear();
	
	// Load image
	form->statusBar->showMessage( tr("Loading image '%1'...").arg( sourceImageFile) );
	if( ! img.load( sourceImageFile ) ) {
		form->statusBar->showMessage( tr("Unable to open file '%1'.").arg( sourceImageFile) );
		QMessageBox::warning( this, tr("Error open file"),
							  tr("Unable to open file '%1'.").arg( sourceImageFile ),
							  QMessageBox::Ok );
		return;
	}
	
	page = scene.addPixmap( img );
	//page = scene.addPixmap( QPixmap("icons/tool-open.png") );
	page->setOffset( 5, 5 );

	// Show image
	form->source->show();

	form->statusBar->showMessage( tr("Image '%1' is loaded.").arg( sourceImageFile ) );

	// Set render text action enabled
	form->actionRecognizeText->setDisabled( false );

	config->setInputFile( sourceImageFile );

}

// On process start
void MainWindow::onProcessStart() {

	if( sourceImageFile.isEmpty() ) {
		QMessageBox::warning( this, tr("Image is not loaded"),
							  tr("Image is not loaded. Please, load scanned image."),
							  QMessageBox::Ok );
		return;
	}

	if( ! process->onStart() ) {
		QMessageBox::critical( this, tr("Error start"), 
			tr("Could not start Cuneiform executables. Check your installation.") );
		return;
	}

	form->statusBar->showMessage( tr("Recognition is processing...") );
	form->result->clear();
	progressDialog->exec();
	
}

// On process finish
void MainWindow::onProcessFinish() {

	QString err;

	process->onFinish();

	// Process result
	err = process->getError();
	if( ! err.isEmpty() ) {
		// Show error message
		progressDialog->hide();
		QString errText = tr("Cuneiform error:\n%1").arg( err ); // TODO parse and localize error description
		form->statusBar->showMessage( errText );
		QMessageBox::warning( this, tr("Error recognition"),
							  errText, 
							  QMessageBox::Ok );
		return;

	} else {
		// Show result
		// TODO Discern formats
		form->result->clear();
		QFile f( resultFile );
		f.open( QIODevice::ReadOnly );
		QTextStream stream( &f );
		
		// Set UTF-8 codec for read stream
		stream.setCodec( QTextCodec::codecForName( "UTF-8" ) );
		
		QString content = stream.readAll();
		f.close();
		f.remove();
		if( config->getFormat() == QString( "html" ) || config->getFormat() == QString( "hocr" ) ) {
			form->result->setHtml( content );
		} else {
			form->result->setPlainText( content );
		}
	}
	progressDialog->hide();
	form->statusBar->showMessage( tr("Recognition is complete.") );
	
}

// Stop process
void MainWindow::onProcessStop() {
	process->kill();
	progressDialog->hide();
	form->statusBar->showMessage( tr("Recognition is canceled by user.") );
}

// On data in stdout
void MainWindow::onProcessHasOutput() {
	process->onReadOutput();
}

// On data in stderr
void MainWindow::onProcessHasErrors() {
	process->onReadErrors();
}

// Save settings
void MainWindow::onSettingsSave() {
	config->save();
}

// Save settings
void MainWindow::onSettingsCancel() {
	config->load();
}

// Show dialog about application
// TODO set big application icon
// TODO show cuneiform version
void MainWindow::About() {

	QMessageBox aboutDialog( this );

	aboutDialog.setIconPixmap( QPixmap(QString::fromUtf8(":/small/icons/cuneiform-qt.png")) );
	aboutDialog.setWindowTitle( tr("About Cuneiform-Qt") );
	aboutDialog.setText( 							  
						 tr("<b>Cuneiform-Qt:</b><br />"
						 "   Graphical interface for Cuneiform OCR system.<br />"
						 "   Version: %1<br />"
						 "   Copyright &copy; Andrey Cherepanov <cas@altlinux.org>, 2009<br />"
						 "   Web-site: <a href='%2'>%2</a><br />"
						 "<br />"
						 "<b>Cuneiform:</b><br />"
						 "   Copyright &copy; Cognitive technologies<br />"
						 "   Web-site: <a href='https://launchpad.net/cuneiform-linux'>https://launchpad.net/cuneiform-linux</a><br />"
						 ).arg( 
						 QString( VERSION ) ).arg(  // Application version
						 QApplication::organizationDomain() ) // Application site
						);
	aboutDialog.setStandardButtons( QMessageBox::Ok );
	aboutDialog.setDefaultButton( QMessageBox::Ok );

	aboutDialog.exec();
}
