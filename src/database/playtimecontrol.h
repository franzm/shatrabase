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

#include "timecontrol.h"

/** TimeControl with added realtime game functionallity */
class PlayTimeControl : public TimeControl
{
    Q_OBJECT
public:
    explicit PlayTimeControl(QObject *parent = 0);

    /** Is current stm out of time? */
    bool isTimeout() const;

signals:

    /** The side to move has timed out */
    void timeOut(int stm);

public slots:

    /** Inititalize for a new game */
    void start(int stm_0_or_1 = 0);

    /** Start counting the current stm */
    void startMove();

    /** The current stm has finished it's move.
     *  Changes stm and move count */
    void endMove();


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

    QTime messure_;
};

#endif // PLAYTIMECONTROL_H
