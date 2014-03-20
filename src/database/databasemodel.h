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

#ifndef DATABASEMODEL_H
#define DATABASEMODEL_H

#include <QAbstractItemModel>
#include <QStringList>

class Database;

/** New wrapper around games in Database.
  */
class DatabaseModel : public QAbstractItemModel
{
    Q_OBJECT

    /** bitshift for cache value.
     *  Implicitly defines max number of columns!! */
    static const int MAX_COLUMNS_SHIFT = 8;

public:
    explicit DatabaseModel(Database * db, QObject *parent = 0);

    /* Changes the current database. */
    //void setDatabase(Database * db);

    // --- itemmodel interface impl. ---

    virtual QModelIndex index(int row, int column, const QModelIndex &parent) const;
    virtual QModelIndex parent(const QModelIndex &child) const;
    virtual int rowCount(const QModelIndex &parent) const;
    virtual int columnCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;

    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;

signals:

public slots:

protected:
    Database * db_;
    QStringList columnNames_, columnTags_;

    /** cache for game values */
    typedef quint64 Hash;
    mutable QHash<Hash, QVariant> cache_;
    typedef QHash<Hash, QVariant>::const_iterator CacheIter;
};

#endif // DATABASEMODEL_H
