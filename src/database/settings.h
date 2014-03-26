/***************************************************************************
 *                           Shatrabase                                    *
 *   (C) 2012-2014 AFKM <francis@afkm.com>                                 *
 *                   derived from ChessX with thanks                       *
 *   (C) 2005-2009 Michal Rudolf <mrudolf@kdewebdev.org>                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include <QSettings>
#include "engineoptiondata.h"

class QWidget;


#ifdef Q_OS_MAC
    #define MIN_WHEEL_COUNT 120
    #define DEFAULT_FONTSIZE 12
    #define DEFAULT_LISTFONTSIZE 12
#else
    #define MIN_WHEEL_COUNT 0
    #define DEFAULT_FONTSIZE 11
    #define DEFAULT_LISTFONTSIZE 11
#endif

/**
    The Settings class provides a wrapper for the Qt QSettings class. It allows
    easier save/restore of application settings and gets paths for various resources.
*/

class Settings : public QSettings
{
	Q_OBJECT
public:
	enum {Show = 1} LayoutFlags;
	Settings();
	~Settings();

    /** @return directory where data is stored. */
    QString dataPath();

    /** Returns map of short locale (e.g. 'en') to language name */
    const QMap<QString, QString>& languages() const { return m_langMap; }

    /** Returns the language name for the given locale.
        Return 'English' if unknown. */
    const QString& language(const QString& locale) const;

    /** Returns the short locale (e.g. 'en') for the language name (as in languages()).
        Returns 'en' when unknown. */
    const QString& locale(const QString& languageName) const;

    /** Returns 'en' */
    const QString& defaultLocale() const { return m_defaultLocale; }

    /* Returns current board style name. */
    //const QString& boardStyle() const { return m_boardStyle; }
    /* Returns @p index'th style name */
    //const QString& boardStyle(unsigned int index) const { return m_boardStyles[index]; }

    /** Restore widget's layout based on its name. Optionally show window if it is visible.
	@return @p true if the state was restored. */
    bool layout(QWidget* w);

    /** Write widget's layout with its name.
       If x, y, w or h != -1 they will be used instead of the widget's actual pos and size. */
    void setLayout(const QWidget* widget, int x = -1, int y = -1, int w = -1, int h = -1);

    /** Write integer list to configuration file. Does it by converting it to QString */
    void setList(const QString& key, QList<int> list);

    /** Appends values to the list. @return @p true if the list contains exact number of items.
	If @p items is @p -1 , always return @p true. */
    bool list(const QString& key, QList<int>& list, int items = -1);

    /// set a QByteArray into the settings
    void setByteArray(const QString& key, const QByteArray& arr);
    /// Read a QByteArray from the Settings
    QByteArray byteArray(const QString& key);

    /// Overloading value from QSettings with a single place where defaults come from
    QVariant getValue(const QString &key) const;

    void setMap(const QString& key, const OptionValueList& map);
    void getMap(const QString& key, OptionValueList& map);
private:

    QMap<QString, QVariant> initDefaultValues() const;

    QMap<QString, QString> m_langMap;
    QString m_defaultLocale, m_defaultLanguageName;

    QString m_dataPath,
    /** Current board style */
            m_boardStyle;
    /** All created styles */
    std::vector<QString> m_boardStyles;

};

extern Settings* AppSettings;

#endif

