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

#include <map>

Histogram::Histogram(QWidget *parent) :
    QWidget(parent)
{
}


// -------- data handling ------------

void Histogram::clearData()
{
    map_.clear();
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

void Histogram::setData(const QString key, const QVector<float>& values)
{
    Iter i = map_.find(key);
    // create entry
    if (i == map_.end())
    {
        i = map_.insert(key, Data());
    }

    i.value().v = values;
    initData_(i.value());
}


void Histogram::initData_(Data &d) const
{
    d.average = d.min_v = d.max_v;
    if (!d.v.size()) return;

    // get min/max/average
    d.max_v = d.min_v = d.v[0];
    for (int i=1; i<d.v.size(); ++i)
    {
        d.max_v = std::max(d.max_v, d.v[i]);
        d.min_v = std::min(d.min_v, d.v[i]);
        d.average += d.v[i];
    }
    d.average /= d.v.size();

    const float range = (d.max_v - d.min_v);
    d.scaley = (range != 0)? 1.f / range : 1.f;
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
            // create histogram
            std::map<int,int> set;
            for (int i=0; i<rows; ++i)
            {
                const int val = db.data(db.index(i,j,QModelIndex()), Qt::DisplayRole).toInt();
                set[val]++;
            }

            // transfer vector
            QVector<float> vec(set.size());
            std::map<int,int>::const_iterator k = set.begin();
            for (int i=0; i<vec.size(); ++i, ++k)
                vec[i] = k->second;

            QString key = db.headerData(j, Qt::Horizontal, Qt::DisplayRole).toString();
            setData(key, vec);
        }
    }
    update();
}





void Histogram::paintEvent(QPaintEvent * e)
{
    QWidget::paintEvent(e);

    for (ConstIter i = map_.begin(); i!=map_.end(); ++i)
        paintCurve(i.value());
}

void Histogram::paintCurve(const Data &data)
{
    QPainter p(this);

    int x0 = 0, y0 = 0, x, y;
    for (int i = 0; i<data.v.size(); ++i)
    {
        x = (float)i / data.v.size() * width();
        y = (1.f - (data.v[i] - data.min_v) * data.scaley) * height();

        if (i!=0)
        {
            p.drawLine(x0,y0, x,y);
        }
        x0 = x; y0 = y;
    }
}



