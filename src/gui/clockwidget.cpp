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

#include <QLayout>
#include <QPalette>
#include <QTime>

#include "clockwidget.h"

ClockWidget::ClockWidget(QWidget *parent)
    :   QWidget(parent)
{
    QVBoxLayout * l = new QVBoxLayout(this);
    l->setMargin(0);
    l->setSpacing(0);

    lcdTotal_ = newLcd_();
    l->addWidget(lcdTotal_);

    lcdMove_ = newLcd_();
    l->addWidget(lcdMove_);

    setColor(true);

    setTotalTime(0);
    setMoveTime(0);
}

QLCDNumber * ClockWidget::newLcd_()
{
    QLCDNumber * n = new QLCDNumber(this);

    const int s = 14;
<<<<<<< HEAD
    n->setFixedSize(s*9,s*3);
=======
    n->setFixedSize(s*9,s*3);//s*7,s*2
>>>>>>> 0b63a16179940f3f08ea1f718a2b7afa4315315e
    n->setDigitCount(7);
    n->setSegmentStyle(QLCDNumber::Filled);
    n->setMode(QLCDNumber::Dec);
    n->setAutoFillBackground(true);

    return n;
}

void ClockWidget::setVisible(bool total, bool move)
{
    if (!(total || move))
    {
        QWidget::setVisible(false);
        return;
    }

    if (!isVisible())
        QWidget::setVisible(true);

    lcdTotal_->setVisible(total);
    lcdMove_->setVisible(move);
}

void ClockWidget::setColor(bool white)
{
    QPalette p = lcdTotal_->palette();

    p.setColor(QPalette::Light,      white? Qt::black : Qt::white);
    p.setColor(QPalette::Foreground, white? Qt::black : Qt::white);
    p.setColor(QPalette::Background, white? Qt::white : Qt::black);

    lcdTotal_->setPalette(p);
    lcdMove_->setPalette(p);
}

void ClockWidget::updateDisplays_()
{
    /*
    QPalette p = lcdTotal_->palette();
    p.setColor(QPalette::Foreground,
               s <= 30 ?
                    secondsLeft() <= 10 ?
                        QColor(150,10,10) : QColor(120,40,0)
                    : Qt::black );
    lcdTotal_->setPalette(p);
    */

    //lcdTotal_->display("0:00:00");
    //lcdMove_->display("0:00:00");
}

QString ClockWidget::timeString_(int seconds) const
{
    int h = seconds/60/60,
        m = (seconds/60)%60,
        s = seconds%60;

    QTime t;
    t.setHMS(h, m, s);
    if (h)
        return t.toString("h:mm:ss");
    if (m)
        return t.toString("m:ss");
    return t.toString("s");
}

void ClockWidget::setTotalTime(int msec)
{
    lcdTotal_->display(timeString_(msec/1000));
}

void ClockWidget::setMoveTime(int msec)
{

    lcdMove_->display(timeString_(msec/1000));
}
