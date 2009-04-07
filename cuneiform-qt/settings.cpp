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

#include <settings.h>

Settings::Settings( QWidget *parent )
	: QDialog( parent )
{	
	// Set dialog content
	form = new Ui_SettingsDialog();
	form->setupUi( this );

	// Set default values
	QString locale = QLocale::system().name().left( 2 );
	
	QHash<QString, QString> supportedLanguage;
	supportedLanguage[ "en" ] = QString( "eng" );
	supportedLanguage[ "de" ] = QString( "ger" );
	supportedLanguage[ "fr" ] = QString( "fra" );
	supportedLanguage[ "ru" ] = QString( "ruseng" );
	supportedLanguage[ "sv" ] = QString( "swe" );
	supportedLanguage[ "es" ] = QString( "spa" );
	supportedLanguage[ "it" ] = QString( "ita" );
	supportedLanguage[ "uk" ] = QString( "ukr" );
	supportedLanguage[ "sr" ] = QString( "srp" );
	supportedLanguage[ "hr" ] = QString( "hrv" );
	supportedLanguage[ "pl" ] = QString( "pol" );
	supportedLanguage[ "da" ] = QString( "dan" );
	supportedLanguage[ "pt" ] = QString( "por" );
	supportedLanguage[ "nl" ] = QString( "dut" );
	supportedLanguage[ "cs" ] = QString( "cze" );
	supportedLanguage[ "ro" ] = QString( "rum" );
	supportedLanguage[ "hu" ] = QString( "hun" );
	supportedLanguage[ "bg" ] = QString( "bul" );
	supportedLanguage[ "sl" ] = QString( "slo" );
	supportedLanguage[ "lv" ] = QString( "lav" );
	supportedLanguage[ "lt" ] = QString( "lit" );
	supportedLanguage[ "et" ] = QString( "est" );
	supportedLanguage[ "tr" ] = QString( "tur" );
	
	// Try to guess language from locale
	// qDebug() << "Language:" << qPrintable(locale) << supportedLanguage.contains( locale );

	if( supportedLanguage.contains( locale )) {
		pLanguage = supportedLanguage.value( locale );
	} else {
		pLanguage = QString( "eng" );
	}
	
	pFormat = QString( "html" );
	pCommand = QString( "cuneiform" );

	// Fill combo boxes
	QComboBox *lang = form->language;
	// Supported languages: eng ger fra rus swe spa ita ruseng ukr srp hrv pol dan por dut cze rum hun bul slo lav lit est tur
	lang->clear();
	lang->addItem( tr("English"), "eng" );
	lang->addItem( tr("German"), "ger" );
	lang->addItem( tr("French"), "fra" );
	lang->addItem( tr("Russian"), "rus" );
	lang->addItem( tr("Swedish"), "swe" );
	lang->addItem( tr("Spanish"), "spa" );
	lang->addItem( tr("Italian"), "ita" );
	lang->addItem( tr("Russian+English"), "ruseng" );
	lang->addItem( tr("Ukrainian"), "ukr" );
	lang->addItem( tr("Serbian"), "srp" );
	lang->addItem( tr("Croatian"), "hrv" );
	lang->addItem( tr("Polish"), "pol" );
	lang->addItem( tr("Danish"), "dan" );
	lang->addItem( tr("Portuguese"), "por" );
	lang->addItem( tr("Dutch"), "dut" );
	lang->addItem( tr("Czech"), "cze" );
	lang->addItem( tr("Romanian"), "rum" );
	lang->addItem( tr("Hungarian"), "hun" );
	lang->addItem( tr("Bulgarian"), "bul" );
	lang->addItem( tr("Slovenian"), "slo" );
	lang->addItem( tr("Latvian"), "lav" );
	lang->addItem( tr("Lithuanian"), "lit" );
	lang->addItem( tr("Estonian"), "est" );
	lang->addItem( tr("Turkish"), "tur" );
	
	
	QComboBox *fmt = form->format;
	/* Supported formats:
    html         HTML format
    hocr         hOCR HTML format
    native       Cuneiform 2000 format
    rtf          RTF format
    smarttext    plain text with TeX paragraphs
    text         plain text
	*/
	fmt->clear();
	fmt->addItem( tr("HTML"), "html" );
	/* TODO other format currently is not supported */
	fmt->addItem( tr("hOCR HTML"), "hocr" );

	// Remove native option because it is binary format and it will not display correctly
	//fmt->addItem( tr("Cuneiform 2000 format"), "native" );

	fmt->addItem( tr("RTF"), "rtf" );
	fmt->addItem( tr("Plain text with TeX paragraphs"), "smarttext" );
	fmt->addItem( tr("Plain text"), "text" );
	/**/

	// TODO Sort languages
	

	// Load settings
	load();
	
}

// Load settings from config file
void Settings::load() {

	int id;

	QComboBox *lang = form->language;
	QComboBox *fmt = form->format;

	// Restore saved values
	QSettings settings;
	pLanguage = settings.value( "Global/Language", QVariant( pLanguage ) ).toString();
	pFormat   = settings.value( "Global/Format",   QVariant( pFormat ) ).toString();
	pCommand  = settings.value( "Global/Command",  QVariant( pCommand ) ).toString();
	
	// Apply settings to form
	form->command->setText( pCommand );
	
	id = lang->findData( QVariant( pLanguage ) );
	lang->setCurrentIndex( id );
	
	id = fmt->findData( QVariant( pFormat ) );
	fmt->setCurrentIndex( id );

}

// Save settings to config file
void Settings::save() {

	int id;

	id = form->language->currentIndex();
	pLanguage = form->language->itemData( id ).toString();
	id = form->format->currentIndex();
	pFormat = form->format->itemData( id ).toString();
	pCommand  = form->command->text();

	QSettings settings;
	settings.setValue( "Global/Language", pLanguage );
	settings.setValue( "Global/Format",   pFormat );
	settings.setValue( "Global/Command",  pCommand );
	settings.sync();
	
}

// Get program name
QString Settings::getProgram() {
	return pCommand;
}

// Get selected language
QString Settings::getLanguage() {
	return pLanguage;
}

// get selected format
QString Settings::getFormat() {
	return pFormat;
}

// get input file name
QString Settings::getInputFile() {
	return in;
}

// Get output file name
QString Settings::getOutputFile() {
	return out;
}
	
// Set input file name
void Settings::setInputFile( QString fileName ) {
	in = fileName;
}

// Set output file name
void Settings::setOutputFile( QString fileName ) {
	out = fileName;
}
