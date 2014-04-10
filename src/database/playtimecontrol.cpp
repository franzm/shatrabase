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

#include "playtimecontrol.h"

PlayTimeControl::PlayTimeControl(QObject *parent) :
    TimeControl(parent)
{
}

void PlayTimeControl::start(int stm)
{
    startStm_ = stm_ = stm;
    move_ = 1;

    totalTime_[0] = totalTime_[1] = totalTimeAtStart();
    moveTime_[0] = moveTime_[1] = 0;
}

void PlayTimeControl::startMove()
{
    messure_.start();
}

void PlayTimeControl::endMove()
{
    int e = messure_.elapsed();

    // remove time of move
    totalTime_[stm_] -= e;

    if (totalTime_[stm_] < 0)
    {
        emit timeOut(stm_);
        return;
    }

    // add bonus time
    totalTime_[stm_] += getTimeInc(move_);

    // switch stm and inc move

    stm_ ^= 1;

    if (stm_ == startStm_)
        move_++;
}

/*
    40 in 100

*/

bool PlayTimeControl::isTimeout() const
{
    return totalTime_[stm_] <= 0;
}
