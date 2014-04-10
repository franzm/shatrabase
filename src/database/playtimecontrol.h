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

#ifndef PLAYTIMECONTROL_H
#define PLAYTIMECONTROL_H

#include <QTime>
#include <QTimer>

#include "timecontrol.h"

/** TimeControl with added realtime game functionallity */
class PlayTimeControl : public TimeControl
{
    Q_OBJECT
public:
    explicit PlayTimeControl(QObject *parent = 0);

    // ---- state getter ----

    /** Return current side to move */
    int toMove() const { return stm_; }

    bool isMoving() const { return moving_; }

    /** Is current stm out of time? */
    bool isTimeout() const;

    /** Current move number.
     *  Starts at 1, increments when stm == startStm in endMove() */
    int getMove() const { return move_; }

    /** Returns total time (msec) left for either player.
     *  Might be TimeControl::Unlimited */
    int getTotalTime(int stm) const { return totalTime_[stm]; }

    /** Returns last move's time (msec) for either player. */
    int getMoveTime(int stm) const { return moveTime_[stm]; }

signals:

    /** Send when the side to move has timed out */
    void timeOut(int stm);

    /** Send every second during move,
     *  getTotalTime() and getMoveTime() will give the actual timings. */
    void timeUpdated();

public slots:

    /** Inititalize for a new game */
    void start(int stm_0_or_1 = 0);

    /** Start counting the current stm */
    void startMove();

    /** The current stm has finished it's move.
     *  Manages totalTime, stm and move count.
     *  Also returns the move-time in millisecs.
     *  timeOut() might be emitted. */
    int endMove();

private slots:

    /** callback from timeout timer */
    void slotTimer_();

private:

    /** Move starts at 1, incremented on startStm_'s turn */
    int move_,
    /** First side to move */
        startStm_,
    /** Current side to move */
        stm_,
    /** Total time left */
        totalTime_[2],
    /** current move time */
        moveTime_[2];

    bool moving_,
         didSendTimeOut_;

    QTime messure_;
    QTimer timer_;

    int taken_;
};

#endif // PLAYTIMECONTROL_H
