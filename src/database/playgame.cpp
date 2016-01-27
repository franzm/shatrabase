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


PlayGame::PlayGame(EngineDebugWidget * debug, QObject *parent)
    :   QObject         (parent),
        engineDebug_    (debug),
        engine1_        (new PlayGameEngine(engineDebug_, this)),
        engine2_        (new PlayGameEngine(engineDebug_, this)),
        player1Ready_   (false),
        player2Ready_   (false),
        readySend_      (false)
{
    SB_PLAY_DEBUG("PlayGame::PlayGame(...)");

    connect(engine1_, SIGNAL(moveMade(SHATRA::Move)), SLOT(engineMove1_(SHATRA::Move)));
    connect(engine2_, SIGNAL(moveMade(SHATRA::Move)), SLOT(engineMove2_(SHATRA::Move)));

    connect(engine1_, SIGNAL(moveInfo(SHATRA::Move,int)), SIGNAL(moveInfo1(SHATRA::Move,int)));
    connect(engine2_, SIGNAL(moveInfo(SHATRA::Move,int)), SIGNAL(moveInfo2(SHATRA::Move,int)));

    connect(engine1_, SIGNAL(ready()), SLOT(engineActivated1_()));
    connect(engine2_, SIGNAL(ready()), SLOT(engineActivated2_()));

    connect(engine1_, SIGNAL(engineClueless()), SIGNAL(engineClueless()));
    connect(engine2_, SIGNAL(engineClueless()), SIGNAL(engineClueless()));

    connect(engine1_, SIGNAL(engineCrashed(QString)), SLOT(engineError_(QString)));
    connect(engine1_, SIGNAL(engineCrashed(QString)), SLOT(engineError_(QString)));

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
    engineName1_ = AppSettings->getValue("/PlayGame/Player1/engine").toString();
    engineName2_ = AppSettings->getValue("/PlayGame/Player2/engine").toString();

    // make a null-string for "" or when engine is not in the list
    if (!checkEngineName_(engineName1_))
        engineName1_ = QString();
    if (!checkEngineName_(engineName2_))
        engineName2_ = QString();

    SB_PLAY_DEBUG("PlayGame::reconfigure(): engine1="<<engineName1_<<" engine2="<<engineName2_);

    name1_ = AppSettings->getValue("/PlayGame/Player1/name").toString();
    name2_ = AppSettings->getValue("/PlayGame/Player2/name").toString();
}

void PlayGame::activate()
{
    SB_PLAY_DEBUG("PlayGame::activate()");

    readySend_ = false;

    if (!engineName1_.isNull())
    {
        player1Ready_ = false;
        engine1_->startEngine(engineName1_);
        engine1_->setWaitTime(
                    AppSettings->getValue("/PlayGame/Player1/minTime").toInt()
                    );
    }
    else player1Ready_ = true;

    if (!engineName2_.isNull())
    {
        player2Ready_ = false;
        engine2_->startEngine(engineName2_);
        engine2_->setWaitTime(
                    AppSettings->getValue("/PlayGame/Player2/minTime").toInt()
                    );
    }
    else player2Ready_ = true;
}

void PlayGame::deactivate()
{
    engine1_->stop();
    engine2_->stop();
    player1Ready_ = player2Ready_ = readySend_ = false;
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
    AppSettings->setValue("/PlayGame/Player1/name", name1_);
}

void PlayGame::setPlayerName2(const QString &s)
{
    name2_ = s;
    AppSettings->setValue("/PlayGame/Player2/name", name2_);
}

void PlayGame::setEngineName1(const QString &s)
{
    if (!checkEngineName_(s))
        engineName1_ = QString();
    else
        engineName1_ = s;

    AppSettings->setValue("/PlayGame/Player1/engine", engineName1_);
}

void PlayGame::setEngineName2(const QString &s)
{
    if (!checkEngineName_(s))
        engineName2_ = QString();
    else
        engineName2_ = s;

    AppSettings->setValue("/PlayGame/Player2/engine", engineName2_);
}


bool PlayGame::setPosition(const SHATRA::Board& board, const Engine::SearchSettings& settings)
{
    SB_PLAY_DEBUG("PlayGame::setPosition(Board) stm = " << board.toMove());

    if (board.toMove() == SHATRA::White)
    {
        if (engine1_) return engine1_->setPosition(board, settings);
    }
    else
    {
        if (engine2_) return engine2_->setPosition(board, settings);
    }

    //SB_PLAY_DEBUG("...did not care for the position, "
    //              "because there was no engine set up for this player.");

    return false;
}

void PlayGame::engineMove1_(SHATRA::Move m)
{
    SB_PLAY_DEBUG("PlayGame::engineMove1_("<<m.from()<<"-"<<m.to()<<")");

    emit moveMade1(m);
}

void PlayGame::engineMove2_(SHATRA::Move m)
{
    SB_PLAY_DEBUG("PlayGame::engineMove2_("<<m.from()<<"-"<<m.to()<<")");

    emit moveMade2(m);
}


// one of these should fire ;)

void PlayGame::engineActivated1_()
{
    SB_PLAY_DEBUG("PlayGame::engineActivated1_() player1Ready_="<<player1Ready_
                     <<" player2Ready_="<<player2Ready_<<" readySend_="<<readySend_);

    if (!readySend_ && player2Ready_)
    {
        readySend_ = true;
        emit ready();
    }
    player1Ready_ = true;
}

void PlayGame::engineActivated2_()
{
    SB_PLAY_DEBUG("PlayGame::engineActivated2_() player1Ready_="<<player1Ready_
                     <<" player2Ready_="<<player2Ready_<<" readySend_="<<readySend_);

    if (!readySend_ && player1Ready_)
    {
        readySend_ = true;
        emit ready();
    }
    player2Ready_ = true;
}


