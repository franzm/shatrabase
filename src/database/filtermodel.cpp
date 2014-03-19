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
        : QAbstractItemModel(parent), m_filter(filter), m_gameIndex(-1)
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
        if (i != -1) {
            if (i != m_gameIndex) {
// rico: it would perhaps be better to read here only header information that is
// currently used and not the whole header information.
                m_filter->database()->loadGameHeaders(i, *m_game);
                m_gameIndex = i;
            }
            if (role == Qt::DisplayRole)
            {
                // return index number
                if (index.column() == 0)
                    return i + 1;
                else
                {
                    // read game-header-info
                    QString tag = m_game->tag(m_columnTags.at(index.column()));
                    if (tag == "?") return QString("");
                    bool ok;
                    // return as specific type?
                    { float num = tag.toFloat(&ok); if (ok) return num; }
                    { int num = tag.toInt(&ok); if (ok) return num; }
                    // return as string
                    return tag;
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

    // sorted index
/*  if (m_sorted.size() == m_filter->database()->count())
    {
        qDebug() << "sorted " << row << "->" << m_sorted[row];
        row = m_sorted[row];
    }
*/
    return createIndex(row, column);
}

void FilterModel::setFilter(Filter* filter)
{
    beginResetModel();
	m_filter = filter;
	m_gameIndex = -1;
    endResetModel();
}

Filter* FilterModel::filter()
{
	return m_filter;
}

#ifdef DONE_DIFFERENTLY_NOW__
void FilterModel::sort(int column, Qt::SortOrder order)
{
    Q_ASSERT(column < m_isnumber.size());
    if (column >= m_isnumber.size())
        return;

    emit layoutAboutToBeChanged();

    // recreate this array
    m_sorted.resize(m_filter->database()->count());

    // number columns are handled separately
    if (m_isnumber[column])
    {
        // preload all game's tags
        std::multimap<float,int> map;
        for (int i=0; i<m_sorted.size(); ++i)
        {
            //const QModelIndex oldi = index(i,column);
            int ig = m_filter->indexToGame(i);//oldi.row());
            if (ig != -1)
            {
                // XXX inefficient
                m_filter->database()->loadGameHeaders(ig, *m_game); m_gameIndex = ig;

                QString tag = m_game->tag(m_columnTags.at(column));
                map.insert(std::make_pair(tag.toFloat(), i));
            }
            else map.insert(std::make_pair(0.f,i));
        }

        std::multimap<float,int>::iterator m = map.begin();
        for (int i=0; i<m_sorted.size(); ++i, ++m)
            m_sorted[i] = m->second;
    }
    // compare by string
    else
    {
        // preload all game's tags
        std::multimap<QString,int> map;
        for (int i=0; i<m_sorted.size(); ++i)
        {
            int ig = m_filter->indexToGame(i);
            if (ig != -1)
            {
                m_filter->database()->loadGameHeaders(ig, *m_game); m_gameIndex = ig;

                QString tag = m_game->tag(m_columnTags.at(column));
                map.insert(std::make_pair(tag, i));
            }
            else map.insert(std::make_pair("",i));
        }

        std::multimap<QString,int>::iterator m = map.begin();
        for (int i=0; i<m_sorted.size(); ++i, ++m)
            m_sorted[i] = m->second;
    }

    // reverse array
    if (order != Qt::AscendingOrder)
    {
        for (int i=0; i<m_sorted.size()/2; ++i)
            std::swap(m_sorted[i], m_sorted[m_sorted.size()-1-i]);
    }

    emit layoutChanged();

    /*
    QModelIndexList from, to;
    for (int i=0; i<m_sorted.size(); ++i)
    {
        from.push_back(index(i, column));
        to.push_back(index(m_sorted[i],column));
    }
    changePersistentIndexList(from, to);
    */
/*
    qSort(m_sorted.begin(), m_sorted.end(), [&](){ * XXX should use c++11 * });
*/
}
#endif

