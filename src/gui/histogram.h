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

#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <QWidget>
#include <QMap>
#include <QString>
#include <QVector>
#include <QPen>

class QListWidget;
class QFrame;
class DatabaseModel;

/** Widget for showing a histogram (frequency of values) of
    Database values. */
class Histogram : public QWidget
{
    Q_OBJECT
public:

    // --------- types --------

    struct Data
    {
        QString key;
        /** vector of floats */
        QVector<float> v;
        /** min and max values */
        float min_v, max_v,
        /** average value */
            average,
        /** multiplier to get [0,1] */
            scaley;

        QPen pen;

        bool visible;
    };

    // -------- ctor ---------------

    explicit Histogram(QWidget *parent = 0);

    // --------- persistance -------------
//public slots:
//    void configure();

    // ------ raw data handling ----------
public:
    /** Clears all */
    void clearData();

    /** Clears specific key */
    void clearData(const QString& key);

    /** Sets a raw data curve.
        Everything for key will be overwritten. */
    Data * setData(const QString key, const QVector<float>& values);

    // ------- database handling ---------

    /** Reset all internal data to readable entries in db */
    void setDatabaseModel(const DatabaseModel & db);

signals:

public slots:
    void showContextMenu(const QPoint& pos);

protected:

    virtual void paintEvent(QPaintEvent * e);
    virtual void mouseMoveEvent(QMouseEvent * e);

    // ------------- stuff ------------

    /** Call after assigning Data::key and Data::v */
    void initData_(Data& d) const;

    void paintCurve(const Data& data);

    QMap<QString, Data> map_;
    typedef QMap<QString, Data>::Iterator Iter;
    typedef QMap<QString, Data>::ConstIterator ConstIter;

    QMap<QString, bool> visible_;

    // ------ widgets ------

    QListWidget * list_;
    QFrame * frame_;
};

#endif // HISTOGRAM_H
