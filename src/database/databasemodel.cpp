/***************************************************************************

Copyright (C) 2014  stefan.berke @ modular-audio-graphics.com

This source is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either
version 3.0 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License
along with this software; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA

****************************************************************************/

#include "databasemodel.h"

#include "database.h"
#include "game.h"

#include <QFont>
#include <QColor>

DatabaseModel::DatabaseModel(Database * db, QObject *parent) :
    QAbstractItemModel(parent),
    db_ (db)
{
    columnNames_
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
    << tr("Moves")
    << tr("Ply")
    << tr("Pieces White")
    << tr("Pieces Black")
    << tr("Opening move");

    columnTags_
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
    << "Length"
    << "Ply"
    << "Pieces White"
    << "Pieces Black"
    << "First move";

}
/*
void DatabaseModel::setDatabase(Database * db)
{

}*/




QModelIndex DatabaseModel::index(int row, int column, const QModelIndex &parent) const
{
    if (parent.isValid()) return QModelIndex();

    return createIndex(row, column);
}

QModelIndex DatabaseModel::parent(const QModelIndex &child) const
{
    Q_UNUSED(child);
    return QModelIndex();
}

int DatabaseModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) return 0;
    return db_->count();
}


int DatabaseModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid()) return 0;
    return columnTags_.count();
}

QVariant DatabaseModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal)
        return columnNames_[section];
    else
        return QString::number(section);
}


QVariant DatabaseModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < db_->count()
        && index.column() < columnTags_.size())
    {
        const int i = index.row();

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
                CacheIter c = cache_.find(cachepos);
                // return cached value
                if (c != cache_.end())
                    return c.value();

                // -- get info first time --

                // read game-header-info
                Game game;
                db_->loadGameHeaders(i, game);
                QString tag = game.tag(columnTags_.at(index.column()));

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
                cache_.insert(cachepos, ret);

                return ret;
            }
        }
        else if (role == Qt::FontRole)
        {
            if (db_->deleted(i))
            {
                QFont font;
                font.setStrikeOut(true);
                return font;
            }
        }
        else if (role == Qt::ForegroundRole)
        {
            if (!db_->getValidFlag(i))
            {
                QVariant v = QColor(Qt::red);
                return v;
            }
            QVariant v = QColor(Qt::black);
            return v;
        }
    }
    return QVariant();
}
