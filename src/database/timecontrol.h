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

#ifndef TIMECONTROL_H
#define TIMECONTROL_H

/** Simple container for play time settings.

mail 20140504-4:39pm

1) Free (ie move only when 'move now' button is pressed (the latter
something we definitely need anyway, only to be disabled during
tournament play)
2) Time/nodes/depth per move
3) Average time per move
4) Match opponent's move times
5) Tournament settings
eg tc1 (n moves - or 'all' - in x secs/millisecs, optional increment y
millisecs)
   tc2 (the same)
   tc3 (the same) - which should cover everything...


*/
class TimeControl
{
public:

    enum Type
    {
        /** No time limit at all */
        T_None,
        /** Average time per move */
        T_Average,
        /** timeAll() for numMoves() + timeAdd() + timeInc() per move */
        T_All,
        /** Match opponent's time */
        T_Match,
        /** Limit time/nodes/depth */
        T_Limit
    };

    enum { Unlimited = -1 };

    TimeControl();

    // --------- getter -------------

    /** Type of time control */
    Type type() const { return type_; }

    /** Number of moves to complete in timeAll() */
    int numMoves() const { return numMoves_; }

    /** Time (sec) to complete numMoves() */
    int timeAll() const { return timeAll_; }

    /** Time (sec) to add after numMoves() */
    int timeAdd() const { return timeAdd_; }

    /** Time (sec) to add after each move */
    int timeInc() const { return timeInc_; }

    /** Average time (sec) per move */
    int timeAverage() const { return timeAv_; }

    /** Maximum time (sec) per move, or Unlimited */
    int timeLimit() const { return timeLimit_; }

    /** Maximum nodes per move, or Unlimited */
    int nodeLimit() const { return nodeLimit_; }

    /** Maximum search depth per move, or Unlimited */
    int depthLimit() const { return depthLimit_; }

    // ------- setter ---------------

    void type(Type type) { type_ = type; }
    void numMoves(int moves) { numMoves_ = moves; }
    void timeAll(int sec) { timeAll_ = sec; }
    void timeAdd(int sec) { timeAdd_ = sec; }
    void timeInc(int sec) { timeInc_ = sec; }
    void timeAverage(int sec) { timeAv_ = sec; }
    void timeLimit(int sec) { timeLimit_ = sec; }
    void nodeLimit(int nodes) { nodeLimit_ = nodes; }
    void depthLimit(int depth) { depthLimit_ = depth; }

private:

    Type type_;

    int numMoves_,
        timeAll_,
        timeAdd_,
        timeInc_,
        timeAv_,
        timeLimit_,
        nodeLimit_,
        depthLimit_;
};

#endif // TIMECONTROL_H
