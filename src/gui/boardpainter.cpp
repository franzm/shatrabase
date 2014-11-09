/***************************************************************************
 *                           Shatrabase                                    *
 *   (C) 2012-2014 AFKM <francis@afkm.com>                                 *
 *                   derived from ChessX with thanks                       *
 *   (C) 2005 Michal Rudolf <mrudolf@kdewebdev.org>                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

// QWinFontEngine: unable to query transformed glyph metrics (GetGlyphOutline() failed, error 1003)... (Die Funktion kann nicht abgeschlossen werden.)

#include "boardpainter.h"

#include "settings.h"
#include "boardtheme.h"

#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsRectItem>

#if (0)
#   define SB_PAINTER_DEBUG(arg__) { qDebug() << arg; }
#else
#   define SB_PAINTER_DEBUG(unused__)
#endif

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
                  {3,13}, {4,13}, {5,13},      {0,0}
};


// ------------------------- SquareItem -----------------------------

class SquareItem : public QGraphicsPixmapItem
{
public:
    SquareItem(Square square, const QPixmap& pixmap,
               const BoardPainter::Decoration& decoration,
               QGraphicsItem * parent = 0)
        :   QGraphicsPixmapItem(pixmap, parent),
            square     (square),
            deco       (decoration),
            overlay    (0),
            frame      (false),
            temdek     (false),
            highlights (0)
    { }

    Square square;

    const BoardPainter::Decoration& deco;

    const QPixmap
    /** use for specific square decoration */
        * overlay;

    /** Show frame around square */
    bool frame;

    /** 0=off, 1 = White, 2 = Black */
    int temdek,
    /** Or combi of BoardPainter::Highlight */
        highlights;

    QString numberStr;

protected:

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
    {
        QGraphicsPixmapItem::paint(painter, option, widget);

        // overlay graphic
        if (overlay && !overlay->isNull())
            painter->drawPixmap(0,0,*overlay);

        // hover
        if (highlights & BoardPainter::H_HOVER)
        {
            painter->setPen(QPen(Qt::NoPen));
            painter->setBrush(deco.highlightBrush[0]);
            painter->drawRect(QRect(0,0,pixmap().width(), pixmap().height()));
        }
        // goal highlight (every possible goal)
        if (highlights & BoardPainter::H_GOAL)
        {
            painter->setPen(Qt::NoPen);
            painter->setBrush(deco.highlightBrush[1]);
            int r = pixmap().width() / 1.5;
            painter->drawRect(QRect(r/2,r/2,pixmap().width()-r, pixmap().height()-r));
        }
        // target highlight (selected target)
        if (highlights & BoardPainter::H_TARGET)
        {
            painter->setBrush(Qt::NoBrush);
            painter->setPen(deco.highlightPen[1]);
            painter->drawRect(QRect(deco.highlightPen[1].width()/2,
                                    deco.highlightPen[1].width()/2,
                                    pixmap().width() - deco.highlightPen[1].width(),
                                    pixmap().height() - deco.highlightPen[1].width()));
        }
        // temdek cross
        if (temdek)
        {
            painter->setPen(temdek == 1? deco.whiteTemdekPen : deco.blackTemdekPen);
            painter->setBrush(Qt::NoBrush);
            int o = deco.whiteTemdekPen.width()/2;
            painter->drawLine(o, o, pixmap().width()-o, pixmap().height()-o);
            painter->drawLine(o, pixmap().height()-o, pixmap().width()-o, o);
        }
        // frame
        if (frame)
        {
            painter->setPen(deco.framePen);
            painter->setBrush(Qt::NoBrush);
            painter->drawRect(QRect(0,0,pixmap().width(), pixmap().height()));
        }
        // number display
        if (!numberStr.isNull())
        {
            painter->setFont(deco.font);
            painter->setPen(deco.fontPen);
            painter->drawText(pixmap().rect(),
                              Qt::AlignCenter | Qt::AlignHCenter, numberStr);
        }
        // select highlight
        if (highlights & BoardPainter::H_SELECT)
        {
            painter->setPen(deco.selectPen);
            painter->setBrush(Qt::NoBrush);
            painter->drawRect(QRect(deco.selectPen.width()/2,
                                    deco.selectPen.width()/2,
                                    pixmap().width() - deco.selectPen.width(),
                                    pixmap().height() - deco.selectPen.width()));
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
            piece           (piece),
            square          (square),
            squareTo        (InvalidSquare),
            animate         (false),
            animateRemove   (false),
            anim_length     (0),
            animatePixmap   (0),
            overlay(0)
    { }

    ~PieceItem()
    {
        if (animatePixmap)
            delete animatePixmap;
    }

    /** associated Piece */
    Piece piece;
    /** assoiciated Square */
    Square square,
    /** Square to go to in animation */
        squareTo;
    /** should this piece be animated (square > squareTo) */
    bool animate,
         animateRemove;
    int anim_length;

    /** if animate==true, this triggers a pixmap switch when != 0 */
    QPixmap * animatePixmap;
    const QPixmap * overlay;

    /* Sets 180 rotation on/off */
    /*void setRotate(bool doit)
    {
        QTransform t = transform();
        if (!doit)
            t.reset();
        else
        {
            t.translate(pixmap().width(), pixmap().height());
            t.rotate(180);
        }
        setTransform(t);
    }*/

    void setScale(qreal scale)
    {
        QTransform t = transform();
        t.reset();
        t.scale(scale, scale);
        //t.translate(0.5, 0.5);
        qreal fak = (1.0-scale)/2.0;
        if (scale != 0)
            fak /= scale;
        t.translate(fak * pixmap().width(), fak * pixmap().height());
        setTransform(t);
    }

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
    {
        QGraphicsPixmapItem::paint(painter, option, widget);

        // draw overlay
        if (overlay && !overlay->isNull())
            painter->drawPixmap(0,0,*overlay);
    }
};







// ---------------------------- BoardPainter -----------------------


BoardPainter::BoardPainter(BoardTheme * theme, QWidget *parent)
    :
    QGraphicsView   (parent),
    m_theme         (theme),
    m_scene         (new QGraphicsScene(this)),
    m_org_drag_piece(0),
    m_drag_piece    (0),
    m_move_white    (0),
    m_move_black    (0),
    m_center        (4.5,7),
    m_size          (0),
    m_frame_width   (1),
    m_flipped       (true), /* XXX This probably has to match the init value in AppSettings
                                   to be correct on first installation */
    m_is_white      (true),
    m_do_moat       (true),
    m_do_square_numbers(false),
    m_do_animate    (true),
    m_do_show_side  (true),
    m_anim_speed    (10),
    m_fixed_anim_length(1),
    m_use_fixed_anim_length(0),
    m_animations    (0),
    m_anim_length   (0)
{    
    SB_PAINTER_DEBUG("BoardPainter" << this);
    setScene(m_scene);

    // get size of the bitmaps
    m_size = m_theme->rect().width();

    // XXX need to do this although we don't want the events here...
    setMouseTracking(true);

    connect(&m_anim_timer, SIGNAL(timeout()), SLOT(animationStep_()));
    m_anim_timer.setSingleShot(false);
    m_anim_timer.setInterval(1000 / 60);

    configure();



#if (0) // XXX that basically works ;)
    QTransform t = transform();
    t.rotate(70, Qt::XAxis);
    setTransform(t);
#endif

    setRenderHint(QPainter::Antialiasing, true);
    setRenderHint(QPainter::HighQualityAntialiasing, true);
    setRenderHint(QPainter::SmoothPixmapTransform, true);

}


// ----------------- config -------------------

void BoardPainter::configure()
{
    AppSettings->beginGroup("/Board/");
    //m_do_show_side = ...
//    m_do_moat = AppSettings->getValue("showMoat").toBool();
//    m_do_tower = AppSettings->getValue("showTower").toBool();
    m_do_moat = g_version == 2;
    m_do_tower = m_do_moat;
    m_do_show_frame = AppSettings->getValue("showFrame").toBool();
    m_do_square_numbers = AppSettings->getValue("showSquareNumbers").toBool();
    m_frame_width = AppSettings->getValue("frameWidth").toInt();
    m_do_animate = AppSettings->getValue("animateMoves").toBool();
    m_anim_speed = AppSettings->getValue("animateMovesSpeed").toDouble();
    m_fixed_anim_length = AppSettings->getValue("animateMovesLength").toDouble();
    m_use_fixed_anim_length = AppSettings->getValue("animateMovesSpeedVsLength").toDouble();
    m_deco.hoverColor = AppSettings->getValue("highlightColor").value<QColor>();
    m_deco.hoverColor.setAlpha(70);
    m_deco.selectColor = AppSettings->getValue("currentMoveColor").value<QColor>();
    m_deco.selectColor.setAlpha(150);
    QColor back1 = AppSettings->getValue("backgroundColor").value<QColor>();
    QColor back2 = AppSettings->getValue("backgroundColor2").value<QColor>();
    AppSettings->endGroup();

    // setup background
    QLinearGradient grad(QPointF(0,-1), QPointF(0,1));
    grad.setColorAt(0, back1);
    grad.setColorAt(1, back2);
    QBrush b(grad);
    setBackgroundBrush(b);

}


void BoardPainter::resizeEvent(QResizeEvent *event)
{
    QGraphicsView::resizeEvent(event);

    // adjust scale to fit board in view
    QRectF r = sceneRect();
    qreal sy = (r.height() + 2.0*m_margin) / height(),
          sx = (r.width()  + 2.0*m_margin) / width(),
          sm = 1.0 / std::max(sx,sy);

    {   QTransform t;
        t.scale(sm,sm);
        setTransform(t);
    }
    // center everything
    ensureVisible(sceneRect(), m_margin, m_margin);

    // scale background brush
    QTransform t;
    t.scale(1, sceneRect().height()/2);
    QBrush b = backgroundBrush();
    b.setTransform(t);
    setBackgroundBrush(b);
}

void BoardPainter::paintEvent(QPaintEvent * e)
{
    QGraphicsView::paintEvent(e);

    // start messure time
    if (m_start_anim)
    {
        SB_PAINTER_DEBUG("starting animations");
        m_anim_time.start();
        m_anim_timer.start();
        m_start_anim = false;
    }
}


void BoardPainter::setBoard(const Board& board, const Move * move, Square ignore_to)
{
    SB_PAINTER_DEBUG("BoardPainter::setBoard(board," << (move? "anim" : "-") << ", " << ignore_to << ")");

    // keep side to turn
    m_is_white = board.toMove() == White;

    /** @todo Right now, the QGraphicsItems are recreated for each ply.
        It would probably be more cpu friendly to update only what's needed.
        */
    createBoard_(board);
    createPieces_(board);
    updateMoveIndicators_();

    if (m_animations)
    {
        stopAnimation_();
        emit animationFinished(oldBoard_);
    }

    // add animations to que (and count m_animations)
    if (m_do_animate && m_anim_speed > 0.0
        && move)
        guessAnimations_(board, *move, ignore_to);

    // start animation timer on next paintEvent
    m_start_anim = m_animations > 0;

    oldBoard_ = board;

    // ALWAYS EMIT THIS
    // PlayGameWidget needs a feedback
    if (m_animations <= 0)
        emit animationFinished(board);
    else
    {
        SB_PAINTER_DEBUG("qued" << m_animations << "animations");
    }
}


void BoardPainter::guessAnimations_(const Board& b, const Move& move, Square ignore_to)
{
    /* PieceItems are already placed at the new positions on entry. */

    int from = BN[move.from()],
        to = BN[move.to()];

    // move animation
    if (from != InvalidSquare && to != InvalidSquare
        && to != ignore_to
        // don't animate if did the same last time
        // XXX not perfect for gamebrowsing (now checked in MainWindow::slotMoveChanged())
        //oldBoard_.pieceAt(to) != b.pieceAt(to)
            )
            addMoveAnimation_(from, to);

    for (int i=fsq; i<=lsq; ++i)
    {
        Piece pold = oldBoard_.pieceAt(i),
              pnew = b.pieceAt(i);

        // nothing's changed on that square?
        if (pold == pnew)
            continue;

        // became defunkt
        if (isDefunkt(pnew) && pieceType(pold) != None
            // only animate the last captured piece
            && BN[move.capturedAt()] == i
            // don't animate when user dragged
            && to != ignore_to)
        {
            SB_PAINTER_DEBUG("turned defunkt " << i);

            PieceItem * pinew = pieceItemAt(i);
            addPixmapAnimation_(pinew,
                // get previous pixmap (of real piece)
                m_theme->piece(pold,
                             (isFlipped() && pold == BlackBatyr)
                          || (!isFlipped() && pold == WhiteBatyr) ) );
            continue;
        }

        // disappeared
        if (pnew == Empty && pold != Empty)
        {
            // removed defunkt
            if (isDefunkt(pold))
            {
                SB_PAINTER_DEBUG("removed defunkt" << i);
                addRemoveAnimation_(b, i, pold);
                continue;
            }

            // captured?
            int cap = BN[move.capturedAt()];
            if (cap == i)
            {
                SB_PAINTER_DEBUG("capturedAt" << cap);
                addRemoveAnimation_(b, i, pold);
                continue;
            }
        }
    }
}


void BoardPainter::createBoard_(const Board& board)
{
    // delete previous
    for (size_t i=0; i<m_squares.size(); ++i)
    {
        m_scene->removeItem(m_squares[i]);
        delete m_squares[i];
    }
    m_squares.clear();

    // set standard brushes/pens and stuff
    m_deco.highlightBrush[0] = QBrush(m_deco.hoverColor);
    m_deco.highlightBrush[1] = QBrush(m_deco.hoverColor);
    m_deco.highlightPen[0] = QPen(m_deco.hoverColor);
    m_deco.highlightPen[0].setWidth(m_size / 10);
    m_deco.highlightPen[0].setCapStyle(Qt::RoundCap);
    m_deco.highlightPen[1] = m_deco.highlightPen[0];
    m_deco.selectPen = QPen(m_deco.selectColor);
    m_deco.selectPen.setWidth(m_size / 10);
    m_deco.selectPen.setCapStyle(Qt::RoundCap);
    m_deco.framePen.setColor( m_theme->color(BoardTheme::Frame) );
    m_deco.framePen.setWidth( m_frame_width * m_theme->size().width() / 100);
    m_deco.blackTemdekPen = QPen(Qt::black);
    m_deco.blackTemdekPen.setCapStyle(Qt::RoundCap);
    m_deco.blackTemdekPen.setWidthF(m_size/10);
    m_deco.whiteTemdekPen = m_deco.blackTemdekPen;
    m_deco.whiteTemdekPen.setColor(Qt::white);
    m_deco.fontPen = QPen(QColor(178,178,178,150));
    m_deco.font.setPixelSize(m_size/2);
    // XXX m_deco.font.setFamily(?);

    // create board squares
    for (Square i=fsq; i<=lsq; ++i)
    {
        const int x = gBoard[i][0],
                  y = gBoard[i][1];

        // select tile texture
        const QPixmap& pm = m_theme->square(!((x+y)&1));

        // setup tile
        SquareItem * s = new SquareItem(i, pm, m_deco);
        s->setPos(squarePos(i));
        s->setZValue(-1); // always behind pieces

        // set frame
        if (m_do_show_frame)
        {
            s->frame = true;
        }

        // set temdek flag
        if ((i == gateAt[Black] && board.temdekOn(Black)) ||
            (i == gateAt[White] && board.temdekOn(White)))
        {
            s->temdek = (i == gateAt[White])? 2 : 1;
        }

        // set tower square overlay
        if (m_do_tower && board.isTower(i))
        {
            s->overlay = &m_theme->towerOverlay();
        }

        // hint for enPassant
        if (board.enPassantSquare() == i
                && board.epPossible(i, board.toMove()))
        {
            s->overlay = &m_theme->enPassantOverlay();
        }

        // number display
        if (m_do_square_numbers)
        {
            s->numberStr = QString::number(BN[NB[i]]);
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
    {
        m_scene->removeItem(m_pieces[i]);
        delete m_pieces[i];
    }
    m_pieces.clear();

    m_org_drag_piece = 0;
    m_drag_piece = 0;

    // create pieces
    for (Square i=fsq; i<=lsq; ++i)
    {
        Piece p = board.pieceAt(i);
        if (p == InvalidPiece) continue;

        createPiece_(board, i, p);
    }
}

PieceItem * BoardPainter::createPiece_(const Board& board, Square sq, Piece p)
{
    // pixmap for piece
    const QPixmap& pm = m_theme->piece(p,
                (isFlipped() && p == BlackBatyr)
            || (!isFlipped() && p == WhiteBatyr) );

    PieceItem * item = new PieceItem(p, sq, pm);
    item->setPos(squarePos(sq));

    // set urgent flag
    if (board.isUrgent(sq))
    {
        item->overlay = &m_theme->urgent();
    }

    // add to scene
    m_scene->addItem(item);
    m_pieces.push_back(item);

    return item;
}

void BoardPainter::updateMoveIndicators_()
{
    // create the GraphicItems
    if (!m_move_white)
    {
        m_move_white = new QGraphicsRectItem;
        m_move_white->setRect(0,0, m_size/2, m_size/2);
        m_move_white->setPen(QPen(Qt::gray));
        m_move_white->setBrush(QBrush(Qt::white));
        m_scene->addItem(m_move_white);
    }
    if (!m_move_black)
    {
        m_move_black = new QGraphicsRectItem;
        m_move_black->setRect(0,0, m_size/2, m_size/2);
        m_move_black->setPen(QPen(Qt::gray));
        m_move_black->setBrush(QBrush(Qt::black));
        m_scene->addItem(m_move_black);
    }

    m_move_white->setVisible(m_do_show_side && m_is_white);
    m_move_black->setVisible(m_do_show_side && !m_is_white);
    if (!m_do_show_side) return;

    // set positions on view (also update from boardflip)
    if (!isFlipped())
    {
        m_move_black->setPos(squarePos(11).x(), squarePos(60).y()
                             + m_size - m_move_white->rect().height());
        m_move_white->setPos(squarePos(11).x(), squarePos(1).y());
    }
    else
    {
        m_move_black->setPos(squarePos(17).x(), squarePos(60).y());
        m_move_white->setPos(squarePos(17).x(), squarePos(1).y()
                             + m_size - m_move_white->rect().height());
    }
}

void BoardPainter::onFlip_()
{
    for (size_t i=0; i<m_squares.size(); ++i)
    {
        m_squares[i]->setPos(squarePos(m_squares[i]->square));
    }
    for (size_t i=0; i<m_pieces.size(); ++i)
    {
        m_pieces[i]->setPos(squarePos(m_pieces[i]->square));
        // update batyr graphic
        if (m_pieces[i]->piece == WhiteBatyr)
            m_pieces[i]->setPixmap(m_theme->piece(WhiteBatyr, !isFlipped()));
        if (m_pieces[i]->piece == BlackBatyr)
            m_pieces[i]->setPixmap(m_theme->piece(BlackBatyr, isFlipped()));
    }

    updateMoveIndicators_();
}

// -------------------- coords ---------------------------

QRectF BoardPainter::squareRect(Square sq) const
{
    const int x = isFlipped()?  8 - gBoard[sq][0] : gBoard[sq][0],
              y = isFlipped()? 13 - gBoard[sq][1] : gBoard[sq][1];

    return QRectF(
            (x-m_center.x())*m_size,
            (y-m_center.y())*m_size + m_do_moat *
                ((isFlipped()?(sq<32):(sq>31))*2-1) * 0.05*m_size,
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

    // XXX this should account for the ditch as well!

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

    Square sq = !isFlipped() ?
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

void BoardPainter::addHighlight(Square sq, int highlights)
{
    SquareItem * s = squareItemAt(sq);
    if (!s) return;

    if ((s->highlights & highlights) == highlights) return;

    s->highlights |= highlights;
    s->update();
}

void BoardPainter::clearHighlight(Square sq, int highlights)
{
    SquareItem * s = squareItemAt(sq);
    if (!s) return;

    if (!(s->highlights & highlights)) return;

    s->highlights &= ~highlights;
    s->update();
}

void BoardPainter::clearHighlights(int highlights)
{
    int inv = ~highlights;
    for (size_t i=0; i<m_squares.size(); ++i)
    {
        SquareItem * s = m_squares[i];

        if (s->highlights & highlights)
        {
            s->highlights &= inv;
            s->update();
        }
    }
}



void BoardPainter::setDragPiece(Square sq, Piece piece, const QPoint& view, bool visible)
{
    bool remove = (sq == InvalidSquare || piece == InvalidPiece);

    // delete previous m_drag_piece
    if (remove || (m_drag_piece && m_drag_piece->piece != piece))
    {
        delete m_drag_piece;
        m_drag_piece = 0;
    }

    // simply undo drag action
    if (remove)
    {
        if (m_org_drag_piece)
            m_org_drag_piece->setVisible(true);
        m_org_drag_piece = 0;
        return;
    }

    // find PieceItem to drag
    PieceItem * it = pieceItemAt(sq);
    if (!it) return;

    // remove original piece to be dragged
    it->setVisible(visible);
    m_org_drag_piece = it;

    // get position
    QPointF pos = mapToScene(view) - QPointF(m_size>>1,m_size>>1);

    // keep in range of board
    pos.setX(std::max(-3.5 * m_size,std::min(2.5 * m_size, pos.x())));
    pos.setY(std::max(-7.0 * m_size,std::min(6.0 * m_size, pos.y())));

    // create or refresh
    if (!m_drag_piece)
    {
        m_drag_piece = new PieceItem(piece, sq, m_theme->piece(piece));
        m_drag_piece->setPos(pos);
        m_scene->addItem(m_drag_piece);
        m_drag_piece->setZValue(1); // in front
    }
    else
        m_drag_piece->setPos(pos);
}


// --------------------- animation -------------------------

int BoardPainter::animationLength_(Square from, Square to) const
{
    // fixed?
    if (from == InvalidSquare || to == InvalidSquare)
    {
        from = 1;
        to = 3;
    }
        //return m_fixed_anim_length;

    // or distance of move

    qreal dx = gBoard[from][0] - gBoard[to][0],
          dy = gBoard[from][1] - gBoard[to][1],
          dist = sqrt(dx*dx + dy*dy),
    // animation length in terms of speed
          length_from_speed = dist / std::max(0.1, m_anim_speed),
    // crossfade between speed & fixed length
          t = std::max(0.0,std::min(1.0, m_use_fixed_anim_length ));
    // resulting animation length
    return 1000 * (length_from_speed * (1.0-t) + t * (m_fixed_anim_length));
}

void BoardPainter::addMoveAnimation_(Square from, Square to)
{
    SB_PAINTER_DEBUG("BoardPainter::addMoveAnimation_("<<from<<","<<to<<")");

    PieceItem * p = pieceItemAt(to);
    if (!p)
    {
        SB_PAINTER_DEBUG("illegal animation: no piece at " << to);
        return; // or throw a logic exception ;)
    }

    // setup the piece to animate
    p->animate = true;
    p->anim_length = animationLength_(from, to);
    m_animations++;

    p->square = from; // put back to start
    p->setPos(squarePos(p->square));
    p->squareTo = to; // say it's a move animation
}

void BoardPainter::addRemoveAnimation_(const Board& board, Square s, Piece p)
{
    SB_PAINTER_DEBUG("BoardPainter::addRemoveAnimation_("<<s<<","<<p<<")");

    PieceItem * pi = pieceItemAt(s);
    //if (!pi)
        pi = createPiece_(board, s, p);

    pi->animate = true;
    pi->anim_length = animationLength_();
    m_animations++;

    pi->animateRemove = true;

}

void BoardPainter::addPixmapAnimation_(PieceItem *pi, const QPixmap &oldpix)
{
    SB_PAINTER_DEBUG("BoardPainter::addPixmapAnimation_("<<pi->square<<")");

    pi->animate = true;
    pi->anim_length = animationLength_();
    m_animations++;

    // animate to current pixmap
    pi->animatePixmap = new QPixmap(pi->pixmap());
    // oldpix for start
    pi->setPixmap( oldpix );
}

void BoardPainter::stopAnimation_()
{
    m_anim_timer.stop();
    m_animations = 0;
    m_start_anim = false;

    // set all PieceItems to final position
    // (to be sure)
    for (size_t i=0; i<m_pieces.size(); ++i)
    if (m_pieces[i]->animate && m_pieces[i]->anim_length>0)
    {
        PieceItem * p = m_pieces[i];

        endPieceAnimation_(p);
    }
}

void BoardPainter::endPieceAnimation_(PieceItem * p)
{
    p->animate = false;

    // pixmap change
    if (p->animatePixmap)
    {
        p->setPixmap(*p->animatePixmap);
    }

    // remove animation
    else
    if (p->animateRemove)
    {
        p->setVisible(false);
    }

    // a move animation
    else
    if (p->squareTo != InvalidSquare)
    {
        // set final position
        p->square = p->squareTo;
        p->setPos(squarePos(p->square));
        p->setZValue(0);
    }
}


void BoardPainter::animationStep_()
{
    if (m_animations <= 0)
        return;

    int e = m_anim_time.elapsed();

    // go through all pieces that need to be animated
    // XXX This is the generalized approach to move
    // more than one piece at a time (but why?)
    for (size_t i=0; i<m_pieces.size(); ++i)
    if (m_pieces[i]->animate && m_pieces[i]->anim_length>0)
    {
        PieceItem * p = m_pieces[i];

        SB_PAINTER_DEBUG("animating " << p->square << " " << e << "/" << p->anim_length);

        // current animation position [0,1]
        qreal t = (qreal)e / p->anim_length;
        // sigmoid fade [0,1]
        qreal ts = (3.0 - 2.0*t) * t*t;

        // stop if anim time is over
        if (t>1)
        {
            endPieceAnimation_(p);
            m_animations--;
            continue;
        }

        // pixmap animation
        if (p->animatePixmap)
        {
            if (t<0.5) continue;
            // simply switch and end animation
            p->setPixmap( *p->animatePixmap );
            endPieceAnimation_(p);
            m_animations--;
            continue;
        }

        // a remove animation
        if (p->animateRemove)
        {
            p->setScale(1.0 - t);
            continue;
        }

        // a move animation
        if (p->squareTo != InvalidSquare)
        {
            QPointF
                from = squarePos(p->square),
                to = squarePos(p->squareTo);

            from += ts * (to - from);

            p->setPos(from);
            p->setZValue(10); // always on top
            continue;
        }

    }

    update();

    // done all animations?
    if (m_animations <= 0)
    {
        stopAnimation_();
        emit animationFinished(oldBoard_);
    }
}

