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
#include "boardstyle.h"

#include "boardtheme.h"

BoardStyle::BoardStyle(QObject *parent)
    :   QObject(parent)
{
    setDefault();
}

void BoardStyle::setDefault()
{
    map_.insert("flipped", true);
    map_.insert("showMoat", true);
    map_.insert("showTower", true);
    map_.insert("showFrame", true);
    map_.insert("showSquareNumbers", false);
    map_.insert("frameWidth", 4);
    map_.insert("showCurrentMove", true);
    map_.insert("showAllMoves", true);
    map_.insert("animateMoves", true);
    map_.insert("animateMovesSpeed", 20.0);
    map_.insert("animateMovesLength", 0.2);
    map_.insert("animateMovesSpeedVsLength", 0.5);
    map_.insert("pieceTheme", "motifshatra");
    map_.insert("pieceEffect", BoardTheme::Plain);
    map_.insert("boardTheme", "shatra1");
    map_.insert("lightColor", QColor(Qt::lightGray));
    map_.insert("darkColor", QColor(Qt::darkGray));
    map_.insert("highlightColor", QColor(Qt::yellow));
    map_.insert("frameColor", QColor(Qt::black));
    map_.insert("currentMoveColor", QColor(Qt::blue));
    map_.insert("backgroundColor", QColor(50,70,100));
    map_.insert("backgroundColor2", QColor(90,70,50));
}


