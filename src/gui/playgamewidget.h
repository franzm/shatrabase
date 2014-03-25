/***************************************************************************

Copyright (C) 2014  stefan.berke @ modular-audio-graphics.com

This source is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either
version 3.0 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License
along with this software; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA

****************************************************************************/

#ifndef PLAYERSELECT_H
#define PLAYERSELECT_H

#include <QWidget>
#include <QList>

#include "board.h"

class Board;
class PlayGame;

namespace Ui { class PlayGame; }

class PlayGameWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PlayGameWidget(QWidget *parent = 0);
    ~PlayGameWidget();

    bool isPlaying() const { return playing_; }

    /** Returns if White should be able to interact with board */
    bool whiteCanMove() const;
    /** Returns if Black should be able to interact with board */
    bool blackCanMove() const;

signals:
    /** Emitted when a new game wants to be played */
    void startNewGame();
    /** Emitted when player wants to resign. */
    void resignGame();

    /** The Engine has send a new position. */
    void moveMade(Move);

public slots:
    void slotReconfigure();

    /** Sets new position. Signals that a move has been performed.
        If required, the Engine will be queried. */
    void setPosition(const Board& board);

    /** Signal that board has done the last issued move */
    void animationFinished();

private slots:

    // -- engine callbacks --

    void enginesReady();
    void engineClueless();
    void moveFromEngine(Move);

    // -- widget callbacks --

    void slotName1Changed_(const QString&);
    void slotName2Changed_(const QString&);

    void slotEngine1Changed_(const QString&);
    void slotEngine2Changed_(const QString&);

    /** Starts new game */
    void start_();
    /** Flip the players */
    void flipPlayers_();
    /** haveit */
    void resign_();

private:

    /** Updates widgets */
    void setWidgetsPlayer_(int stm);
    /** Updates widgets */
    void setWidgetsPlaying_(bool);

    Ui::PlayGame * ui_;

    PlayGame * play_;

    Move lastMoveSend_;
    QList<Move> plyQue_;

    bool playing_,
    /** Flag used when first player is Engine */
         sendFreshBoardWhenReady_;
};

#endif // PLAYERSELECT_H
