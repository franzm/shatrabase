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

#include <algorithm>

#include <QDebug>

#include "playtimecontrol.h"

PlayTimeControl::PlayTimeControl(QObject *parent) :
    TimeControl     (parent),
    moving_         (false),
    didSendTimeOut_ (false)
{
    timer_.setSingleShot(true);
    connect(&timer_, SIGNAL(timeout()), SLOT(slotTimer_()));
}

bool PlayTimeControl::isTimeout() const
{
    return type() == T_Tournament
        && totalTime_[stm_] <= 0;
}


void PlayTimeControl::slotTimer_()
{
    // update running state
    totalTime_[stm_] -= timer_.interval();
    moveTime_[stm_] += timer_.interval();
    // remember how much is messure already
    taken_ += timer_.interval();

    emit timeUpdated();

    // timeout?
    if (isTimeout())
    {
        didSendTimeOut_ = true;
        emit timeOut(stm_);
        return;
    }

    // restart timer
    if (type() == T_Tournament)
        timer_.setInterval(std::min(1000, totalTime_[stm_]));
    else
        timer_.setInterval(1000);
    timer_.start();
}

void PlayTimeControl::start(int stm)
{
    // just init everything

    startStm_ = stm_ = stm;
    move_ = 1;

    totalTime_[0] = totalTime_[1] = totalTimeAtStart();
    moveTime_[0] = moveTime_[1] = 0;

    emit timeUpdated();
}

void PlayTimeControl::startMove()
{
    qDebug() << "StartMove(" << move_ << "): " << (stm_? "Black" : "White");

    if (moving_) return; //Q_ASSERT(!moving_);

    didSendTimeOut_ = false;
    moving_ = true;
    taken_ = 0;

    moveTime_[stm_] = 0;
    emit timeUpdated();

    // init timer to total time left but at most every second
    if (type() == T_Tournament)
        timer_.setInterval(std::min(1000, totalTime_[stm_]));
    else
    // simply update movetime, don't count total time
        timer_.setInterval(1000);

    timer_.start();
    messure_.start();
}

int PlayTimeControl::endMove()
{
    int e = moveTime_[stm_] = messure_.elapsed();

    qDebug() << "EndMove(" << move_ << "): " << (stm_? "Black" : "White") << e << "ms";

    if (!moving_) return e; //Q_ASSERT(moving_);

    timer_.stop();
    moving_ = false;

    // remove time of move
    totalTime_[stm_] -= (e - taken_);
    moveTime_[stm_] = e;
    taken_ = 0;

    emit timeUpdated();

    // check time-out
    // (should have been handled by timer_ though)
    if (isTimeout() && !didSendTimeOut_)
    {
        didSendTimeOut_ = true;
        emit timeOut(stm_);
        return e;
    }

    // add bonus time
    totalTime_[stm_] += getTimeInc(move_);

    // switch stm and inc move

    stm_ ^= 1;

    if (stm_ == startStm_)
        move_++;

    return e;
}

