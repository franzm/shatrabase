#include "playgameengine.h"
#include "enginedebugwidget.h"
#include "settings.h"

PlayGameEngine::PlayGameEngine(EngineDebugWidget * debug, QObject *parent)
    :   QObject             (parent),
        engineDebug_        (debug),
        engine_             (0),
        minWaitTime_        (500),
        stopBetweenMoves_   (true),
        listening_          (false),
        sendPositionOnReady_(false),
        dontSendReady_      (false)
{
    SB_PLAY_DEBUG("PlayGameEngine::PlayGameEngine()");

    terminateTimer_.setSingleShot(true);
    connect(&terminateTimer_, SIGNAL(timeout()), SLOT(sendMoves_()));
}

PlayGameEngine::~PlayGameEngine()
{
    SB_PLAY_DEBUG("PlayGameEngine::~PlayGameEngine()");
    destroyEngine_();
}


bool PlayGameEngine::startEngine(const QString& name)
{
    SB_PLAY_DEBUG("PlayGameEngine::startEngine("<<name<<")");

    name_ = name;
    return createEngine_();
}

void PlayGameEngine::stop()
{
    SB_PLAY_DEBUG("PlayGameEngine::stop()");

    // XXX Engine::isActive() is stupid! (not clear what to call here)
    if (engine_ && (engine_->isRunning() || engine_->isActive()))
        engine_->deactivate();
}

bool PlayGameEngine::createEngine_()
{
    SB_PLAY_DEBUG("PlayGameEngine::createEngine_()");

    stopBetweenMoves_ = AppSettings->getValue("/PlayGame/restartEngineBetweenMoves").toBool();

    if (engine_)
        destroyEngine_();

    EngineList elist;
    elist.restore();

    int i = elist.names().indexOf(name_);
    if (i >= 0)
    {
        engine_ = Engine::newEngine(i);

        connect(engine_, SIGNAL(activated()), SLOT(engineActivated_()));
        connect(engine_, SIGNAL(readyOk()), SLOT(engineReadyOk_()));
        connect(engine_, SIGNAL(error(QProcess::ProcessError)), SLOT(engineError_(QProcess::ProcessError)));
        connect(engine_, SIGNAL(deactivated()), SLOT(engineDeactivated_()));
        connect(engine_, SIGNAL(analysisUpdated(Analysis)),
                                    SLOT(engineAnalysis_(const Analysis&)));
        connect(engine_, SIGNAL(bestMoveSend(Move)), SLOT(engineBestMove_(Move)));

        // debug
        if (engineDebug_)
        {
            connect(engine_, SIGNAL(engineDebug(Engine*,Engine::DebugType,QString)),
                    engineDebug_, SLOT(slotEngineDebug(Engine*,Engine::DebugType,QString)));
        }
        connect(engine_, SIGNAL(engineDebug(Engine*,Engine::DebugType,QString)),
                         SIGNAL(engineDebug(Engine*,Engine::DebugType,QString)));

        //if (!stopBetweenMoves_)
            engine_->activate();

        SB_PLAY_DEBUG("PlayGameEngine::createEngine_(): created Engine " << name_);
        return true;
    }
    else
    {
        SB_PLAY_DEBUG("PlayGameEngine::createEngine_(): unknown Engine name " << name_);
        return false;
    }
}

void PlayGameEngine::destroyEngine_()
{
    SB_PLAY_DEBUG("PlayGameEngine::destroyEngine_()");

    listening_ = false;
    if (engine_)
    {
        if (engine_->isRunning())
            engine_->deactivate();
        delete engine_;
        engine_ = 0;
    }
}

void PlayGameEngine::engineActivated_()
{
    SB_PLAY_DEBUG("PlayGameEngine::engineActivated_() sendPositionOnReady_="
                    << sendPositionOnReady_);
}

void PlayGameEngine::engineReadyOk_()
{
    SB_PLAY_DEBUG("PlayGameEngine::engineReadyOk_() sendPositionOnReady_="
                    << sendPositionOnReady_);

    // keep right going
    if (sendPositionOnReady_)
    {
        startAnalysis_(board_);

        sendPositionOnReady_ = false;
    }

    if (!dontSendReady_)
        emit ready();
    else
        dontSendReady_ = false;

}

void PlayGameEngine::engineDeactivated_()
{
    SB_PLAY_DEBUG("PlayGameEngine::engineDeactivated()");
}

void PlayGameEngine::engineError_(QProcess::ProcessError e)
{
    Q_UNUSED(e);
    SB_PLAY_DEBUG("PlayGameEngine::engineError()" << Engine::processErrorText(e));
    emit engineCrashed(Engine::processErrorText(e));
}

void PlayGameEngine::engineBestMove_(const Move& m)
{
    SB_PLAY_DEBUG("PlayGameEngine::engineBestMove_()");

    if (!listening_)
        return;

    terminateTimer_.stop();

    bestMove_ = m;
    gotBestMove_ = true;

    sendMoves_();
}

void PlayGameEngine::engineAnalysis_(const Analysis& a)
{
    SB_PLAY_DEBUG("PlayGameEngine::engineAnalysis_()");

    if (!a.variation().empty())
        emit moveInfo(a.variation()[0], a.score());

    /*
    // discard output if not wanted
    if (!listening_)
        return;

    // keep the mainline
    bestMoves_ = a.variation();
    gotMove_ = true;

    // leave the Engine some time
    if (waitTimer_.elapsed() < minWaitTime_)
        return;

    // send best move (and stop engine)
    sendMoves_();
    */
}

void PlayGameEngine::sendMoves_()
{
    // thanks, we got what we wanted
    listening_ = false;

    terminateTimer_.stop();

    if (engine_) /* it should be there though */
    {
        if (stopBetweenMoves_)
            engine_->deactivate();
        else
            engine_->stopAnalysis();
    }

    if (!(gotMove_ || gotBestMove_))
        emit engineClueless();
    else
    {
        if (gotBestMove_)
            emit moveMade(bestMove_);
        else
        if (!bestMoves_.empty())
        {
            emit moveMade(bestMoves_[0]);

            // multi-ply?
            int stm = bestMoves_[0].sideMoving();
            int i = 1;
            while (i < bestMoves_.size() && bestMoves_[i].sideMoving() == stm)
            {
                emit moveMade(bestMoves_[i]);
                ++i;
            }
        }
        else
            emit engineClueless();
    }

}

bool PlayGameEngine::setPosition(const Board &b, const Engine::SearchSettings& settings)
{
    SB_PLAY_DEBUG("PlayGameEngine::setPosition() stopBetweenMoves_=" << stopBetweenMoves_);

    if (!engine_)
    {
        qDebug() << "*** PlayGameEngine::setPosition(..) without engine_";
        return false;
    }

    settings_ = settings;

    if (stopBetweenMoves_)
    {
        dontSendReady_ = true;

        if (engine_->isRunning())
            engine_->deactivate();

        sendPositionOnReady_ = true;
        board_ = b;

        engine_->activate();
        return true; // best guess
    }

    return startAnalysis_(b);
}


bool PlayGameEngine::startAnalysis_(const Board& b)
{
    SB_PLAY_DEBUG("PlayGameEngine::startAnalysis_(...) engine_="<<engine_);

    if (!engine_)
    {
        qDebug() << "*** PlayGameEngine::startAnalysis_(..) without engine_";
        return false;
    }

    listening_ = true;
    gotMove_ = gotBestMove_ = false;
    waitTimer_.start();

    /*
    if (settings_.maxTime != Engine::SearchSettings::Unlimited)
    {
        minWaitTime_ = settings_.maxTime;
        //terminateTimer_.start(minWaitTime_+200);
    }*/

    return engine_->startAnalysis(b, 1, settings_);
}

