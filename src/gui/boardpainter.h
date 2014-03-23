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

    enum Highlight
    {
        H_HOVER =  0x01,
        H_SELECT = 0x02,
        H_GOAL  =  0x04,
        H_TARGET = 0x08
    };
    static const int maxHighlights = 2;
    static const int AllHighlights = H_HOVER | H_SELECT | H_GOAL | H_TARGET;


    // --- ctor ---

    explicit BoardPainter(BoardTheme * theme, QWidget *parent = 0);

    // ---------- config --------------

    /** Loads system settings */
    void configure();

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

    // --------- indicators ---------

    /** Adds a OR combination of highlights to the particular square */
    void addHighlight(Square sq, int highlights);
    /** Clears all given OR-combined highlights for the particular square */
    void clearHighlight(Square sq, int highlights = AllHighlights);
    /** Clears all given OR-combined highlights for all squares. */
    void clearHighlights(int highlights = AllHighlights);

    /* Shows a half-transparent rect on Square @p sq.
        Any previous color will be overwritten.
    void setSquareColor(Square sq, const QColor& color);
    void clearSquareColor(Square sq);
    void clearSquareColors();

    * Hilights all squares as reachable.
        Previous hilights are removed.
    void setReachableSquares(const std::vector<Square>& squares);
    void clearReachableSquares(); */

    /** Sets the drag/move display.
        @p view is the current mouse coords for the piece.
        Set @p sq to InvalidSquare to stop dragging. */
    void setDragPiece(Square sq = InvalidSquare, Piece piece = InvalidPiece,
                      const QPoint& view = QPoint());
signals:

    /** Emitted when a move animation has ended. */
    void moveFinished();
    /** Signal to display a message, e.g. on hover. */
    void displayMessage(const QString& msg);
protected slots:

    /** animation executer */
    void animationStep_();
    /** stops animation and sets pieces to final position. */
    void stopAnimation_();

    // ________ PROTECTED ____________
protected:
    virtual void resizeEvent(QResizeEvent *event);

    /* XXX The view is eating all mouse events, why? */
    virtual void mousePressEvent(QMouseEvent * e) { e->ignore(); };
    virtual void mouseMoveEvent(QMouseEvent * e) { e->ignore(); };
    virtual void mouseReleaseEvent(QMouseEvent * e) { e->ignore(); };
    virtual void mouseDoubleClickEvent(QMouseEvent * e) { e->ignore(); }

    // -------- coords ---------------

    /** rect of a square in scene coords */
    QRectF squareRect(Square sq) const;
    /** (corner) position of a square in scene coords */
    QPointF squarePos(Square sq) const { return squareRect(sq).topLeft(); }

    /** Returns the SquareItem for the position, or 0 */
    SquareItem * squareItemAt(Square sq) const;
    /** Returns the PieceItem for the position, or 0 */
    PieceItem * pieceItemAt(Square sq) const;

    // --------- internal ------------

    void createBoard_(const Board& board);
    void createPieces_(const Board& board);
    void updateMoveIndicators_();
    /** Recalculates item positions after a board flip. */
    void onFlip_();

    /** starts the animation thread and animates all
        flagged pieces. @p from and @p to are only
        used to derive the animation length */
    void startAnimation_(Square from, Square to);

    // ----------- member ------------

    /** margin of board in view */
    static const int m_margin = 50;

    BoardTheme * m_theme;

    QGraphicsScene * m_scene;

    QTimer m_timer;

    std::vector<SquareItem*> m_squares;
    std::vector<PieceItem*> m_pieces;
    PieceItem
    /** The piece on the board that is currently dragged.
        The dragged piece is in m_drag_piece. This one
        will remain where it is. */
        * m_org_drag_piece,
    /** currently dragged piece (copy of original) */
        * m_drag_piece;
    QGraphicsRectItem * m_move_white,
                      * m_move_black;
    /** center position (in squares) of board */
    QPointF m_center;
    /** size of a tile (BoardTheme's size of Pixmaps). */
    int m_size,
        m_frame_width;

    bool m_flipped,
        m_is_white,
        m_do_moat,
        m_do_square_numbers,
        m_do_tower,
        m_do_animate,
        m_do_show_side,
        m_do_show_frame;

    qreal
    /** set piece move animation speed in squares per second */
        m_anim_speed,
    /** set piece move animation length */
        m_fixed_anim_length,
    /** set ratio between speed and fixed length [0,1] */
        m_use_fixed_anim_length,
    /** length of animations in seconds (calculated from above settings) */
        m_anim_length,
    /** current animation from 0 to 1 */
        m_anim_t;

    QColor m_hoverColor,
           m_selectColor;

    int m_own_from, m_own_to;
};

#endif // BOARDPAINTER_H
