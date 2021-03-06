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

#ifndef __BOARDSETUPDIALOG_H__
#define __BOARDSETUPDIALOG_H__

#include "ui_boardsetup.h"
#include "board.h"
#include "common.h"

class QMenu;
class QAction;
class QActionGroup;
class BoardView;

/** The BoardSetupDialog class represents a dialog for setting up a position.  */

class BoardSetupDialog : public QDialog
{
	Q_OBJECT
public:
	/* Create dialog */
	BoardSetupDialog(QWidget* parent = 0);
	/* Destroy dialog */
	virtual ~BoardSetupDialog();
	/* Set current board */
    void setBoard(const SHATRA::Board& b);
	/* Set current board */
	void setFlipped(bool flipped);
	/* Return current board */
    SHATRA::Board board() const;

public slots:
	/* Execute dialog */
	int exec();
    /* Start a drag operation */
    void startDrag(QWidget* w, QMouseEvent* event);
    void labelClicked(SHATRA::Piece p);
signals:
    void signalClearBackground(SHATRA::Piece p);

private slots:
	/* If the position is legal, accept it and close the dialog. */
	void slotAccept();
	/* Clear board. */
	void slotClear();
	/* Restore standard position */
	void slotReset();
	/* Copy current position as SPN. */
	void slotCopySPN();
	/* Paste SPN from clipboard as current position. */
	void slotPasteSPN();
	/* Change current piece using wheel */
	void slotChangePiece(int dir);
    /* A Drop operation was inidcated by the boardview */
    void slotDroppedPiece(SHATRA::Square s, SHATRA::Piece p);
	/* Select square and insert piece */
    void slotSelected(SHATRA::Square s, int button);
	/* Manually adjust en passant square. */
    //void slotEnPassantSquare();
	/* Adjust move number. */
	void slotMoveNumber();
	/* Move piece */
    void slotMovePiece(SHATRA::Square from, SHATRA::Square to);
	/* Copy piece */
    void slotCopyPiece(SHATRA::Square from, SHATRA::Square to);
    /* Remove piece */
    void slotInvalidMove(SHATRA::Square from);
	/* Toggle side to move */
	void slotToggleSide();

    void slotSquareDefunkt();
    void slotSquareTemdek();
    void slotSquareEnPassant();
    void slotSquareUrgent();
    void slotSquareTransit();

private:
	Ui::BoardSetupDialog ui;

    SHATRA::Color m_toMove;
	/* Display side to move */
	void showSideToMove();
	/* Message for incorrect setups. Returns empty string if the position is correct. */
	QString boardStatusMessage() const;
	/* Sets status message for the board (either spn or error message). */
	void setStatusMessage();

    void openSquarePopup(SHATRA::Square s);

protected:
	/* Scroll current piece with a wheel */
	virtual void wheelEvent(QWheelEvent* e);
    int m_wheelCurrentDelta;
    int m_minDeltaWheel;
    SHATRA::Piece m_selectedPiece;
    BoardView * m_boardView;
    SHATRA::Board m_board;
    QMenu * m_popmenu;
    SHATRA::Square m_popsquare;
    // popup actions
    QAction * pa_defunkt,
            * pa_transit,
            * pa_temdek,
            * pa_enpassant,
            * pa_urgent;
};


#endif
