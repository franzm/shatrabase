/***************************************************************************
 *                           Shatrabase                                    *
 *   (C) 2012-2014 AFKM <francis@afkm.com>                                 *
 *                   derived from ChessX with thanks                       *
 *   (C) 2005-2006 William Hoggarth <whoggarth@users.sourceforge.net       *
 *   (C) 2007-2009 Michal Rudolf <mrudolf@kdewebdev.org>                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include <QFile>
#include <QFileInfo>
#include <QtDebug>
#include "database.h"

Database::Database() : m_break(false)
{
    connect(&m_index, SIGNAL(progress(int)), this, SIGNAL(progress(int)));
}

Database::~Database()
{
}

Index* Database::index()
{
	return &m_index;
}

const Index* Database::index() const
{
    return &m_index;
}

QString Database::name() const
{
	QFileInfo fi(filename());
	return fi.completeBaseName();
}

struct LocalGameInfo
{
    int gameIndex;
    MoveList moves;
};
typedef QVector<LocalGameInfo> LocalGameInfos;

void Database::findDuplicates(QVector<int>& indices, bool do_symmetric)
{
    Game game;

#define SB_MIRROR(at__) \
    ( ((at__)&15) + (8 - (at__)/16) * 16 )

#define SB_TO_ONE_SIDE(at__) \
    ((at__)/16 <= 4 ? (at__) : SB_MIRROR(at__))

    // first:
    // find all games that start with the same move
    // and have the same length
    QMap<int, LocalGameInfos> map;
    for (int i=0; i<count(); ++i)
    {
        loadGameMoves(i, game);

        LocalGameInfo inf;
        inf.gameIndex = i;
        game.getAllPlies(inf.moves);

        if (inf.moves.empty())
            continue;

        int index;
        if (do_symmetric)
        {
            index =  (int)SB_TO_ONE_SIDE(inf.moves[0].from())
                  | ((int)SB_TO_ONE_SIDE(inf.moves[0].to()) << 8)
                  | ((int)inf.moves.count() << 16);
        }
        else
        {
            index =  (int)inf.moves[0].from()
                  | ((int)inf.moves[0].to() << 8)
                  | ((int)inf.moves.count() << 16);
        }

        map[index].push_back(inf);
    }

    qDebug() << "games " << count() << "/ map " << map.count();

    QSet<int> dups;
    int numsym = 0;

    // now find duplicates
    for (QMap<int, LocalGameInfos>::const_iterator i = map.begin(); i!=map.end(); ++i)
    {
        const LocalGameInfos & inf = i.value();

        // ignore single entries
        if (inf.count() < 2)
            continue;

        qDebug() << inf.count() << " games starting "
                 << BN[inf[0].moves[0].from()] << "-" << BN[inf[0].moves[0].to()]
                 << " and length " << inf[0].moves.count();

        // compare each
        for (int j=0; j<inf.count(); ++j)
        for (int k=j+1; k<inf.count(); ++k)
        {
            bool equal = true;
            for (int l=0; l<inf[j].moves.count(); ++l)
                if (inf[j].moves[l].from() != inf[k].moves[l].from()
                 || inf[j].moves[l].to() != inf[k].moves[l].to())
            {
                equal = false;
                break;
            }

            // compare symmetric
            bool sym = false;
            if (!equal && do_symmetric)
            {
                equal = true;
                for (int l=0; l<inf[j].moves.count(); ++l)
                    if (inf[j].moves[l].from() != SB_MIRROR(inf[k].moves[l].from())
                     || inf[j].moves[l].to() != SB_MIRROR(inf[k].moves[l].to()))
                {
                    equal = false;
                    break;
                }
                if (equal)
                {
                    sym = true;
                    numsym++;
                }
            }

            // store index to duplicate game
            if (equal)
            {
                int dup = inf[k].gameIndex;
#ifdef QT_DEBUG
                if (dups.find(dup) == dups.end())
                {
                    if (!sym)
                        qDebug() << inf[k].gameIndex << "is a duplicate of " << inf[j].gameIndex;
                    else
                        qDebug() << inf[k].gameIndex << "is a mirrored duplicate of " << inf[j].gameIndex;
                }
#endif
                dups.insert(dup);
            }
        }
    }

#undef SB_MIRROR
#undef SB_TO_ONE_SIDE

    if (!numsym)
        qDebug() << "found" << dups.count() << "duplicates";
    else
        qDebug() << "found" << dups.count() << "duplicates of which" << numsym << "where symmetries.";

    indices.clear();
    for (QSet<int>::const_iterator i=dups.begin(); i!=dups.end(); ++i)
        indices.push_back(*i);
}
