/***************************************************************************
 *                           Shatrabase                                    *
 *   (C) 2012-2014 AFKM <francis@afkm.com>                                 *
 *                   derived from ChessX with thanks                       *
 *   (C) 2006-2007 Marius Roets <roets.marius@gmail.com>                   *
 *   (C) 2007-2009 Michal Rudolf <mrudolf@kdewebdev.org>                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef __OUTPUTOPTIONS_H__
#define __OUTPUTOPTIONS_H__

#include <QString>
#include <QMap>
#include <QStringList>
#include <QObject>

/** @ingroup Core
The OutputOptions class manages options for @ref Output class.
*/
class OutputOptions : public QObject
{
public :
	enum OutputOptionType { Boolean, String, Integer, Color };
	OutputOptions();

	bool createOption(const QString& optionName, OutputOptionType optionType, const QString& allowValues,
			  const QString& defaultValue, const QString& description);
	bool createOption(const QString& optionName, const QString& optionType, const QString& allowValues,
			  const QString& defaultValue, const QString& description);
	bool createDefaultOptions();

	/* Setting values */
	bool setOption(const QString& optionString);
	bool setOption(const QString& optionName, const QString& optionValue);
	bool setOption(const QString& optionName, bool optionValue);
	bool setOption(const QString& optionName, int optionValue);

	/* Retrieving values */
	int getOptionAsInt(const QString& optionName);
	QString getOptionAsString(const QString& optionName);
	bool getOptionAsBool(const QString& optionName);
	QString getOptionDescription(const QString& optionName);
	QStringList getOptionList();
	QMap<QString, QString> getOptionListAndDescription();

private :

	QMap<QString, QString> m_list;
	QMap<QString, OutputOptionType> m_type;
	QMap<QString, QString> m_default;
	QMap<QString, QString> m_allow;
	QMap<QString, QString> m_description;

	bool validateValue(const QString& optionName, const QString& value);
	OutputOptionType optionString2Type(const QString& optionTypeStr);


};

#endif	// __OUTPUTOPTIONS_H__

