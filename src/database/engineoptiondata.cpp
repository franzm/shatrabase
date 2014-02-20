/***************************************************************************
 *                           Shatrabase                                    *
 *   (C) 2012-2014 AFKM <francis@afkm.com>                                 *
 *                   derived from ChessX with thanks                       *
 *   Copyright (C) 2012 by Jens Nissen jens-chessx@gmx.net                 *
 ***************************************************************************/

#include "engineoptiondata.h"

EngineOptionData::EngineOptionData()
{
}

EngineOptionData* EngineOptionData::FindInList(QString name, QList<EngineOptionData>& list)
{
    QList<EngineOptionData>::iterator it = list.begin();
    while(it != list.end())
    {
        if ((*it).m_name == name)
        {
            return &(*it);
        }
        ++it;
    }
    return 0;
}
