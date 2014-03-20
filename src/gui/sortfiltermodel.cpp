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

#include "sortfiltermodel.h"
#include "filtermodel.h"

#include <QDate>
#include <QTime>

SortFilterModel::SortFilterModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{
}


bool SortFilterModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    QVariant dleft = sourceModel()->data(left),
             dright = sourceModel()->data(right);

#define SB_VARIANT_COMPARE2(Type__, ToType__) \
    if ((QMetaType::Type)dleft.type() == QMetaType::Type__) \
        return dleft.to##ToType__() < dright.to##ToType__(); else

#define SB_VARIANT_COMPARE1(Type__) \
    SB_VARIANT_COMPARE2(Type__, Type__)

    SB_VARIANT_COMPARE1(Int)
    SB_VARIANT_COMPARE1(UInt)
    //SB_VARIANT_COMPARE(Long)
    //SB_VARIANT_COMPARE(ULong)
    SB_VARIANT_COMPARE1(LongLong)
    SB_VARIANT_COMPARE1(ULongLong)
    //SB_VARIANT_COMPARE(Short)
    //SB_VARIANT_COMPARE(UShort)
    SB_VARIANT_COMPARE1(Char)
    //SB_VARIANT_COMPARE(UChar)
    SB_VARIANT_COMPARE1(Float)
    SB_VARIANT_COMPARE1(Double)
    SB_VARIANT_COMPARE2(QDate, Date)
    SB_VARIANT_COMPARE2(QTime, Time)

#undef SB_VARIANT_COMPARE1
#undef SB_VARIANT_COMPARE2

    // else compare string rep.
    return dleft.toString() < dright.toString();
}


FilterModel * SortFilterModel::sourceFilter() const
{
    return static_cast<FilterModel*>(sourceModel());
}
