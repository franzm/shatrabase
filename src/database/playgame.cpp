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

#include "playgame.h"
#include "settings.h"
#include "enginelist.h"
#include "engine.h"

PlayGame::PlayGame(QObject *parent)
    :   QObject     (parent),
        engine1_    (new PlayGameEngine(this)),
        engine2_    (new PlayGameEngine(this))
{
    SB_PLAY_DEBUG("PlayGame::PlayGame(...)");

    slotReconfigure();
}

PlayGame::~PlayGame()
{
    SB_PLAY_DEBUG("PlayGame::~PlayGame()");
}

bool PlayGame::checkEngineName_(const QString& name) const
{
    if (name.isNull() || name == "") return false;

    EngineList elist;
    elist.restore();

    return elist.names().contains(name);
}

void PlayGame::slotReconfigure()
{
    engineName1_ = AppSettings->getValue("/PlayGame/Engine1").toString();
    engineName2_ = AppSettings->getValue("/PlayGame/Engine2").toString();

    // make a null-string for "" or when engine is not in the list
    if (!checkEngineName_(engineName1_))
        engineName1_ = QString();
    if (!checkEngineName_(engineName2_))
        engineName2_ = QString();

    name1_ = AppSettings->getValue("/PlayGame/Name1").toString();
    name2_ = AppSettings->getValue("/PlayGame/Name2").toString();
}

void PlayGame::activate()
{
    SB_PLAY_DEBUG("PlayGame::activate()");

    if (!engineName1_.isNull())
    {
        engine1_->startEngine(engineName1_);
    }

    if (!engineName2_.isNull())
    {
        engine2_->startEngine(engineName2_);
    }
}

const QString& PlayGame::playerName1() const
{
    return name1_;
}

const QString& PlayGame::playerName2() const
{
    return name2_;
}

const QString& PlayGame::engineName1() const
{
    return engineName1_;
}

const QString& PlayGame::engineName2() const
{
    return engineName2_;
}

void PlayGame::setPlayerName1(const QString &s)
{
    name1_ = s;
    AppSettings->setValue("/PlayGame/Name1", name1_);
}

void PlayGame::setPlayerName2(const QString &s)
{
    name2_ = s;
    AppSettings->setValue("/PlayGame/Name2", name2_);
}

void PlayGame::setEngineName1(const QString &s)
{
    if (!checkEngineName_(s))
        engineName1_ = QString();
    else
        engineName1_ = s;

    AppSettings->setValue("/PlayGame/Engine1", engineName1_);
}

void PlayGame::setEngineName2(const QString &s)
{
    if (!checkEngineName_(s))
        engineName2_ = QString();
    else
        engineName2_ = s;

    AppSettings->setValue("/PlayGame/Engine2", engineName2_);
}


bool PlayGame::setPosition(const Board& board)
{
    SB_PLAY_DEBUG("PlayGame::setPosition(Board) stm = " << board.toMove());

    if (board.toMove() == White)
    {
        return (engine1_ && engine1_->setPosition(board));
    }
    else
    {
        return (engine2_ && engine2_->setPosition(board));
    }

    SB_PLAY_DEBUG("...did not care for the position, "
                  "because there was no engine set up for this player.");

    return false;
}

void PlayGame::engineMove1_(Move m)
{
    emit moveMade1(m);
}

void PlayGame::engineMove2_(Move m)
{
    emit moveMade2(m);
}
