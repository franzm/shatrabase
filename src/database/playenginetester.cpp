/** @file playenginetester.cpp

    @brief

    <p>(c) 2014, stefan.berke@modular-audio-graphics.com</p>

    <p>created 5/11/2014</p>
*/

#include "playenginetester.h"

PlayEngineTester::PlayEngineTester(QObject *parent) :
    QObject(parent)
{
    engine_[0] = new PlayGameEngine(0, this);
    engine_[1] = new PlayGameEngine(0, this);

    connect(engine_[0], SIGNAL(ready()), SLOT(engineReady_()));
    connect(engine_[0], SIGNAL(moveMade(SHATRA::Move)), SLOT(engineMove_(SHATRA::Move)));
    connect(engine_[0], SIGNAL(engineClueless()), SLOT(engineClueless_()));
    connect(engine_[0], SIGNAL(engineCrashed(QString)), SLOT(engineCrashed_(QString)));
    connect(engine_[0], SIGNAL(engineDebug(Engine*,Engine::DebugType,QString)),
            SLOT(engineDebug_(Engine*,Engine::DebugType,QString)));
}


void PlayEngineTester::run()
{
    settings_.maxTime = 2000;
    board_.setStandardPosition();

    engine_[0]->startEngine("Sdev55");
}


void PlayEngineTester::engineReady_()
{
    qDebug() << "ready";
    engine_[0]->setPosition(board_, settings_);
}

void PlayEngineTester::engineMove_(const SHATRA::Move& m)
{
    qDebug() << "move " << m.toNumeric();
}

void PlayEngineTester::engineClueless_()
{
    qDebug() << "clueless";
}

void PlayEngineTester::engineCrashed_(const QString & err)
{
    qDebug() << "** " << err;
}

void PlayEngineTester::engineDebug_(Engine *, Engine::DebugType, const QString &msg)
{
    qDebug() << "### " << msg;
}
