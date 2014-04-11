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
#include "playtimecontrol.h"
#include "qled.h"

class Board;
class PlayGame;
class EngineDebugWidget;

namespace Ui { class PlayGame; }

/** More than a GUI wrapper around PlayGame class.
 *
 *  Actually, this class handles all the gui integration of gaming
 *  and the time control!.
 */
class PlayGameWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PlayGameWidget(EngineDebugWidget * debug, QWidget *parent = 0);
    ~PlayGameWidget();

    bool isPlaying() const { return playing_; }

    /** Returns if White should be able to interact with board */
    bool whiteCanMove() const;
    /** Returns if Black should be able to interact with board */
    bool blackCanMove() const;

signals:
    /** Emitted when a new game wants to be played */
    void startNewGame(const QMap<QString, QString>& tags);

    /** Emitted when the current game in Chessbrowser should be continued. */
    void continueGame();

    /** Emitted when player needs a break. */
    void pauseGame();

    /** The Engine has send a new position. */
    void moveMade(Move);

    void playerWins();
    void playerLoses();

public slots:

    /** Applies app settings */
    void slotReconfigure();

    /** Stops play mode. E.g. when save current game was cancelled. */
    void stop();

    /** Stops any connected Engines and discards their next moves.
        XXX Not possible to restart thinking right now. */
    void stopThinking();

    /** Sets new position. Signals that a move has been performed.
        If required, the Engine will be queried. */
    void setPosition(const Board& board);

    /** Signal that board has done the last issued move */
    void animationFinished(const Board& board);

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

    void slotConfig1Clicked_();
    void slotConfig2Clicked_();

    void slotBlinkTimer_();
    void slotUpdateClocks_();
    void slotTimeout_(int stm);

    /** Starts new game */
    void start_();
    /** Continues current game in ChessBrowser */
    void continue_();
    /** Flip the players */
    void flipPlayers_();
    /** haveit for now */
    void pause_();
    /** Player hit resign */
    void resign_();

private:

    void initTiming_();

    /** Updates widgets */
    void setWidgetsPlayer_(int stm);
    /** Updates widgets */
    void setWidgetsPlaying_(bool);
    /** Updates widgets according to who's an Engine */
    void updateEngineWidgets_();

    /** Checks Game result and talks with MainWindow.
        Returns if game ended. */
    bool checkGameResult_(const Board&, bool triggerWinSignals, bool doStop);

    // ---- config ---
    QLed::ledColor colorPlayer_;
    // for blinking when engine thinks
    QLed::ledColor colorEngine0_;
    QLed::ledColor colorEngine1_;
    /** Engine thinking blinking interval in ms */
    int blinkInterval_;

    // ___ MEMBER ___

    Ui::PlayGame * ui_;

    EngineDebugWidget * engineDebug_;

    QTimer blinkTimer_;
    int activeLed_;

    PlayGame * play_;

    Color lastStm_;
    QList<Move> plyQue_;

    bool playing_,
    /* Player is within a multi capture sequence */
        //playerMultiPly_,
    /** Ignore the next answer from Engine */
        ignoreAnswer_,
    /** Flag used when first player is Engine */
        sendFreshBoardWhenReady_;

    // ---- time control ----

    PlayTimeControl tc_;

    /** Move (incremented when White's turn) */
    int timeMove_,
        timeStm_,
        totalTime_[2],
        moveTime_[2];

};

#endif // PLAYERSELECT_H
