/***************************************************************************
 *                           Shatrabase                                    *
 *   (C) 2012-2014 AFKM <francis@afkm.com>                                 *
 *                   derived from ChessX with thanks                       *
 *   (C) 2005 William Hoggarth <whoggarth@users.sourceforge.net>           *
 *   (C) 2005-2009 Michal Rudolf <mrudolf@kdewebdev.org>                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/


#include "movelist.h"

bool MoveList::isRepeating() const
{
    for (int j=size()-1; j>0; --j)
    {
        int rc = 0;
        for (int i=j-1; i>=0; --i)
        {
            if (at(i) == at(j))
                rc ++;

            if (rc>=2)
                return true;
        }
    }

    return false;
}

