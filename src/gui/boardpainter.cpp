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


// ------------------------- SquareItem -----------------------------

class SquareItem : public QGraphicsPixmapItem
{
public:
    SquareItem(Square square, const QPixmap& pixmap,
              QGraphicsItem * parent = 0)
        :   QGraphicsPixmapItem(pixmap, parent),
            square (square),
            highlight (false),
            temdek    (false)
    { }

    Square square;
    bool highlight;
    QBrush highlightBrush;
    bool temdek;
    QPen temdekPen;

protected:

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
    {
        QGraphicsPixmapItem::paint(painter, option, widget);
        if (highlight)
        {
            painter->setPen(QPen(Qt::NoPen));
            painter->setBrush(highlightBrush);
            painter->drawRect(QRect(0,0,pixmap().width(), pixmap().height()));
        }
        if (temdek)
        {
            painter->setPen(temdekPen);
            painter->setBrush(Qt::NoBrush);
            int o = temdekPen.width()/2;
            painter->drawLine(o, o, pixmap().width()-o, pixmap().height()-o);
            painter->drawLine(o, pixmap().height()-o, pixmap().width()-o, o);
        }
    }

};


// --------------------------- PieceItem ---------------------------

class PieceItem : public QGraphicsPixmapItem
{
public:
    PieceItem(Piece piece, Square square, const QPixmap& pixmap,
              QGraphicsItem * parent = 0)
        :   QGraphicsPixmapItem(pixmap, parent),
            piece  (piece),
            square (square)
    { }
    /** associated Piece */
    Piece piece;
    /** assoiciated Square */
    Square square,
    /** Square to go to in animation */
        squareTo;
    /** should this piece be animated (square > squareTo) */
    bool animate;
};




// ---------------------------- BoardPainter -----------------------


BoardPainter::BoardPainter(BoardTheme * theme, QWidget *parent)
    :
    QGraphicsView   (parent),
    m_theme         (theme),
    m_scene         (new QGraphicsScene(this)),
    m_drag_piece    (0),
    m_center        (4.5,7),
    m_size          (0),
    m_flipped       (false),
    m_anim_speed    (6)
{    
    setScene(m_scene);

    // get size of the bitmaps
    m_size = m_theme->rect().width();

    // get hover events
    setMouseTracking(true);

    // timer for animations
    m_timer.setInterval(1000/30);
    connect(&m_timer, &QTimer::timeout, this, &BoardPainter::animationStep);

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
    if (from != InvalidSquare && to != InvalidSquare)
    {
        PieceItem * p = pieceItemAt(to);
        if (!p) return; // or throw a logic exception ;)

        // setup the piece to animate
        p->square = from; // put back to start
        p->squareTo = to;
        p->animate = true;
        p->setPos(squarePos(p->square));
        // run an animation thread
        startAnimation_(from,to);
    }
}



void BoardPainter::createBoard_(const Board& board)
{
    // delete previous
    for (size_t i=0; i<m_squares.size(); ++i)
        delete m_squares[i];
    m_squares.clear();

    // create board squares
    for (Square i=fsq; i<=lsq; ++i)
    {
        const int x = gBoard[i][0],
                  y = gBoard[i][1];

        // select tile texture
        const QPixmap& pm = m_theme->square((x+y)&1);

        // setup tile
        SquareItem * s = new SquareItem(i, pm);
        s->setPos(squarePos(i));
        s->setZValue(-1);

        // set temdek flag
        if ((i == temdekAt[Black] && board.temdekOn(Black)) ||
            (i == temdekAt[White] && board.temdekOn(White)))
        {
            s->temdek = true;
            s->temdekPen = QPen(i==temdekAt[White]? Qt::black : Qt::white );
            s->temdekPen.setCapStyle(Qt::RoundCap);
            s->temdekPen.setWidthF(s->pixmap().width()/10);
        }


        // add to scene
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

PieceItem * BoardPainter::pieceItemAt(Square sq) const
{
    if (!(sq>=fsq && sq<=lsq)) return 0;

    // search the PieceItem that fits
    for (size_t i=0; i<m_pieces.size(); ++i)
        if (m_pieces[i]->square == sq)
            return m_pieces[i];

    return 0;
}

// ---------------- highlights ---------------------

void BoardPainter::setSquareColor(Square sq, const QColor& color)
{
    SquareItem * s = squareItemAt(sq);
    if (!s) return;

    QColor col(color);
    col.setAlpha(100);

    s->highlight = true;
    s->highlightBrush = QBrush(col);
    s->update();
}

void BoardPainter::clearSquareColor(Square sq)
{
    SquareItem * s = squareItemAt(sq);
    if (s) { s->highlight = false; s->update(); }
}

void BoardPainter::clearSquareColors()
{
    for (size_t i=0; i<m_squares.size(); ++i)
    {
        m_squares[i]->highlight = false;
        m_squares[i]->update();
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

    m_drag_piece->setPos(mapToScene(view) - QPointF(m_size>>1,m_size>>1));
}




// --------------------- animation -------------------------

void BoardPainter::startAnimation_(Square from, Square to)
{
    qreal dx = gBoard[from][0] - gBoard[to][0],
          dy = gBoard[from][1] - gBoard[to][1],
          dist = sqrt(dx*dx + dy*dy);

    if (dist <= 0) return;

    m_anim_length = m_anim_speed / dist;

    m_anim_t = 0;
    m_timer.start();
}

void BoardPainter::stopAnimation_()
{
    m_timer.stop();

    // set pieces to final positions (to be safe)
    for (size_t i=0; i<m_pieces.size(); ++i)
    if (m_pieces[i]->animate)
    {
        m_pieces[i]->square = m_pieces[i]->squareTo;

        m_pieces[i]->setPos(squarePos(m_pieces[i]->square));
    }

    emit moveFinished();
}

void BoardPainter::animationStep()
{
    const qreal step = m_anim_length * m_timer.interval() / 1000.0;
    m_anim_t += step;

    if (m_anim_t >= 1)
        stopAnimation_();

    // go through all pieces
    // XXX This is the generalized approach to move
    // more than one piece at the same time (but why?)
    for (size_t i=0; i<m_pieces.size(); ++i)
    if (m_pieces[i]->animate)
    {
        QPointF
            from = squarePos(m_pieces[i]->square),
            to = squarePos(m_pieces[i]->squareTo);

        from += m_anim_t * (to - from);

        m_pieces[i]->setPos(from);
    }
}
