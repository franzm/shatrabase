/***************************************************************************
 *                           Shatrabase                                    *
 *   (C) 2012-2014 AFKM <francis@afkm.com>                                 *
 *                   derived from ChessX with thanks                       *
 *   (C) 2005 Michal Rudolf <mrudolf@kdewebdev.org>                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "boardpainter.h"

#include "boardtheme.h"
#include "../database/sboard.h"

#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsRectItem>

const int gBoard[64][2] = // graphics board x, y
{
     {0,0},        {3,0}, {4,0}, {5,0},
                   {3,1}, {4,1}, {5,1},
                   {3,2}, {4,2}, {5,2},
                          {4,3},
     {1,4}, {2,4}, {3,4}, {4,4}, {5,4}, {6,4}, {7,4},
     {1,5}, {2,5}, {3,5}, {4,5}, {5,5}, {6,5}, {7,5},
     {1,6}, {2,6}, {3,6}, {4,6}, {5,6}, {6,6}, {7,6},
     {1,7}, {2,7}, {3,7}, {4,7}, {5,7}, {6,7}, {7,7},
     {1,8}, {2,8}, {3,8}, {4,8}, {5,8}, {6,8}, {7,8},
     {1,9}, {2,9}, {3,9}, {4,9}, {5,9}, {6,9}, {7,9},
                          {4,10},
                  {3,11}, {4,11}, {5,11},
                  {3,12}, {4,12}, {5,12},
                  {3,13}, {4,13}, {5,13},      {0,0},
};





BoardPainter::BoardPainter(BoardTheme * theme, QWidget *parent)
    :
    QGraphicsView   (parent),
    m_theme         (theme),
    m_scene         (new QGraphicsScene(this)),
    m_center        (4.5,7),
    m_size          (50),
    m_flipped       (false)
{
    setScene(m_scene);
    createBoard_();

#if (0) // XXX that basically works ;)
    QTransform t = transform();
    t.rotate(70, Qt::XAxis);
    setTransform(t);
#endif
}



void BoardPainter::createBoard_()
{
    m_theme->setSize(QSize(m_size, m_size));

    // create board squares
    for (Square i=fsq; i<=lsq; ++i)
    {
        const int x = gBoard[i][0],
                  y = gBoard[i][1];

        const QPixmap& pm = m_theme->square((x+y)&1);

        QGraphicsRectItem * s = new QGraphicsRectItem;
        s->setRect((x-m_center.x())*m_size,
                   (y-m_center.y())*m_size,
                   m_size, m_size);
        s->setPen(QPen(Qt::NoPen));
        s->setBrush(QBrush(pm));

        m_scene->addItem(s);
    }
}



// -------------------- coords ---------------------------

Square BoardPainter::squareAt(const QPoint& viewpos) const
{
    // transform mouse coords to scene
    QPointF p = mapToScene(viewpos);
    // cancel scale
    p /= m_size;
    // cancel board placement
    p += m_center;

    int x = p.x(),
        y = (int)(p.y()+1)-1; // avoid negative fraction round to zero

    if (x <= 0 || y < 0 || x >= 8 || y >= 15)
        return InvalidSquare;

    Square sq = isFlipped() ?
                BN[((8 - x)<<4) + y + 1] :
                BN[(x<<4) + 14 - y];

    return (sq>=fsq && sq<=lsq)? sq : InvalidSquare;
}

