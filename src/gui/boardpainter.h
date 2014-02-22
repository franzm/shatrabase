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

#ifndef BOARDPAINTER_H
#define BOARDPAINTER_H

#include <QGraphicsView>
#include <QPoint>
#include <QMouseEvent>
#include <QTimer>

#include "../database/common.h"
#include "board.h"

class QGraphicsScene;

class BoardTheme;
class PieceItem;
class SquareItem;

/** Basically a paint backend for BoardView which also
    translates between coordinates and squares/pieces. */
class BoardPainter : public QGraphicsView
{
    Q_OBJECT
public:
    explicit BoardPainter(BoardTheme * theme, QWidget *parent = 0);

    // ----------- coords -------------

    /** Maps the view coordinates to board coords */
    QPoint mapToBoard(const QPoint& view_coords) const;

    Square squareAt(const QPoint& view_coords) const;

    bool isFlipped() const { return m_flipped; }
    void setFlipped(bool flipped) { m_flipped = flipped; onFlip_(); }

    // --------- board/pieces --------

    /** Updates/creates the board pieces, and performs the animation if
        from and to != InvalidSquare.
        Currently this clears all hightlights.
        If @p from and @p to are set appropriately an animation will be
        triggered for that piece and the moveFinished() signal is emitted
        when the animation is done.
        */
    void setBoard(const Board& board, int from = InvalidSquare, int to = InvalidSquare);

    // --------- inidicators ---------

    /** Shows a half-transparent rect on Square @p sq.
        Any previous color will be overwritten. */
    void setSquareColor(Square sq, const QColor& color);
    void clearSquareColor(Square sq);
    void clearSquareColors();

    /** Starts drag/move animation.
        @p view is the mouse coords for the piece.
        Set @p sq to InvalidSquare to stop dragging. */
    void setDragPiece(Square sq = InvalidSquare, Piece piece = InvalidPiece,
                      const QPoint& view = QPoint());
signals:

    /** Emitted when a move animation has ended. */
    void moveFinished();

protected slots:

    /** animation executer */
    void animationStep_();

    // ________ PROTECTED ____________
protected:
    /* XXX The view is eating all mouse events, why? */
    virtual void mousePressEvent(QMouseEvent * e) { e->ignore(); };
    virtual void mouseMoveEvent(QMouseEvent * e) { e->ignore(); };
    virtual void mouseReleaseEvent(QMouseEvent * e) { e->ignore(); };
    virtual void mouseDoubleClickEvent(QMouseEvent * e) { e->ignore(); }

    // -------- coords ---------------

    /** rect of a square in scene coords */
    QRect squareRect(Square sq) const;
    /** (corner) position of a square in scene coords */
    QPoint squarePos(Square sq) const { return squareRect(sq).topLeft(); }

    /** Returns the SquareItem for the position, or 0 */
    SquareItem * squareItemAt(Square sq) const;
    /** Returns the PieceItem for the position, or 0 */
    PieceItem * pieceItemAt(Square sq) const;

    // --------- internal ------------

    void createBoard_(const Board& board);
    void createPieces_(const Board& board);
    /** Recalculates item positions after a board flip. */
    void onFlip_();

    /** starts the animation thread and animates all
        flagged pieces. @p from and @p to are only
        used to derive the animation length */
    void startAnimation_(Square from, Square to);
    /** stops animation and sets pieces to final position. */
    void stopAnimation_();

    // ----------- member ------------

    BoardTheme * m_theme;

    QGraphicsScene * m_scene;

    QTimer m_timer;

    std::vector<SquareItem*> m_squares;
    std::vector<PieceItem*> m_pieces;
    /** currently dragged piece */
    PieceItem * m_drag_piece;

    /** center position (in squares) of board */
    QPointF m_center;
    /** size of a tile (somewhat arbitrary since QGraphicsView scales anyway,
     *  but we ask BoardTheme for QPixMaps in this size). */
    int m_size;

    bool m_flipped;

    qreal
    /** piece move animation speed in squares per second */
        m_anim_speed,
    /** length of animations in seconds (calculated from speed) */
        m_anim_length,
    /** current animation from 0 to 1 */
        m_anim_t;
};

#endif // BOARDPAINTER_H
