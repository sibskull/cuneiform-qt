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
#ifndef SETTINGS_H
#define SETTINGS_H

#include <QtGui>
#include "ui_settings.h"

class Settings : public QDialog
{
	Q_OBJECT
	
public:
	
	Settings( QWidget *parent );

	// Get setting value
	QString getProgram();
	QString getLanguage();
	QString getFormat();
	QString getInputFile();
	QString getOutputFile();
	
	// Set setting value
	void setInputFile( QString fileName );
	void setOutputFile( QString fileName );

	// Save settings
	void save();
	void load();

private:
	
	// Form
	Ui_SettingsDialog *form;

	// Parameters
	QString pLanguage;
	QString pFormat;
	QString pCommand;
	QString in;
	QString out;

};

#endif // SETTINGS_H
