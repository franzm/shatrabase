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
    enum {WheelUp = Qt::LeftButton, WheelDown = Qt::RightButton};
    enum {IgnoreSideToMove = 1, AllowCopyPiece = 4};
    /** Create board widget. */
    BoardView(QWidget* parent, int flags = 0);
    /** Destroy widget. */
    ~BoardView();
    /** Set flags for board. Flags include:
    * @p IgnoreSideToMove - allow dragging all pieces (useful for setting up a position)
    */
    void setFlags(int flags);
    /** Update and shows current position. */
    void setBoard(const Board& value, int from = InvalidSquare, int to = InvalidSquare);
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

public slots:
    /** Reconfigure current theme. */
    void configure();

signals:
    /** User clicked source and destination squares */
    void moveMade(Square from, Square to, int button);
    /** User dragged and dropped a piece holding Control */
    void copyPiece(Square from, Square to);
    /** User dragged and dropped a piece holding Control */
    void invalidMove(Square from);
    /** User clicked square */
    void clicked(Square square, int button, QPoint pos);
    /** User moved mouse wheel. */
    void wheelScrolled(int dir);
    /** Indicate that a piece was dropped to the board */
    void pieceDropped(Square to, Piece p);

protected:

    // -------------- events ------------------

    /** Handle mouse events */
    virtual void mousePressEvent(QMouseEvent* e);
    /** Handle mouse events */
    virtual void mouseMoveEvent(QMouseEvent* e);
    /** Handle mouse events */
    virtual void mouseReleaseEvent(QMouseEvent* e);
    /** Handle mouse wheel events */
    virtual void wheelEvent(QWheelEvent* e);

protected: //Drag'n'Drop Support
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dragLeaveEvent(QDragLeaveEvent *event);
    void dropEvent(QDropEvent *event);

private:
    /** @return square at given position */
    Square squareAt(const QPoint& p) const;
    /** Check if piece at square @p square can be dragged */
    bool canDrag(Square s);
    //bool eventFilter(QObject *obj, QEvent *ev);

    // ------ highlighting ------

    /** Selects given square. Previously selected square is unselected automatically.
       Clear hightlight with s = InvalidSquare. */
    void selectSquare(Square s = InvalidSquare);
    /** Unselects given square. */
    void unselectSquare();
    /** Sets the m_hoverSquare to @p s. Previously selected square is unselected.
       Clear hightlight with s = InvalidSquare */
    void setHoverSquare(Square s = InvalidSquare);

//    void showPossibleMoves(Square s);

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
    Board m_board;
    BoardTheme m_theme;
    BoardPainter * m_view;
    QLayout * m_layout;
    bool m_showCurrentMove;
    int m_selectedSquare;
    int m_hoverSquare;
    int m_currentFrom;
    int m_currentTo;
    int m_flags;
    /** piece currently dragged */
    Piece m_dragged;
    /** start of drag */
    Square m_dragStartSquare;
    /** start of drag (window space) */
    QPoint m_dragStart;
    /** current end of drag (window space) */
    QPoint m_dragPoint;
    int m_button;
    bool m_clickUsed;
    int m_wheelCurrentDelta;
    int m_minDeltaWheel;
    unsigned int m_moveListCurrent;
};

class BoardViewMimeData : public QMimeData
{
    Q_OBJECT

public:
    Piece m_piece;
};

#endif

