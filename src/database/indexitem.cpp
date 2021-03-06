/***************************************************************************
 *                           Shatrabase                                    *
 *   (C) 2012-2014 AFKM <francis@afkm.com>                                 *
 *                   derived from ChessX with thanks                       *
 *   (C) 2005-2006 Marius Roets <roets.marius@gmail.com>                   *
 *   (C) 2007 Rico Zenklusen <rico_z@users.sourceforge.net>                *
 *   (C) 2007-2009 Michal Rudolf <mrudolf@kdewebdev.org>                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include <QtDebug>

#include "indexitem.h"
#include "index.h"


IndexItem::IndexItem()
{
}

IndexItem::~IndexItem()
{
    m_mapTagIndexToValueIndex.clear();
}

void IndexItem::set(SHATRA::TagIndex tagIndex, SHATRA::ValueIndex valueIndex)
{
    m_mapTagIndexToValueIndex[tagIndex] = valueIndex;
}

SHATRA::ValueIndex IndexItem::valueIndex(SHATRA::TagIndex tagIndex) const
{
    if (m_mapTagIndexToValueIndex.contains(tagIndex))
    {
        return m_mapTagIndexToValueIndex[tagIndex];
    }
    return 0;
}

bool IndexItem::hasTagIndex(SHATRA::TagIndex tagIndex) const
{
    return (m_mapTagIndexToValueIndex.contains(tagIndex));
}

void IndexItem::write(QDataStream& out) const
{
    out << m_mapTagIndexToValueIndex;
}

void IndexItem::read(QDataStream& in)
{
    in >> m_mapTagIndexToValueIndex;
}

