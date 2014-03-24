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

class Board;
class PlayerSetup;

namespace Ui { class PlayerSelect; }

class PlayerSelect : public QWidget
{
    Q_OBJECT

public:
    explicit PlayerSelect(QWidget *parent = 0);
    ~PlayerSelect();

signals:
    /** The Engine has send a new position. */
    void positionUpdated(const Board& board);

public slots:
    void slotReconfigure();

    /** Sets new position. Signals that a move has been performed.
        If required, the Engine will be queried. */
    void setPosition(const Board& board);


protected slots:
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
    Ui::PlayerSelect * ui_;

    PlayerSetup * player_;
};

#endif // PLAYERSELECT_H
