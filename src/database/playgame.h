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

#ifndef PLAYERSETUP_H
#define PLAYERSETUP_H

#include <QObject>
#include <QString>
#include <QProcess>

class Board;
class Engine;
class Analysis;

/** This class handles the setup of the two players (human or engine)
 *  and can be used to play a game. */
class PlayGame : public QObject
{
    Q_OBJECT
public:
    explicit PlayGame(QObject *parent = 0);
    ~PlayGame();

    // ------- getter -----------

    /** Returns name of player 1. */
    const QString& playerName1() const;
    /** Returns name of player 1. */
    const QString& playerName2() const;

    /** Return name of engine, or null-string if human */
    const QString& engineName1() const;
    /** Return name of engine, or null-string if human */
    const QString& engineName2() const;

    bool engine1Ready() const { return engine1Active_; }
    bool engine2Ready() const { return engine2Active_; }

    bool player1IsEngine() const { return !engineName1_.isNull(); }
    bool player2IsEngine() const { return !engineName2_.isNull(); }


    // ------- setter -----------

    /** Sets the player name */
    void setPlayerName1(const QString& s);
    /** Sets the player name */
    void setPlayerName2(const QString& s);

    /** Sets the engine name (can be "" or null-string to disable) */
    void setEngineName1(const QString& s);
    /** Sets the engine name (can be "" or null-string to disable) */
    void setEngineName2(const QString& s);

signals:

public slots:
    /** Updates settings */
    void slotReconfigure();

    /** Start playing (creates Engines) */
    void start();

    /** Sets new position. Signals, that a move has been performed.
        If required, the Engine will be queried. */
    void setPosition(const Board& board);

protected slots:

    // ---- callbacks from Engine ----

    void engine1Activated_();
    void engine1Deactivated_();
    void engine1Error_(QProcess::ProcessError);
    void engine1Analysis_(const Analysis&);
    void engine2Activated_();
    void engine2Deactivated_();
    void engine2Error_(QProcess::ProcessError);
    void engine2Analysis_(const Analysis&);

private:

    /** See if string is okay and engine is known */
    bool checkEngineName_(const QString& name) const;
    /** Create the engines, if necessary */
    void createEngines_();

    QString engineName1_, engineName2_,
            name1_, name2_;

    /** current board */
    Board * board_;

    Engine * engine1_, * engine2_;

    bool engine1Active_, engine2Active_;
};

#endif // PLAYERSETUP_H
