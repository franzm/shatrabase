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
 
#include "arrayhashkey.h"


int arrayHashkey::initHashkeys(quint64* hk, uint rows, uint cols)
{
    static bool firsttime = true;
    if (firsttime) seedRandomGenerator(0xBD27112D);

    m_n = rows * cols;
    m_fix = 0;
    m_to = m_at; uint i , j;

    for (i = 0; i < m_n; ++i, ++m_to)
    {
        m_list.append(ranval());
        if (m_to) checkUnique(m_list[m_to]);
    }
    for (i = 0; i < rows; ++i)
    {
        m_rowOffset = (i * cols);
        for (j = 0; j < cols; ++j)
        {
            hk[m_rowOffset + j] = m_list[m_at++];
        }
    }
    firsttime = false;
    return m_fix;
}

inline void arrayHashkey::checkUnique(quint64 val)
{
    for (uint i = 0; i < m_to; ++i)
        if (m_list[i] == val) --m_to, --i, ++m_fix;
}

#define rot(x,k) (((x)<<(k))|((x)>>(64-(k))))
inline quint64 arrayHashkey::ranval()
{
    quint64 e = x.a - rot(x.b, 7);
    x.a = x.b ^ rot(x.c, 13);
    x.b = x.c + rot(x.d, 37);
    x.c = x.d + e;
    x.d = e + x.a;
    return x.d;
}

inline void arrayHashkey::raninit(quint64 seed)
{
    x.a = 0xf1ea5eed, x.b = x.c = x.d = seed;
    for (uint i = 0; i < 20; ++i) (void)ranval();
}
#undef rot
