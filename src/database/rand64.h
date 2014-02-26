/***************************************************************************
 *                           Shatrabase                                    *
 *   (C) 2012-2014 AFKM <francis@afkm.com>                                 *
 *                   derived from ChessX with thanks                       *
 *   (C) 2005 Marius Roets <roets.marius@gmail.com>                        *
 *   (C) 2006-2009 Michal Rudolf <mrudolf@kdewebdev.org>                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef __RAND64_H__
#define __RAND64_H__


#define MAX_PIECES 10
#define MAX_SQUARES 64 // two spare :)
#define MAX_EN_PASSANT_SQUARES 10
#define MAX_URGENT_SQUARES 20


extern quint64 RAND_VALUES[MAX_PIECES][MAX_SQUARES];
extern quint64 RAND_EN_PASSANT[MAX_EN_PASSANT_SQUARES];
extern quint64 RAND_URGENT[MAX_URGENT_SQUARES];
extern quint64 RAND_TRANSIT[MAX_SQUARES];
extern quint64 RAND_TEMDEK[2];
extern quint64 START_HASH;
extern quint64 RAND_TO_MOVE;


#endif
