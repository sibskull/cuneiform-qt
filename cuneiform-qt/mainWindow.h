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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#include "ui_mainWindow.h"
#include "settings.h"
#include "backend.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT
	
public:

	MainWindow( QStringList *names );
	~MainWindow();

public slots:

	// Window related
	void onWindowClose();
	void OpenImage();
	void RecognizeText();
	void SaveText();
	void About();
	
	// Process related
	void onProcessStart();
	void onProcessFinish();
	void onProcessStop();
	void onProcessHasOutput();
	void onProcessHasErrors();
	
	// Settings
	void onSettingsSave();
	void onSettingsCancel();


private:
	
	// Methods
	void renderImage();
	
	// Fields
	Ui_MainWindow *form;
	Backend *process;
	
	Settings *config;
	QStringList *files;
	
	QString sourceImageFile;
	QString resultFile;
	
	// Progress dialog
	QMessageBox *progressDialog;
	
	// Scene for display image
	QGraphicsScene scene;

};


#endif // MAINWINDOW_H
