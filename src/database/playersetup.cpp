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

#include "playersetup.h"
#include "settings.h"
#include "enginelist.h"

#include "board.h"
#include "engine.h"

PlayerSetup::PlayerSetup(QObject *parent)
    :   QObject     (parent),
        board_      (new Board),
        engine1_    (0),
        engine2_    (0),
        engine1Active_  (false),
        engine2Active_  (false)
{
}

PlayerSetup::~PlayerSetup()
{
    if (engine1_) delete engine1_;
    if (engine2_) delete engine2_;
    delete board_;
}

bool PlayerSetup::checkEngineName_(const QString& name) const
{
    if (name.isNull() || name == "") return false;

    EngineList elist;
    elist.restore();

    // make a null-string for "" or when engine is not in the list
    return elist.names().contains(name);
}

void PlayerSetup::slotReconfigure()
{
    engineName1_ = AppSettings->getValue("/PlayerSetup/Engine1").toString();
    engineName2_ = AppSettings->getValue("/PlayerSetup/Engine2").toString();

    if (!checkEngineName_(engineName1_))
        engineName1_ = QString();
    if (!checkEngineName_(engineName2_))
        engineName2_ = QString();

    name1_ = AppSettings->getValue("/PlayerSetup/Name1").toString();
    name2_ = AppSettings->getValue("/PlayerSetup/Name2").toString();
}

void PlayerSetup::start()
{
    createEngines_();
}

const QString& PlayerSetup::playerName1() const
{
    return name1_;
}

const QString& PlayerSetup::playerName2() const
{
    return name2_;
}

const QString& PlayerSetup::engineName1() const
{
    return engineName1_;
}

const QString& PlayerSetup::engineName2() const
{
    return engineName2_;
}

void PlayerSetup::setPlayerName1(const QString &s)
{
    name1_ = s;
    AppSettings->setValue("/PlayerSetup/Name1", name1_);
}

void PlayerSetup::setPlayerName2(const QString &s)
{
    name2_ = s;
    AppSettings->setValue("/PlayerSetup/Name2", name2_);
}

void PlayerSetup::setEngineName1(const QString &s)
{
    if (!checkEngineName_(s))
        engineName1_ = QString();
    else
        engineName1_ = s;

    AppSettings->setValue("/PlayerSetup/Engine1", engineName1_);
}

void PlayerSetup::setEngineName2(const QString &s)
{
    if (!checkEngineName_(s))
        engineName2_ = QString();
    else
        engineName2_ = s;

    AppSettings->setValue("/PlayerSetup/Engine2", engineName2_);
}


void PlayerSetup::setPosition(const Board& board)
{
    *board_ = board;
}


void PlayerSetup::createEngines_()
{
    if (engine1_) delete engine1_;
    if (engine2_) delete engine2_;
    engine1_ = engine2_ = 0;
    engine1Active_ = engine2Active_ = false;

    EngineList elist;
    elist.restore();

    // setup engine 1
    int i = elist.names().indexOf(engineName1_);
    if (i >= 0)
    {
        engine1_ = Engine::newEngine(i);

        connect(engine1_, SIGNAL(activated()), SLOT(engine1Activated_()));
        connect(engine1_, SIGNAL(error(QProcess::ProcessError)), SLOT(engine1Error_(QProcess::ProcessError)));
        connect(engine1_, SIGNAL(deactivated()), SLOT(engine1Deactivated_()));
        connect(engine1_, SIGNAL(analysisUpdated(const Analysis&)),
                                    SLOT(engine1Analysis_(const Analysis&)));
        engine1_->activate();
    }

    // setup engine 2
    i = elist.names().indexOf(engineName2_);
    if (i >= 0)
    {
        engine2_ = Engine::newEngine(i);

        connect(engine2_, SIGNAL(activated()), SLOT(engine2Activated_()));
        connect(engine2_, SIGNAL(error(QProcess::ProcessError)), SLOT(engine2Error_(QProcess::ProcessError)));
        connect(engine2_, SIGNAL(deactivated()), SLOT(engine2Deactivated_()));
        connect(engine2_, SIGNAL(analysisUpdated(const Analysis&)),
                                    SLOT(engine2Analysis_(const Analysis&)));
        engine2_->activate();
    }
}

void PlayerSetup::engine1Activated_()
{
    engine1Active_ = true;
}

void PlayerSetup::engine1Deactivated_()
{
    engine1Active_ = false;
}

void PlayerSetup::engine1Error_(QProcess::ProcessError)
{

}

void PlayerSetup::engine1Analysis_(const Analysis&)
{

}

void PlayerSetup::engine2Activated_()
{
    engine2Active_ = true;
}

void PlayerSetup::engine2Deactivated_()
{
    engine2Active_ = false;
}

void PlayerSetup::engine2Error_(QProcess::ProcessError)
{

}

void PlayerSetup::engine2Analysis_(const Analysis&)
{

}
