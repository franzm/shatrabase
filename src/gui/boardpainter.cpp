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


/** Wrapper around QGraphicsRectItem to assign a Square number */
class SquareItem : public QGraphicsRectItem
{
public:
    explicit SquareItem(Square square, QGraphicsItem * parent = 0)
    :   QGraphicsRectItem(parent),
        m_square(square)
    { }

    Square square() const { return m_square; }
protected:
    Square m_square;
};








BoardPainter::BoardPainter(BoardTheme * theme, QWidget *parent)
    :
    QGraphicsView   (parent),
    m_theme         (theme),
    m_scene         (new QGraphicsScene(this)),
    m_size          (50)
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

        SquareItem * s = new SquareItem(i);
        s->setRect(QRect((x-4.5)*m_size,(y-7)*m_size,m_size,m_size));
        s->setPen(QPen(Qt::NoPen));
        s->setBrush(QBrush(pm));

        m_scene->addItem(s);
    }
}



// ---------------------- events -------------------------

void BoardPainter::mousePressEvent(QMouseEvent *event)
{
    // transform mouse coords to scene
    QPointF p = mapToScene(event->pos());

    // find clicked item
    QGraphicsItem * item = m_scene->itemAt(p, transform());
    if (!item) return;

    if (event->button() == Qt::LeftButton)
    {
        // clicked on square?
        SquareItem * sq = dynamic_cast<SquareItem*>(item);
        if (sq)
        {
            signalSquareClicked(sq->square());
            event->accept();
        }

    }
}
