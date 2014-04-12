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
#include <QTime>
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

    /** Collection of Pens and Brushes,
        initialized by configure() */
    struct Decoration
    {
        QColor hoverColor,
               selectColor;
        /** Pen for frame around squares */
        QPen framePen;

        /** 0=H_HOVER, 1=H_GOAL */
        QBrush highlightBrush[2];
        QPen highlightPen[2];
        QPen selectPen;

        QPen blackTemdekPen,
             whiteTemdekPen;
        /** For numbers on squares */
        QFont font;
        QPen fontPen;
    };

    // --- ctor ---

    explicit BoardPainter(BoardTheme * theme, QWidget *parent = 0);

    // ---------- config --------------

    /** Loads system settings */
    void configure();

    /** Return current brushes and stuff */
    const Decoration& decoration();

    // ----------- coords -------------

    /** Maps the view coordinates to board coords */
    QPoint mapToBoard(const QPoint& view_coords) const;

    Square squareAt(const QPoint& view_coords) const;

    bool isFlipped() const { return m_flipped; }
    void setFlipped(bool flipped) { m_flipped = flipped; onFlip_(); }

    bool isAnimating() const { return m_animations > 0; }

    // --------- board/pieces --------

    /** Updates/creates the board pieces, and performs the animation if
        from and to != InvalidSquare.
        Currently this clears all hightlights.
        If @p from and @p to are set appropriately an animation will be
        triggered for that piece and the moveFinished() signal is emitted
        when the animation is done.
        */
    //void setBoard(const Board& board, int from = InvalidSquare, int to = InvalidSquare);

    /** Simply updates the board with or without animation.
        A move with goal square @p ignore_move_to will not be animated. */
    void setBoard(const Board& board, const Move * move = 0, Square ignore_move_to = 0);

    // --------- indicators ---------

    /** Adds a OR combination of highlights to the particular square */
    void addHighlight(Square sq, int highlights);
    /** Clears all given OR-combined highlights for the particular square */
    void clearHighlight(Square sq, int highlights = AllHighlights);
    /** Clears all given OR-combined highlights for all squares. */
    void clearHighlights(int highlights = AllHighlights);

    /** Sets the drag/move display.
        @p view is the current mouse coords for the piece.
        Set @p sq to InvalidSquare to stop dragging. */
    void setDragPiece(Square sq = InvalidSquare, Piece piece = InvalidPiece,
                      const QPoint& view = QPoint(), bool previous_visible = false);
signals:

    /** Emitted when a move animation has ended. */
    void animationFinished();

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
    virtual void paintEvent(QPaintEvent *event);

    /* XXX The view is eating all mouse events, why? */
    virtual void mousePressEvent(QMouseEvent * e) { e->ignore(); };
    virtual void mouseMoveEvent(QMouseEvent * e) { e->ignore(); };
    virtual void mouseReleaseEvent(QMouseEvent * e) { e->ignore(); };
    virtual void mouseDoubleClickEvent(QMouseEvent * e) { e->ignore(); }

    virtual void dragEnterEvent(QDragEnterEvent * e) { e->ignore(); }
    virtual void dragMoveEvent(QDragMoveEvent * e) { e->ignore(); }
    virtual void dragLeaveEvent(QDragLeaveEvent * e) { e->ignore(); }

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

    /** Compares new board with old board and sets up animations.
        A move with goal square @p ignore_move_to will not be animated. */
    void guessAnimations_(const Board& newBoard, const Move& move, Square ignore_move_to = 0);

    void createBoard_(const Board& board);
    void createPieces_(const Board& board);
    PieceItem * createPiece_(const Board& board, Square sq, Piece p);

    void updateMoveIndicators_();
    /** Recalculates item positions after a board flip. */
    void onFlip_();

    /** Returns msecs for an animation, optionally with distance influence */
    int animationLength_(Square from = InvalidSquare, Square to = InvalidSquare) const;

    /** Sets the PieceItem to final animation state and clears anim flags */
    void endPieceAnimation_(PieceItem * p);

    /** Ques a move animation.
        PieceItems at @p from and @p to are supposed to represent the FINAL position
        and must be set up already. */
    void addMoveAnimation_(Square from, Square to);
    /** Ques an animation for removing a piece.
        PieceItem of removed piece may exist already or will be created. */
    void addRemoveAnimation_(const Board& board, Square s, Piece p);
    /** Ques an animation from @p old_pixmap to current pixmap */
    void addPixmapAnimation_(PieceItem * piece, const QPixmap& old_pixmap);

    // ----------- member ------------

    /** margin of board in view */
    static const int m_margin = 50;

    BoardTheme * m_theme;

    QGraphicsScene * m_scene;

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
    // config
    /** Piece move animation speed in squares per second */
        m_anim_speed,
    /** Piece animation fixed length */
        m_fixed_anim_length,
    /** set ratio between speed and fixed length [0,1] */
        m_use_fixed_anim_length;
    // state
    /** number of animations currently running */
    int m_animations,
    /** maximum length of all animations in seconds */
        m_anim_length;
    /** first time after new board? */
    bool m_start_anim;
    /** messure screen update */
    QTime m_anim_time;
    QTimer m_anim_timer;

    Decoration m_deco;

    int m_own_from, m_own_to;

    /** used to automatically trigger animations */
    Board oldBoard_;
};

#endif // BOARDPAINTER_H
