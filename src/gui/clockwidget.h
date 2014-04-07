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

#ifndef CLOCKWIDGET_H
#define CLOCKWIDGET_H

#include <QWidget>
#include <QLCDNumber>

class ClockWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ClockWidget(QWidget *parent = 0);

    void setColor(bool white);

signals:

public slots:

    void setTotalTime(int seconds);
    void setMoveTime(int seconds);
    void setTime(int total_seconds, int move_seconds)
        { setTotalTime(total_seconds); setMoveTime(move_seconds); }

protected:

    QString timeString_(int second) const;

    QLCDNumber * newLcd_();
    void updateDisplays_();

    QLCDNumber * lcdTotal_, * lcdMove_;
};

#endif // CLOCKWIDGET_H
