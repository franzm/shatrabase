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

#include <QObject>
#include <QString>

// switch to turn off clocks atm
//#define SB_NO_CLOCKS

/** Names of TimeControl::Type
    used for (re-)storing settings. */
extern const char * timeControlTypeName[];

/** Simple container for play time settings.

   It's a QObject more or less for tr()
   and because derived PlayTimeControl sends signals.

*/
class TimeControl : public QObject
{
    Q_OBJECT
public:

    /** Order has to follow timeControlTypeName[] !! */
    enum Type
    {
        /** Free play */
        T_None,
        /** Match opponent's time */
        T_Match,
        /** Limit time/nodes/depth */
        T_Limit,
        /**   timeForMoves1() with timeInc1() for numMoves1()
         *  + timeForMoves2() with timeInc2() for numMoves2()
         *  + timeAdd() with timeInc3() */
        T_Tournament,
        /** no valid type */
        MaxTypes
    };

    enum Format
    {
        F_Long,
        F_Seconds,
        F_HMS,
        F_HMSMS,
        /** no valid format */
        MaxFormat
    };
    static const QString& formatName(Format f);
    static const QString& formatNameTr(Format f);
    static Format formatFromName(const QString& );

    enum { Unlimited = -1 };

    TimeControl(QObject * parent = 0);

    // ------- strings ---------

    /** Return humand readable description of current settings */
    QString humanReadable() const;

    /** Nice readable string according to AppSettings */
    QString msecToString(int msec) const;

    // ------ game time info --------

    /** Returns wheter the time is up. */
    //bool isTimeout(int last_move_number, int time_after_last_move_msec) const;

    /** Returns the total time at start of game,
        or Unlimited if not in tournament mode. */
    int totalTimeAtStart() const;

    /** Returns the time increment after the move. */
    int getTimeInc(int last_move_number) const;


    // --------- getter -------------

    /** Type of time control */
    Type type() const { return type_; }

    Format format() const { return format_; }

    /** Number of moves to complete in timeForMoves1().
        If Unlimited, all moves have to be completed in time. */
    int numMoves1() const { return numMoves1_; }

    /** Number of moves to complete in timeForMoves2().
        If Unlimited, all moves have to be completed in time.
        If 0, this is not used and TimeAdd() kicks in. */
    int numMoves2() const { return numMoves2_; }

    /** Time (msec) to complete numMoves1() */
    int timeForMoves1() const { return timeForMoves1_; }

    /** Time (msec) to complete numMoves2() */
    int timeForMoves2() const { return timeForMoves2_; }

    /** Time (msec) to add after each move */
    int timeInc1() const { return timeInc1_; }

    /** Time (msec) to add after each move */
    int timeInc2() const { return timeInc2_; }

    /** Time (msec) to add after each move */
    int timeInc3() const { return timeInc3_; }

    /** Time (msec) to add after numMoves2() */
    int timeAdd() const { return timeAdd_; }

    /** Maximum time (msec) per move, or Unlimited */
    int timeLimit() const { return timeLimit_; }

    /** Maximum nodes per move, or Unlimited */
    int nodeLimit() const { return nodeLimit_; }

    /** Maximum search depth per move, or Unlimited */
    int depthLimit() const { return depthLimit_; }

    // ------- setter ---------------

    void setType(Type type) { type_ = type; }
    void setFormat(Format f) { format_ = f; }

    void setNumMoves1(int moves) { numMoves1_ = moves; }
    void setNumMoves2(int moves) { numMoves2_ = moves; }
    void setTimeForMoves1(int msec) { timeForMoves1_ = msec; }
    void setTimeForMoves2(int msec) { timeForMoves2_ = msec; }
    void setTimeInc1(int msec) { timeInc1_ = msec; }
    void setTimeInc2(int msec) { timeInc2_ = msec; }
    void setTimeInc3(int msec) { timeInc3_ = msec; }
    void setTimeAdd(int msec) { timeAdd_ = msec; }
    void setTimeLimit(int msec) { timeLimit_ = msec; }
    void setNodeLimit(int nodes) { nodeLimit_ = nodes; }
    void setDepthLimit(int depth) { depthLimit_ = depth; }

    /** Read from AppSettings */
    void configure();

private:

    Type type_;
    Format format_;

    int numMoves1_,
        numMoves2_,
        timeForMoves1_,
        timeForMoves2_,
        timeInc1_,
        timeInc2_,
        timeInc3_,
        timeAdd_,
        timeLimit_,
        nodeLimit_,
        depthLimit_;
};

#endif // TIMECONTROL_H
