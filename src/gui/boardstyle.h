/***************************************************************************
 *                           Shatrabase                                    *
 *   (C) 2012-2014 AFKM <francis@afkm.com>                                 *
 *                   derived from ChessX with thanks                       *
 *   (C) 2005-2009 Michal Rudolf <mrudolf@kdewebdev.org>                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef BOARDSTYLE_H
#define BOARDSTYLE_H

#include <QObject>
#include <QVariant>
#include <QString>
#include <QMap>

class BoardStyle : public QObject
{
    Q_OBJECT
public:
    explicit BoardStyle(QObject *parent = 0);

//signals:

    void setDefault();

public slots:

private:
    QMap<QString, QVariant> map_;
};

#endif // BOARDSTYLE_H
