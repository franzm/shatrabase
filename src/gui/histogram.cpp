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

#include "histogram.h"
#include "databasemodel.h"

#include <QPainter>
#include <QDebug>
#include <QMenu>
#include <QAction>
#include <QListWidget>
#include <QLayout>
#include <QFrame>
#include <QMouseEvent>

#include <map>

Histogram::Histogram(QWidget *parent) :
    QWidget         (parent),
    m_messageSend   (false)
{
    setMouseTracking(true);

    // accepted values
    accepted_.insert("Moves");
    accepted_.insert("Ply");
    accepted_.insert("Pieces White");
    accepted_.insert("Pieces Black");
    accepted_.insert("Opening move");


    // set dark background
    QPalette p = palette();
    p.setColor(QPalette::Background, QColor(70,70,70));
    setPalette(p);
    setAutoFillBackground(true);

    // enable contextmenu
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(QPoint)),
                this, SLOT(showContextMenu(const QPoint&)));
}

// ---------- persistance ------------

/*
void Histogram::configure()
{

}*/


// -------- data handling ------------

void Histogram::clearData()
{
    map_.clear();

    //list_->clear();
    update();
}

void Histogram::clearData(const QString& key)
{
    Iter i = map_.find(key);
    if (i != map_.end())
    {
        map_.erase(i);
        update();
    }
}

Histogram::Data * Histogram::setData(const QString key, const QVector<int>& values)
{
    Iter i = map_.find(key);
    // create entry
    if (i == map_.end())
    {
        i = map_.insert(key, Data());
    }

    i.value().v = values;
    i.value().key = key;
    initData_(i.value());

    return &(i.value());
}


void Histogram::initData_(Data &d) const
{
    d.average = d.min_v = d.max_v;
    d.visible = false;
    if (!d.v.size()) return;

    d.visible = visible_[d.key];

    // get min/max/average
    d.max_v = d.min_v = d.v[0];
    for (int i=1; i<d.v.size(); ++i)
    {
        d.max_v = std::max((int)d.max_v, d.v[i]);
        d.min_v = std::min((int)d.min_v, d.v[i]);
        d.average += d.v[i];
    }
    d.average /= d.v.size();

    // display scale [0,1]
    const float range = (d.max_v - d.min_v);
    d.scaley = (range != 0)? 1.f / range : 1.f;

    d.pen = QPen(QColor((rand()&63)+192, (rand()&63)+192, (rand()&63)+192));
    d.pen.setWidth(1);
}



void Histogram::setDatabaseModel(const DatabaseModel & db)
{
    clearData();

    int rows = db.rowCount(QModelIndex());
    if (rows)
    for (int j = 0; j<db.columnCount(QModelIndex()); ++j)
    {
        // determine if numerical column
        const QVariant v = db.data(db.index(0,j,QModelIndex()), Qt::DisplayRole);
        bool ok;
        v.toInt(&ok);
        if (ok)
        {
            QString key = db.headerData(j, Qt::Horizontal, Qt::UserRole).toString();
            QString trkey = db.headerData(j, Qt::Horizontal, Qt::DisplayRole).toString();

            // don't add what's not predefined
            if (accepted_.find(key) == accepted_.end())
                continue;

            // update visibility
            if (!visible_.contains(trkey))
                visible_.insert(trkey, true);

            // create histogram
            std::map<int,int> set;
            for (int i=0; i<rows; ++i)
            {
                const int val = db.data(db.index(i,j,QModelIndex()), Qt::DisplayRole).toInt();
                set[val]++;
            }

            // transfer frequence vector
            QVector<int> vec(set.size());
            std::map<int,int>::const_iterator k = set.begin();
            for (int i=0; i<vec.size(); ++i, ++k)
                vec[i] = k->second;

            // create data entry
            Data * dat = setData(trkey, vec);

            // transfer 'what' vector
            dat->what.resize(set.size());
            size_t j = 0;
            for (std::map<int,int>::const_iterator i=set.begin(); i!=set.end(); ++i, ++j)
            {
                dat->what[j] = i->first;
            }

            // create an item for it
            /*QAction * act = new QAction(dat->key, list_);
            act->setCheckable(true);
            act->setChecked(visible_[key]);
            list_->addAction(act);*/
        }
    }

    update();
}



void Histogram::displayMessage(const QString &msg)
{
    m_messageSend = true;
    signalDisplayMessage(msg);
}

void Histogram::paintEvent(QPaintEvent * e)
{
    QWidget::paintEvent(e);

    for (ConstIter i = map_.begin(); i!=map_.end(); ++i)
        if (i.value().visible)
            paintCurve(i.value());
}

void Histogram::leaveEvent(QEvent *)
{
    if (m_messageSend)
    {
        m_messageSend = false;
        signalDisplayMessage("");
    }
}

void Histogram::mouseMoveEvent(QMouseEvent * e)
{
    if (!map_.empty())
    {
        QString msg;

        // needs to match paintCurve() below!!
        float x = (float)(e->pos().x() - 1) / (width()-2);

        for (Iter i=map_.begin(); i!=map_.end(); ++i)
        if (visible_[i.value().key])
        {
            int j = x * i.value().v.size();
            if (j >= 0 && j < i.value().v.size() &&
                          j < i.value().what.size())
            {
                //: 'num.' means 'number of' or 'count'
                msg += tr("%1: [%2: num. %3]").arg(i.value().key)
                        .arg(i.value().what[j])
                        .arg(i.value().v[j]);
            }
        }

        if (!msg.isNull())
            displayMessage(msg);
    }
    QWidget::mouseMoveEvent(e);
}

void Histogram::paintCurve(const Data &data)
{
    QPainter p(this);

    p.setPen(data.pen);

    const int xo = 1,
              yo = 1,
              w = width() - 2,
              h = height() - 2;
  /*  const int xo = frame_->pos().x() + frame_->frameWidth(),
              yo = frame_->pos().y() + frame_->frameWidth(),
              w = frame_->width() - frame_->frameWidth() * 2,
              h = frame_->height() - frame_->frameWidth() * 2;
    */
    int x0 = 0, y0 = 0, x, y;
    for (int i = 0; i<data.v.size(); ++i)
    {
        x = xo + (float)i / data.v.size() * (w-1);
        y = yo + h - 1 - (data.v[i] - data.min_v) * data.scaley * (h-2);

        if (i!=0)
        {
            p.drawLine(x0,y0, x,y);
        }
        x0 = x; y0 = y;
    }
}


void Histogram::showContextMenu(const QPoint& pos)
{
    // create context menu

    QMenu headerMenu;

    QAction* showAll = headerMenu.addAction(tr("Show all"));
    headerMenu.addSeparator();
    QAction* hideAll = headerMenu.addAction(tr("Hide all"));
    headerMenu.addSeparator();

    showAll->setEnabled(!map_.empty());
    hideAll->setEnabled(!map_.empty());

    for (ConstIter i = map_.begin(); i!=map_.end(); ++i)
    {
        QAction * a = headerMenu.addAction(i.key());
        a->setCheckable(true);
        a->setChecked(i.value().visible);
        a->setData(QVariant(i.key()));
    }

    // execute
    QAction* selectedItem = headerMenu.exec(mapToGlobal(pos));

    if (selectedItem == hideAll)
    {
        for (Iter i = map_.begin(); i!=map_.end(); ++i)
            i.value().visible = false;
    }
    if (selectedItem == showAll)
    {
        for (Iter i = map_.begin(); i!=map_.end(); ++i)
            i.value().visible = true;
    }
    else if (selectedItem && (QMetaType::Type)selectedItem->data().type() == QMetaType::QString)
    {
        // make data item (in-)visible
        const QString key = selectedItem->data().toString();
        Iter i = map_.find(key);
        if (i!=map_.end())
        {
            i.value().visible = selectedItem->isChecked();
        }
        // set visible_[] flag
        QMap<QString,bool>::Iterator j = visible_.find(key);
        if (j!=visible_.end())
            j.value() = selectedItem->isChecked();
    }

    update();
}

