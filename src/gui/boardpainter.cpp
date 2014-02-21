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


class SquareItem : public QGraphicsPixmapItem
{
public:
    SquareItem(Square square, const QPixmap& pixmap,
              QGraphicsItem * parent = 0)
        :   QGraphicsPixmapItem(pixmap, parent),
            square (square),
            selected (false)
    { }

protected:

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
    {
        QGraphicsPixmapItem::paint(painter, option, widget);
        if (selected)
        {
            painter->setPen(QPen(Qt::NoPen));
            painter->setBrush(QBrush(QColor(100,255,100,100)));
            painter->drawRect(QRect(0,0,pixmap().width(), pixmap().height()));
        }
    }

public:

    Square square;
    bool selected;
};

class PieceItem : public QGraphicsPixmapItem
{
public:
    PieceItem(Piece piece, Square square, const QPixmap& pixmap,
              QGraphicsItem * parent = 0)
        :   QGraphicsPixmapItem(pixmap, parent),
            piece  (piece),
            square (square)
    { }

    Piece piece;
    Square square;
};



BoardPainter::BoardPainter(BoardTheme * theme, QWidget *parent)
    :
    QGraphicsView   (parent),
    m_theme         (theme),
    m_scene         (new QGraphicsScene(this)),
    m_selected_square(0),
    m_drag_piece    (0),
    m_center        (4.5,7),
    m_size          (50),
    m_flipped       (false)
{    
    setScene(m_scene);

    m_theme->setSize(QSize(m_size, m_size));

    setMouseTracking(true);
#if (0) // XXX that basically works ;)
    QTransform t = transform();
    t.rotate(70, Qt::XAxis);
    setTransform(t);
#endif
}


void BoardPainter::setBoard(const Board& board, int from, int to)
{
    createBoard_(board);
    createPieces_(board);
}



void BoardPainter::createBoard_(const Board& board)
{
    // delete previous
    for (size_t i=0; i<m_squares.size(); ++i)
        delete m_squares[i];
    m_squares.clear();

    m_selected_square = 0;

    // create board squares
    for (Square i=fsq; i<=lsq; ++i)
    {
        const int x = gBoard[i][0],
                  y = gBoard[i][1];

        const QPixmap& pm = m_theme->square((x+y)&1);

        SquareItem * s = new SquareItem(i, pm);
        s->setPos(squarePos(i));
        s->setZValue(-1);

        m_scene->addItem(s);
        m_squares.push_back(s);
    }
}



void BoardPainter::createPieces_(const Board& board)
{
    // delete previous
    for (size_t i=0; i<m_pieces.size(); ++i)
        delete m_pieces[i];
    m_pieces.clear();

    m_drag_piece = 0;

    // create pieces
    for (Square i=fsq; i<=lsq; ++i)
    {
        Piece p = board.pieceAt(i);
        if (p == InvalidPiece) continue;

        // pixmap for piece
        const QPixmap& pm = m_theme->piece(p);

        PieceItem * item = new PieceItem(p, i, pm);
        item->setPos(squarePos(i));

        // add to scene
        m_scene->addItem(item);
        m_pieces.push_back(item);
    }
}



// -------------------- coords ---------------------------

QRect BoardPainter::squareRect(Square sq) const
{
    const int x = gBoard[sq][0],
              y = gBoard[sq][1];

    return QRect(
            (x-m_center.x())*m_size,
            (y-m_center.y())*m_size,
            m_size, m_size
            );
}

QPoint BoardPainter::mapToBoard(const QPoint& viewpos) const
{
    // transform mouse coords to scene
    QPointF p = mapToScene(viewpos);
    // cancel scale
    p /= m_size;
    // cancel board placement
    p += m_center;

    return QPoint(
        p.x(),
        (int)(p.y()+1)-1 // avoid negative fraction rounding to zero
        );
}

Square BoardPainter::squareAt(const QPoint& viewpos) const
{
    QPoint p = mapToBoard(viewpos);

    if (p.x() <= 0 || p.y() < 0 || p.x() >= 8 || p.y() >= 15)
        return InvalidSquare;

    Square sq = isFlipped() ?
                BN[((8 - p.x())<<4) + p.y() + 1] :
                BN[(p.x()<<4) + 14 - p.y()];

    // make illegal output always InvalidSquare,
    // otherwise would be 0 from BN[]
    return (sq>=fsq && sq<=lsq)? sq : InvalidSquare;
}

SquareItem * BoardPainter::squareItemAt(Square sq) const
{
    if (!(sq>=fsq && sq<=lsq)) return 0;

    // search the SquareItem that fits
    for (size_t i=0; i<m_squares.size(); ++i)
        if (m_squares[i]->square == sq)
            return m_squares[i];

    return 0;
}


// ---------------- highlights ---------------------

void BoardPainter::selectSquare(Square sq)
{
    //if previously selected
    if (m_selected_square)
    {
        // dont update what's not needed
        if (m_selected_square->square == sq)
            return;

        // unslect
        if (m_selected_square->selected)
        {
            m_selected_square->selected = false;
            m_selected_square->update();
        }
    }

    m_selected_square = squareItemAt(sq);

    if (m_selected_square)
    {
        m_selected_square->selected = true;
        m_selected_square->update();
    }
}

void BoardPainter::setDragPiece(Square sq, Piece piece, const QPoint& view)
{
    bool remove = (sq == InvalidSquare || piece == InvalidPiece);

    // delete previous
    if (remove || (m_drag_piece && m_drag_piece->piece != piece))
    {
        delete m_drag_piece;
        m_drag_piece = 0;
    }

    // simply undo selection
    if (remove) return;

    // create or refresh
    if (!m_drag_piece)
    {
        m_drag_piece = new PieceItem(piece, sq, m_theme->piece(piece));
        m_scene->addItem(m_drag_piece);
    }

    m_drag_piece->setPos(mapToScene(view));
}
