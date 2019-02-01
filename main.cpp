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

#include <QtGui>
#include <iostream>
#include "mainWindow.h"
#include <QMessageBox>

int main( int argc, char *argv[] ) {

	Q_INIT_RESOURCE( mainWindow );
	
	// Application instance
	QApplication app( argc, argv );

	// Set application details
	app.setApplicationName( "cuneiform-qt" );
	app.setOrganizationDomain( "http://www.altlinux.org/Cuneiform-Qt" );

	// Load localization
	QTranslator translator;
	QString locale = QLocale::system().name().left( 2 );
	translator.load( QString( DATADIR ) + app.applicationName() + QString ( "_" ) + locale );
    app.installTranslator( &translator );

        QTranslator qtTranslator;
        qtTranslator.load("qt_" + QLocale::system().name(), QLibraryInfo::location(QLibraryInfo::TranslationsPath));
        app.installTranslator(&qtTranslator);
	 
	// Process command line arguments
	
	// Show help
	if( app.arguments().contains( "--help" ) || app.arguments().contains( "-h" ) ) {
		std::cout << qPrintable( QObject::tr( "%1 %2. Graphical interface for Cuneiform OCR system.", 
			"%1 is application name, %2 is application version").arg( app.applicationName() ).arg( QString( VERSION ) ) ) << std::endl;
		std::cout << qPrintable( QObject::tr( "Usage: %1 [option] [image]", 
			"%1 is application name" ).arg( app.applicationName() ) )<< std::endl << std::endl;
		std::cout << qPrintable( QObject::tr( "Available options:" ) ) << std::endl;
		std::cout << qPrintable( QObject::tr( "    -h         This help" ) ) << std::endl;
		std::cout << qPrintable( QObject::tr( "    --version  Version information" ) ) << std::endl;
		std::cout << qPrintable( QObject::tr( "    image      Image file in format supported by ImageMagic" ) ) << std::endl;
		exit( 0 );
	}

	// Print version information
	// TODO Print Cuneiform version
	if( app.arguments().contains( "--version" ) ) {
		std::cout << qPrintable( QObject::tr( "%1 %2", 
			"%1 is application name, %2 is application version").arg( app.applicationName() ).arg( QString( VERSION ) ) ) << std::endl;
		exit( 0 );
	}
	
	// Fill package list
	QStringList *names = new QStringList();
	for (int i = 1; i < app.arguments().size(); ++i) {
		  if( app.arguments().at(i)[0] != '-' ) {
				names->append( app.arguments().at(i) );
		  }
	}

	// Main window
	MainWindow *win = new MainWindow( names );
	win->show();

	return app.exec();
}
