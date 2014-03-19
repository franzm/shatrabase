/***************************************************************************
 *                           Shatrabase                                    *
 *   (C) 2012-2014 AFKM <francis@afkm.com>                                 *
 *                   derived from ChessX with thanks                       *
 *   (C) 2007 Marius Roets <roets.marius@gmail.com>                        *
 *   (C) 2007-2009 Michal Rudolf <mrudolf@kdewebdev.org>                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "database.h"
#include "filtermodel.h"

#include <QtGui>

#include <QSortFilterProxyModel>

FilterModel::FilterModel(Filter* filter, QObject* parent)
        : QAbstractItemModel(parent), m_filter(filter)
{
    m_columnNames
    << tr("Nr")
	<< tr("White")
	<< tr("WhiteElo")
	<< tr("Black")
	<< tr("BlackElo")
	<< tr("Event")
	<< tr("Site")
	<< tr("Round")
	<< tr("Date")
	<< tr("Result")
	<< tr("ECO")
    << tr("Moves");

    m_columnTags
    << "Nr"
	<< "White"
	<< "WhiteElo"
	<< "Black"
	<< "BlackElo"
	<< "Event"
	<< "Site"
	<< "Round"
	<< "Date"
	<< "Result"
	<< "ECO"
    << "Length";

    /*
    // for better sorting
    m_isnumber.push_back(1); // nr
    m_isnumber.push_back(0); // white
    m_isnumber.push_back(1); // w elo
    m_isnumber.push_back(0); // black
    m_isnumber.push_back(1); // b elo
    m_isnumber.push_back(0); // event
    m_isnumber.push_back(0); // site
    m_isnumber.push_back(1); // round
    m_isnumber.push_back(0); // date
    m_isnumber.push_back(1); // result
    m_isnumber.push_back(1); // eco
    m_isnumber.push_back(1); // length
    */
	m_game = new Game;
}

FilterModel::~FilterModel()
{
	delete m_game;
}

int FilterModel::rowCount(const QModelIndex& index) const
{
	if (index.isValid())
		return 0;
	return m_filter->count();
}

int FilterModel::columnCount(const QModelIndex&) const
{
	return m_columnNames.count();
}

QVariant FilterModel::data(const QModelIndex &index, int role) const
{
    if (index.isValid() && index.row() < m_filter->count())
    {
        // index to game
        int i = m_filter->indexToGame(index.row());
        if (i != -1)
        {


            if (role == Qt::DisplayRole)
            {
                // return row index number
                if (index.column() == 0)
                    return i + 1;

                // return a game value
                else
                {
                    // lookup cache
                    Hash cachepos = ((Hash)i<<MAX_COLUMNS_SHIFT) | (Hash)index.column();
                    CacheIter c = m_cache.find(cachepos);
                    if (c != m_cache.end())
                    {
                        qDebug() <<  m_cache.size();
                        return c.value();
                    }

                    // read game-header-info
                    m_filter->database()->loadGameHeaders(i, *m_game);
                    QString tag = m_game->tag(m_columnTags.at(index.column()));

                    // construct a variant
                    QVariant ret;
                    if (tag == "?")
                        // unknown
                        ret.setValue( QString("") );
                    else
                    {
                        bool ok;
                        // return as specific type?
                        { float num = tag.toFloat(&ok); if (ok) ret.setValue( num ); }
                        if (!ok) { int num = tag.toInt(&ok); if (ok) ret.setValue( num ); }
                        // return as string
                        if (!ok) ret.setValue( tag );
                    }

                    // store in cache
                    m_cache.insert(cachepos, ret);

                    return ret;
                }
            }
            else if (role == Qt::FontRole)
            {
                if (m_filter->database()->deleted(i))
                {
                    QFont font;
                    font.setStrikeOut(true);
                    return font;
                }
            }
            else if (role == Qt::ForegroundRole)
            {
                if (!m_filter->database()->getValidFlag(i))
                {
                    QVariant v = QColor(Qt::red);
                    return v;
                }
                QVariant v = QColor(Qt::black);
                return v;
            }
        }
    }
    return QVariant();
}

QVariant FilterModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role != Qt::DisplayRole)
		return QVariant();

	if (orientation == Qt::Horizontal)
		return QString("%1").arg(m_columnNames.at(section));
	else
		return QString("%1").arg(section);
}

QModelIndex FilterModel::index(int row, int column, const QModelIndex& parent) const
{
	if (parent.isValid())
		return QModelIndex();

    return createIndex(row, column);
}

void FilterModel::setFilter(Filter* filter)
{
    beginResetModel();
	m_filter = filter;
    endResetModel();
}

Filter* FilterModel::filter()
{
	return m_filter;
}

