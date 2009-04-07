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

#include "backend.h"

Backend::Backend( Settings *cfg ) 
	: QProcess()
{
	config = cfg;
	output.clear();
	errors.clear();
	error.clear();
}

// On process start
int Backend::onStart() {

	if( state() == QProcess::Running ) {
		return true;
	}
	
	// Convert additional arguments to list and pass to process
	QStringList args = config->getProgram().split( " ", QString::SkipEmptyParts );
	QString command = args.at( 0 );
	args.removeAt( 0 );

	// Add option values
	args << "-l" << config->getLanguage();
	args << "-f" << config->getFormat();
	args << "-o" << config->getOutputFile();
	args << config->getInputFile();
	
	//qDebug() << qPrintable( command ) << qPrintable( args.join( " " ) );

	// Clear errors
	errors.clear();
	error.clear();

	// Start process
	start( command, args );
	
	return waitForStarted();
}

// On process finish
void Backend::onFinish() {
	//qDebug() << "FINISH!";
}

// On data in stdout
void Backend::onReadOutput() {

	setReadChannel( StandardOutput );
	
	QByteArray bytes = readAll();
	output += QString( bytes );
	//qDebug() << qPrintable( QString( bytes ) );

}

// On data in stderr
void Backend::onReadErrors() {

	setReadChannel( StandardError );
	
	QByteArray bytes = readAll();
	errors += QString( bytes );

}

QString Backend::getError() {
	return errors;
}
