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
 
#include "common.h"
#include "settings.h"
#include "boardtheme.h"

#include <QtCore>
#include <QtGui>
#include <QWidget>
#include <QMainWindow>
#include <QDebug>

Settings::Settings()
    :   QSettings(IniFormat, UserScope, "shatrabase", "shatrabase"),
        m_defaultLocale("en"),
        m_defaultLanguageName(tr("English"))
{
    //: language name
    m_langMap.insert("en", tr("English"));
    //: language name
    m_langMap.insert("de", tr("German"));
    //: language name
    m_langMap.insert("ru", tr("Russian"));

}

Settings::~Settings()
{}

bool Settings::layout(QWidget* w)
{
	if (!w || w->objectName().isEmpty())
		return false;
	beginGroup("Geometry");
	QList<int> values;
	bool valid = list(w->objectName(), values, 5);
    if (valid) // Enough values
    {
		w->resize(QSize(values[2], values[3]));
		w->move(QPoint(values[0], values[1]));
        if (qobject_cast<QMainWindow*>(w))
        {
			QByteArray docks = value("Docks", QByteArray()).toByteArray();
			if (docks.count())
				qobject_cast<QMainWindow*>(w)->restoreState(docks, 0);
        }
        else
            if (values[4]) // restore non-main windows
                w->show();
	}
	endGroup();
	return valid;
}

void Settings::setLayout(const QWidget* w, int x, int y, int wi, int he)
{
	if (!w || w->objectName().isEmpty())
		return;
	beginGroup("Geometry");
	QList<int> values;
    if (x != -1 || y != -1 || wi != -1 || he != -1)
        values << x << y << wi << he << w->isVisible();
    else
        values << w->x() << w->y() << w->width() << w->height() << w->isVisible();
	setList(w->objectName(), values);
	if (qobject_cast<const QMainWindow*>(w))
		setValue("Docks", qobject_cast<const QMainWindow*>(w)->saveState(0));
	endGroup();
}

QString Settings::dataPath()
{
    // TODO - alles Quark!
	if (m_dataPath.isNull()) {
		m_dataPath = qApp->applicationDirPath();
		if (m_dataPath.endsWith("/games/bin")) {
			m_dataPath.truncate(m_dataPath.length() - 10);
			m_dataPath.append("/share/games/shatrabase");
		} else if (m_dataPath.endsWith("/games")) {
			m_dataPath.truncate(m_dataPath.length() - 6);
			m_dataPath.append("/share/games/shatrabase");
		} else {
            if (m_dataPath.endsWith("/Contents/MacOS"))
				//Changed from 33 to 15 to include the data directory 
				//in the application directory for MaxOSX
				m_dataPath.truncate(m_dataPath.length() - 15);
            else if (m_dataPath.endsWith("/bin"))
				m_dataPath.truncate(m_dataPath.length() - 4);
			m_dataPath.append("/data");
		}
	}
//    qDebug() << m_dataPath;
	return m_dataPath;
}

void Settings::setList(const QString& key, QList<int> list)
{
	QList<QVariant> varlist;
	int i;
	foreach(i, list)
        varlist.append(QVariant(i));
	setValue(key, varlist);
}

bool Settings::list(const QString &key, QList<int>& list, int items)
{
	QList<QVariant> varlist = value(key).toList();
	if (items >= 0 && varlist.count() + list.count() != items)
		return false;
	QVariant v;
	foreach(v, varlist)
        list.append(v.toInt());
	return true;
}

void Settings::setByteArray(const QString& key, const QByteArray& arr)
{
    setValue(key, arr);
}

QByteArray Settings::byteArray(const QString& key)
{
    return value(key, QByteArray()).toByteArray();
}

void Settings::setMap(const QString& key, const OptionValueList& map)
{
    QByteArray data;
    QDataStream * stream = new QDataStream(&data, QIODevice::WriteOnly);

    (*stream) << map;
    delete stream;

    setByteArray(key, data);
}

void Settings::getMap(const QString& key, OptionValueList& map)
{
    QByteArray data = byteArray(key);
    QDataStream * stream = new QDataStream(&data, QIODevice::ReadOnly);
    (*stream) >> map;
    delete stream;
}

const QString& Settings::locale(const QString& l) const
{
    for (QMap<QString, QString>::const_iterator i=m_langMap.begin();
         i != m_langMap.end(); ++i)
    {
        if (i.value() == l)
            return i.key();
    }
    return m_defaultLocale;
}

const QString& Settings::language(const QString &locale) const
{
    QMap<QString, QString>::const_iterator i = m_langMap.find(locale);
    if (i == m_langMap.end())
        return m_defaultLanguageName;
    else
        return i.value();
}

QMap<QString, QVariant> Settings::initDefaultValues() const
{
    QMap<QString, QVariant> map;

    QString lang = language(QLocale::system().name().left(2));
    map.insert("/General/Language", lang);

    map.insert("/General/EditLimit", 10);
    map.insert("/General/Notation", NUM);
    map.insert("/General/useIndexFile", true);
    map.insert("/General/ListFontSize", DEFAULT_LISTFONTSIZE);
    map.insert("/General/autoCommitDB", false);
    map.insert("/GameText/FontSize",DEFAULT_FONTSIZE);
    map.insert("/MainWindow/GameToolBar", false);
    map.insert("/MainWindow/VerticalTabs", false);
    map.insert("/MainWindow/FilterFollowsGame", false);
    map.insert("/History/MaxEntries", 4);

    for (int i=1; i<=2; ++i)
    {
        map.insert(QString("/PlayGame/Player%1/engine").arg(i), "");
        map.insert(QString("/PlayGame/Player%1/name").arg(i), tr("Player %1").arg(i));
        map.insert(QString("/PlayGame/Player%1/minTime").arg(i), 2000);
        map.insert(QString("/PlayGame/Player%1/maxTime").arg(i), 6000);
    }

    // board config
    map.insert("/Board/external", false);
    map.insert("/Board/minWheelCount", MIN_WHEEL_COUNT);
    map.insert("/Board/AutoPlayerInterval", 3000);
    map.insert("/Board/AutoSaveAndContinue", false);
    map.insert("/Board/guessMove", true);
    map.insert("/Board/guessNextMove", true);
    // board style
    map.insert("/Board/flipped", true);
    map.insert("/Board/showMoat", true);
    map.insert("/Board/showTower", true);
    map.insert("/Board/showFrame", true);
    map.insert("/Board/showSquareNumbers", false);
    map.insert("/Board/frameWidth", 4);
    map.insert("/Board/showCurrentMove", true);
    map.insert("/Board/showAllMoves", true);
    map.insert("/Board/animateMoves", true);
    map.insert("/Board/animateMovesSpeed", 20.0);
    map.insert("/Board/animateMovesLength", 0.2);
    map.insert("/Board/animateMovesSpeedVsLength", 0.5);
    map.insert("/Board/pieceTheme", "motifshatra");
    map.insert("/Board/pieceEffect", BoardTheme::Plain);
    map.insert("/Board/boardTheme", "shatra1");
    map.insert("/Board/lightColor", QColor(Qt::lightGray));
    map.insert("/Board/darkColor", QColor(Qt::darkGray));
    map.insert("/Board/highlightColor", QColor(Qt::yellow));
    map.insert("/Board/frameColor", QColor(Qt::black));
    map.insert("/Board/currentMoveColor", QColor(Qt::blue));
    map.insert("/Board/backgroundColor", QColor(50,70,100));
    map.insert("/Board/backgroundColor2", QColor(90,70,50));

    return map;
}

QVariant Settings::getValue(const QString &key) const
{
    static QMap<QString, QVariant> defaultValues = initDefaultValues();
    if (defaultValues.contains(key))
    {
//        qDebug() << key;
        return value(key, defaultValues.value(key));
    }
    else
    {
        QString groupKey = QString("/") + group() + "/" + key;
//        qDebug() << groupKey;
        if (defaultValues.contains(groupKey))
        {
            return value(key, defaultValues.value(groupKey));
        }
    }
    Q_ASSERT_X(false, "request for unknown key", key.toStdString().c_str());
    return QVariant();
}

//////////////////////////////////////////////////////////////////////////////
// The singleton instance pointer of our AppSettings
//////////////////////////////////////////////////////////////////////////////

Settings* AppSettings;

//////////////////////////////////////////////////////////////////////////////
// EOF
//////////////////////////////////////////////////////////////////////////////

