/***************************************************************************
 *                           Shatrabase                                    *
 *   (C) 2012-2014 AFKM <francis@afkm.com>                                 *
 *                   derived from ChessX with thanks                       *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
 
#ifndef __ARRAYHASHKEY_H__
#define __ARRAYHASHKEY_H__

#include <QList>

/* many thanks due to Bob Jenkins for the concise PRNG
   find it at http://burtleburtle.net/bob/index.html */

typedef unsigned int uint;

class arrayHashkey
{
public:
    arrayHashkey() : m_at(0)
    {}
    ~arrayHashkey()
    {}
    int initHashkeys(quint64* m_hk, uint rows, uint cols=1);
private:
    void seedRandomGenerator(quint64 seed) { raninit(seed); }
    void checkUnique(quint64 val);
    quint64 ranval();
    void raninit(quint64 seed);

    struct ranacc { quint64 a; quint64 b; quint64 c; quint64 d; } x;
    QList<quint64> m_list;
    quint64* m_hk;
    uint m_n, m_at, m_to, m_rowOffset, m_fix;
};

#endif
