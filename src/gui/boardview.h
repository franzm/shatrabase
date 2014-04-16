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

#ifndef __BOARDVIEW_H__
#define __BOARDVIEW_H__

#include "common.h"
#include "board.h"
#include "boardtheme.h"

#include <QWidget>

class BoardTheme;
class BoardPainter;
class QLayout;

/** the BoardView class represents a widget for displaying current
position on the screen. */

class BoardView : public QWidget
{
    Q_OBJECT
public:
    enum { WheelUp = Qt::LeftButton, WheelDown = Qt::RightButton };
    enum BoardFlags
    {   /** Don't check for sanity of moves. */
        F_AllowAllMoves = 1,
        /** Allow copy-drag with shift */
        F_AllowCopyPiece = 2,
        /** Do not show move highlights, regardless of properties. */
        F_HideMoveHelp = 4,
        /** Do not execute moves on click, regardless of properties */
        F_NoExecuteMoves = 8,
        /** Don't allow white to move */
        F_DisableWhite = 16,
        /** Don't allow black to move */
        F_DisableBlack = 32
    };

    /** Create board widget.
        The parent is expected to exist!
        BoardView can be externalized, and will reconnect to parent on close.
        For @p flags see BoardFlags enum */
    BoardView(QWidget* parent, int flags = 0);

    /** Destroy widget. */
    ~BoardView();

    /** Set flags for board interaction. @see BoardFlags enum */
    void setFlags(int flags);

    /* Update and shows current position.
        @p to and @p from trigger an animation of the piece. */
    //void setBoard(const Board& value, int from = InvalidSquare, int to = InvalidSquare);
    /** Updates the board with animation */
    void setBoard(const Board& board, const Move & move);
    /** Updates the board without animation */
    void setBoard(const Board& board) { setBoard(board, Move()); }

    /** @return displayed position. */
    Board board() const;
    /** @return current theme */
    const BoardTheme& theme() const;
    /** Flips/unflips board. */
    void setFlipped(bool flipped);
    /** @return @p true if board is displayed upside down. */
    bool isFlipped() const;
    /** Make it almost 1.555 : 1 */
    virtual int heightForWidth(int width) const;
    /** Is in external window? */
    bool isExternal() const { return m_isExternal; }
    /** Makes the board an external window, or jumps back to the
        parent given in the creator. */
    void setExternal(bool);

public slots:
    /** Apply the settings for the board. */
    void configure();

    /** Tell the BoardView the best current move, e.g. from analysis. */
    void setBestMove(int from = InvalidSquare, int to = InvalidSquare);
    /** Executes current set best move. Returns true when there was one, false otherwise. */
    bool execBestMove();

protected slots:
    /** Weiterleitung from BoardPainter */
    void slotDisplayMessage(const QString& msg) { emit displayMessage(msg); }
    /** Weiterleitung from BoardPainter */
    void slotAnimationFinished(const Board& b) { emit animationFinished(b); }

signals:
    /** User clicked source and destination squares */
    void moveMade(Square from, Square to, int button);
    /** User dragged and dropped a piece holding Control */
    void copyPiece(Square from, Square to);
    /** User attempted an out of board move, used for position setup */
    void invalidMove(Square from);
    /** User clicked square */
    void clicked(Square square, int button, QPoint pos);
    /** User moved mouse wheel. */
    void wheelScrolled(int dir);
    /** Indicate that a piece was dropped to the board */
    void pieceDropped(Square to, Piece p);
    /** Board came back from external window. */
    void externalClosed();
    /** Signal to display a message, e.g. on hover. */
    void signalDisplayMessage(const QString& msg);

    /** A piece move animation has been finished */
    void animationFinished(const Board& b);

protected:
    /** Calls signalDisplayMessage and keeps a flag that
     *  a message has been sent. The message will be reset
     *  on-leave. */
    void displayMessage(const QString& msg);

    // -------------- events ------------------

    /** used to remove a previously send message */
    virtual void leaveEvent(QEvent *);

    /** Handle mouse events */
    virtual void mousePressEvent(QMouseEvent* e);
    /** Handle mouse events */
    virtual void mouseMoveEvent(QMouseEvent* e);
    /** Handle mouse events */
    virtual void mouseReleaseEvent(QMouseEvent* e);
    /* Handle mouse wheel events */
    //virtual void wheelEvent(QWheelEvent* e);

    virtual void keyPressEvent(QKeyEvent *);

    virtual void closeEvent(QCloseEvent *);

    //Drag'n'Drop Support
    virtual void dragEnterEvent(QDragEnterEvent *event);
    virtual void dragMoveEvent(QDragMoveEvent *event);
    virtual void dragLeaveEvent(QDragLeaveEvent *event);
    virtual void dropEvent(QDropEvent *event);

private:
    /** @return square at given position */
    Square squareAt(const QPoint& p) const;
    /** Check if piece at square @p square can be dragged */
    bool canDrag(Square s);
    //bool eventFilter(QObject *obj, QEvent *ev);

    /** Returns a nice string for each square */
    QString squareToString_(Square s) const;

    // ------ highlighting ------

    /* Selects given square. Previously selected square is unselected automatically.
       Clear hightlight with s = InvalidSquare. */
    //void selectSquare_(Square s = InvalidSquare);

    /** Sets m_selectedSquare + cares for all highlights. */
    void selectSquare_(Square s = InvalidSquare);

    /** Sets the m_hoverSquare to @p s. Previously selected square is unselected.
       Clear hightlights with s = InvalidSquare */
    void setHoverSquare_(Square s = InvalidSquare);
    /** Sets highlights for all possible goal squares for @p s.
        Previous highlights are cleared.
        @p goal_index controls the selected target square (m_goal_index).
        if -1, it will be reset to 0,
        if 0, stays unchanged,
        if 1, selects next */
    void showGoals_(Square s = InvalidSquare, int goal_index = -1);

#if(0)
    void drawSquares(QPaintEvent* event);
    void drawPieces(QPaintEvent* event);
    void drawMoveIndicator(QPaintEvent* event);
    void drawDraggedPieces(QPaintEvent* event);

    void drawSquareAnnotations(QPaintEvent* event);
    void drawSquareAnnotation(QPaintEvent* event, QString annotation);
    void drawArrowAnnotations(QPaintEvent* event);
    void drawArrowAnnotation(QPaintEvent* event, QString annotation);
#endif
    /** original parent (when not externalized) */
    QWidget * m_parent;
    /** current position */
    Board m_board;
    /** current move */
    Move m_move;
    /** all current moves (as Square) */
    std::vector<SquareMove> m_moves;

    /** graphics container */
    BoardTheme m_theme;
    /** the painter class */
    BoardPainter * m_view;
    /** layout to auto-size painter class. */
    QLayout * m_layout;

    // ---------- settings -----------

    /** ORed flags of F_... */
    int m_flags;
    /** flag if this board is an external window */
    bool m_isExternal,
    /** highlight a plausible move (on drag) */
        m_showCurrentMove,
    /** highlight all plausible moves (on hover) */
        m_showAllMoves,
    /** execute move on single click/release */
        m_guessMove,
    /** skip guessed move on right-click */
        m_guessNextMove;

    // --------- interaction ---------

    /** Square currently selected (clicked) */
    int m_selectedSquare;
    /** square currently under mouse, or InvalidSquare */
    int m_hoverSquare;
    /** from-Square of best move (setBestMove()), or InvalidSquare. */
    int m_bestMoveFrom,
        m_bestMoveTo;
    /** index into m_goals[] */
    unsigned int m_goal_index;
    /* trigger for animation from own action */
    //int m_own_from, m_own_to;
    //int m_currentFrom;
    //int m_currentTo;
    /** piece currently dragged, or InvalidPiece */
    Piece m_dragged;
    /** start square of drag */
    Square m_dragStartSquare;
    /** mouse start of drag (window space) */
    QPoint m_dragStart;
    /** current end of drag (window space) */
    QPoint m_dragPoint;
    /** Last dragged-to square to turn off move animation */
    Square m_lastDropped;
    /** flag for deleting the statusbar on leaveEvent() */
    bool m_messageSend;

    //int m_button;
    //bool m_clickUsed;
    //int m_wheelCurrentDelta;
    //int m_minDeltaWheel;
    //unsigned int m_moveListCurrent;
};

class BoardViewMimeData : public QMimeData
{
    Q_OBJECT

public:
    Piece m_piece;
};

#endif

