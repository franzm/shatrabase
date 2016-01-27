 /**************************************************************************
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

#ifndef __BOARDLIST_H__
#define __BOARDLIST_H__

#include "move.h"
#include "urvct.hpp"

namespace SHATRA {

class BoardList : public urvct<Move,MaxGen>
{
  public:
    BoardList() : urvct<Move,MaxGen>() {}
    inline Move& add() { append(Move()); return back(); }    
};

} // namespace SHATRA

#endif // __BOARDLIST_H__
