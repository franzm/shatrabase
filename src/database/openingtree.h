/***************************************************************************
 *                           Shatrabase                                    *
 *   (C) 2012-2014 AFKM <francis@afkm.com>                                 *
 *                   derived from ChessX with thanks                       *
 *   (C) 2007-2009 Michal Rudolf <mrudolf@kdewebdev.org>                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef __OPENINGTREE_H__
#define __OPENINGTREE_H__

#include <QAbstractTableModel>

#include "database.h"
#include "game.h"

class MoveData
{
public:
	enum MoveType {StandardMove, GameEnd};
	MoveData();
    void addGame(Game& g, SHATRA::Color c, MoveType movetype = StandardMove);
	double percentage() const;
	int averageRating() const;
	int averageYear() const;
	QString move;
	unsigned count;
	unsigned result[4];
	long rating;
	unsigned rated;
	long year;
	unsigned dated;
	friend bool operator<(const MoveData& m1, const MoveData& m2);
	friend bool compareMove(const MoveData& m1, const MoveData& m2);
	friend bool compareScore(const MoveData& m1, const MoveData& m2);
	friend bool compareRating(const MoveData& m1, const MoveData& m2);
	friend bool compareYear(const MoveData& m1, const MoveData& m2);
};


class OpeningTreeUpdater : public QThread
{
    Q_OBJECT
public:
    void run();
    void cancel();
    bool update(Database& f, const SHATRA::Board& b, QList<MoveData>&, int&, bool updateFilter);

signals:
    void UpdateFinished(SHATRA::Board*);
    void UpdateTerminated(SHATRA::Board*);
    void progress(int);

private:
    QList<MoveData>* m_moves;
    int* m_games;

    bool    m_break;
    bool    m_updateFilter;
    SHATRA::Board   m_board;
    Database* m_database;
};

/** @ingroup Search
The OpeningTree class is a class to calculate opening tree for given position. */

class OpeningTree : public QAbstractTableModel
{
	Q_OBJECT
public:
	/** Empty constructor. */
    OpeningTree(QObject * parent);
    /** Initialized constructor, performing automatic update */
    OpeningTree(Database& db, const SHATRA::Board& b, bool updateFilter, QObject * parent);
    /** Calculate opening tree from given position, using given filter. It sets
    the filter to contain only game matching position @p b .
    @return true if the update was not cancelled.*/
    bool update(Database& db, const SHATRA::Board& b, bool updateFilter);
    /** Debug string */
	QString debug();
	/** Returns the number of moves in the Opening Tree */
	virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
	/** Returns the number of columns of the Opening Tree */
	virtual int columnCount(const QModelIndex& parent = QModelIndex()) const;
	/** Returns an item of data given the item 'index' */
	virtual QVariant data(const QModelIndex &index, int role) const;
	/** Returns the header information for header 'section' */
	virtual QVariant headerData(int section, Qt::Orientation orientation,
										 int role = Qt::DisplayRole) const;
	/** Support sorting. */
	virtual void sort(int column, Qt::SortOrder order);
	/** Support sorting. */
	virtual void sort();
	/** Move leading to given entry. */
	QString move(const QModelIndex& index) const;
public slots:
    /** Cancel a running update */
    void cancel(bool);
protected slots:
    void updateFinished(SHATRA::Board*);
    void updateTerminated(SHATRA::Board*);
signals:
	void progress(int);
    void openingTreeUpdated();
    void openingTreeUpdateStarted();
private:
    bool m_bRequestPending;
    QList<MoveData> m_moves;
    int m_games;
	QStringList m_names;
	int m_sortcolumn;
	Qt::SortOrder m_order;
    SHATRA::Board m_board;
    Database* m_database;
    bool m_updateFilter;
};

#endif

