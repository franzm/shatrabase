/***************************************************************************
 *                           Shatrabase                                    *
 *   (C) 2012-2014 AFKM <francis@afkm.com>                                 *
 *                   derived from ChessX with thanks                       *
 *   (C) 2006-2009 Michal Rudolf <mrudolf@kdewebdev.org>                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include <QFile>
#include "databaseinfo.h"
#include "filter.h"
#include "game.h"
#include "sgndatabase.h"
#include "memorydatabase.h"
#include "settings.h"

DatabaseInfo::DatabaseInfo()
{
    m_database = new MemoryDatabase;
    m_bLoaded = true;
    m_utf8 = false;
    newGame();
}

DatabaseInfo::DatabaseInfo(const QString& fname)
    : m_index(NewGame)
{
	m_filename = fname;
    m_bLoaded = false;
    m_utf8 = false;
	QFile file(fname);
    if (file.size() < 1024 * 1024 * AppSettings->getValue("/General/EditLimit").toInt())
        m_database = new MemoryDatabase;
    else if (file.size() < INT_MAX)
    {
        m_database = new SgnDatabase(false);
    }
    else
    {
        m_database = new SgnDatabase(true);
    }
}

void DatabaseInfo::doLoadFile(QString filename)
{
    if (!m_database->open(filename,m_utf8)) {
        emit LoadFinished(this);
        return;
    }
    g_loading = true;
    m_database->parseFile();
    if (g_resModified)
        m_database->setModified(true); // NB temporary fix
    m_bLoaded = true;
    emit LoadFinished(this);
    g_loading = false;
}

void DatabaseInfo::run()
{
    QFileInfo fi = QFileInfo(m_filename);
    QString fname = fi.canonicalFilePath();
    doLoadFile(fname);
}

bool DatabaseInfo::open(bool utf8)
{
    m_bLoaded = false;
    m_utf8 = utf8;
    start();
    return true;
}

void DatabaseInfo::close()
{
    m_bLoaded = false;
    m_database->m_break = true;
    if (isRunning())
    {
        bool bSuccess = wait(5000);
        if (!bSuccess)
        {
            terminate();
        }
    }
	if (m_database) delete m_database;
    m_database = NULL;
}

DatabaseInfo::~DatabaseInfo()
{
}

bool DatabaseInfo::loadGame(int index, bool reload)
{
    if (!m_bLoaded)
        return false;
	if (!isValid())
		return false;
	if (!reload && m_index == index)
		return true;
	if (!m_database || index < 0 || index >= m_database->count())
		return false;
	if (!m_database->loadGame(index, m_game))
		return false;
	m_index = index;
    //m_game.moveToId(index); // XXX Why is that?
    m_game.moveToEnd();
	m_game.setModified(false);
	return true;
}

void DatabaseInfo::newGame()
{
    m_game.clearTags();
	m_game.clear();
    m_game.setModified(false);
	m_index = NewGame;
}

bool DatabaseInfo::saveGame()
{
    if (!m_bLoaded)
        return false;
	if (!isValid() || m_database->isReadOnly())
		return false;

	if (m_index < m_database->count() && m_index >= 0)
    {
        if (m_database->replace(m_index, m_game))
        {
            m_game.setModified(false);
            return true;
        }
    }
    else if (m_index == NewGame && m_database->appendGame(m_game))
    {
		m_index = m_database->count() - 1;
        m_game.setModified(false);
		return true;
    }
    return false;
}

