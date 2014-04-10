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

#include "timecontrol.h"
#include "settings.h"


const char * timeControlTypeName[] =
{
    "free",
    "match",
    "limit",
    "tournament"
};

TimeControl::TimeControl(QObject * p)
    :   QObject         (p),
        type_           (T_None),
        numMoves1_      (40),
        numMoves2_      (20),
        timeForMoves1_  (30*60*1000),
        timeForMoves2_  (30*60*1000),
        timeInc1_       (0),
        timeInc2_       (0),
        timeInc3_       (0),
        timeAdd_        (5*60*1000),
        timeLimit_      (Unlimited),
        nodeLimit_      (Unlimited),
        depthLimit_     (Unlimited)
{
}


void TimeControl::configure()
{
    // Time Control
    AppSettings->beginGroup("/TimeControl/");

    type_ = T_None;
    QString mode = AppSettings->getValue("mode").toString();
    for (int i=0; i<MaxTypes; ++i)
    if (mode == timeControlTypeName[i])
    {
        type_ = (Type)i;
        break;
    }

    numMoves1_ = AppSettings->getValue("numMoves1").toInt();
    numMoves2_ = AppSettings->getValue("numMoves2").toInt();
    if (AppSettings->getValue("allMoves1").toBool())
        numMoves1_ = Unlimited;
    if (AppSettings->getValue("allMoves2").toBool())
        numMoves2_ = Unlimited;

    timeForMoves1_ = AppSettings->getValue("timeForMoves1").toInt();
    timeForMoves2_ = AppSettings->getValue("timeForMoves2").toInt();
    timeAdd_ = AppSettings->getValue("timeAdd").toInt();
    timeInc1_ = AppSettings->getValue("timeInc1").toInt();
    timeInc2_ = AppSettings->getValue("timeInc2").toInt();
    timeInc3_ = AppSettings->getValue("timeInc3").toInt();
    if (!AppSettings->getValue("doTimeInc1").toBool())
        timeInc1_ = 0;
    if (!AppSettings->getValue("doTimeInc2").toBool())
        timeInc2_ = 0;
    if (!AppSettings->getValue("doTimeInc3").toBool())
        timeInc3_ = 0;
    if (!AppSettings->getValue("doTime2").toBool())
    {
        numMoves2_ = 0;
        timeForMoves2_ = 0;
        timeInc2_ = 0;
    }
    if (numMoves1_ == Unlimited || numMoves2_ == Unlimited)
        timeAdd_ = 0;

    timeLimit_ = AppSettings->getValue("timeLimit").toInt();
    nodeLimit_ = AppSettings->getValue("nodeLimit").toInt();
    depthLimit_ = AppSettings->getValue("depthLimit").toInt();
    if (!AppSettings->getValue("doTimeLimit").toBool())
        timeLimit_ = Unlimited;
    if (!AppSettings->getValue("doDepthLimit").toBool())
        nodeLimit_ = Unlimited;
    if (!AppSettings->getValue("doNodeLimit").toBool())
        depthLimit_ = Unlimited;

    AppSettings->endGroup();
}

QString TimeControl::msecToString(int msec) const
{
    if (msec <= 0)
        return tr("0 sec");

    const int h = msec/60/60/1000,
              m = (msec/60/1000) % 60,
              s = (msec/1000) % 60;

    QString r;
    if (h)
        r += tr("%1 h").arg(h);

    if (m)
    {
        if (!r.isEmpty())
            r += ", ";
        r += tr("%1 min").arg(m);
    }

    if (s)
    {
        if (!r.isEmpty())
            r += ", ";
        r += tr("%1 sec").arg(s);
    }

    if (msec % 1000)
    {
        if (!r.isEmpty())
            r += ", ";
        r += tr("%1 ms").arg(msec%1000);
    }

    return r;
}

QString TimeControl::humanReadable() const
{
    if (type_ == T_None)
    {
        return tr("Free play");
    }

    if (type_ == T_Match)
    {
        return tr("Match opponent's move time");
    }

    if (type_ == T_Limit)
    {
        QString s(tr("Limits: "));
        int c=0;
        if (timeLimit_ != Unlimited)
        {
            s += tr("time %1").arg(msecToString(timeLimit_));
            c++;
        }
        if (depthLimit_ != Unlimited)
        {
            if (c) s += ", ";
            s += tr("depth %1").arg(depthLimit_);
            c++;
        }
        if (nodeLimit_ != Unlimited)
        {
            if (c) s += ", ";
            s += tr("nodes %1").arg(nodeLimit_);
            c++;
        }
        if (c==0)
            s += "none";
        return s;
    }

    if (type_ == T_Tournament)
    {
        QString s;
        if (numMoves1_ == Unlimited)
            s += tr("Game in %1").arg(msecToString(timeForMoves1_));
        else
            s += tr("%1 moves in %2").arg(numMoves1_).arg(msecToString(timeForMoves1_));
        if (timeInc1_)
            s += tr(" + %1 per move").arg(msecToString(timeInc1_));
        if (numMoves1_ == Unlimited)
            return s;

        if (numMoves2_ != 0)
        {
            if (numMoves2_ == Unlimited)
                s += tr(", remaining game in %1").arg(msecToString(timeForMoves2_));
            else
                s += tr(", %1 moves in %2").arg(numMoves2_).arg(msecToString(timeForMoves2_));
            if (timeInc2_)
                s += tr(" + %1 per move").arg(msecToString(timeInc2_));
            if (numMoves2_ == Unlimited)
                return s;
        }

        s += tr(", remaining game in %1").arg(msecToString(timeAdd_));
        if (timeInc3_)
            s += tr(" + %1 per move").arg(msecToString(timeInc3_));

        return s;
    }

    return "*misconfigured*";
}


int TimeControl::totalTimeAtStart() const
{
    if (type_ != T_Tournament)
        return Unlimited;

    return timeForMoves1_;
}

/*
bool TimeControl::isTimeout(int move, int time) const
{
    // All in timeForMoves1
    if (numMoves1_ == Unlimited)
    {
        return (time >= timeForMoves1_);
    }

    move -= numMoves1_;

    // All in timeForMoves2
    if (numMoves2_ == Unlimited)
    {
        return (time >= timeForMoves2_);
    }
    else
    //
    if (numMoves2_ > 0)
    {
       time -= timeForMoves2_;
    }
}
*/

int TimeControl::getTimeInc(int move) const
{
    if (type_ != T_Tournament)
        return 0;

    // All in timeForMoves1
    if (numMoves1_ == Unlimited)
    {
        return timeInc1_;
    }

    // within tc1
    if (move < numMoves1_)
    {
        return timeInc1_;
    }

    // final move in tc1
    if (move == numMoves1_)
    {
        if (numMoves2_ == Unlimited)
            return timeInc1_ + timeAdd_;
        else
            return timeInc1_ + timeForMoves2_;
    }

    // tc2
    move -= numMoves1_;

    // All remaining in timeForMoves2
    if (numMoves2_ == Unlimited)
    {
        return timeInc2_;
    }

    // within tc2
    if (numMoves2_ != 0)
    {
        if (move < numMoves2_)
        {
            return timeInc2_;
        }

        // final move in tc2
        if (move == numMoves2_)
        {
            return timeInc3_ + timeAdd_;
        }
    }

    // tc3
    return timeInc3_;
}
