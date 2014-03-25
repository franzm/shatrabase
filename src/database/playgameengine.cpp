#include "playgameengine.h"
#include "engine.h"

PlayGameEngine::PlayGameEngine(QObject *parent)
    :   QObject             (parent),
        engine_             (0),
        stopBetweenMoves_   (true),
        listening_          (true)
{
    SB_PLAY_DEBUG("PlayGameEngine::PlayGameEngine()");
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

bool PlayGameEngine::createEngine_()
{
    SB_PLAY_DEBUG("PlayGameEngine::createEngine_()");

    destroyEngine_();

    EngineList elist;
    elist.restore();

    int i = elist.names().indexOf(name_);
    if (i >= 0)
    {
        engine_ = Engine::newEngine(i);

        connect(engine_, SIGNAL(activated()), SLOT(engineActivated_()));
        connect(engine_, SIGNAL(error(QProcess::ProcessError)), SLOT(engineError_(QProcess::ProcessError)));
        connect(engine_, SIGNAL(deactivated()), SLOT(engineDeactivated_()));
        connect(engine_, SIGNAL(analysisUpdated(const Analysis&)),
                                    SLOT(engineAnalysis_(const Analysis&)));
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

    if (engine_)
    {
        engine_->deactivate();
        delete engine_;
        engine_ = 0;
    }
    listening_ = false;
}

void PlayGameEngine::engineActivated_()
{
    SB_PLAY_DEBUG("PlayGameEngine::engineActivated()");
}

void PlayGameEngine::engineDeactivated_()
{
    SB_PLAY_DEBUG("PlayGameEngine::engineDeactivated()");
}

void PlayGameEngine::engineError_(QProcess::ProcessError e)
{
    SB_PLAY_DEBUG("PlayGameEngine::engineError()" << Engine::processErrorText(e));
}

void PlayGameEngine::engineAnalysis_(const Analysis& a)
{
    SB_PLAY_DEBUG("PlayGameEngine::engineAnalysis_()");

    // discard output if not wanted
    if (!listening_)
        return;

    // we got what we wanted
    listening_ = false;

    // send best move
    MoveList m = a.variation();
    if (!m.empty())
    {
        emit moveMade(m[0]);
    }

    if (engine_) /* it should be there though */
    {
        if (stopBetweenMoves_)
            engine_->deactivate();
        else
            engine_->stopAnalysis();
    }
}

bool PlayGameEngine::setPosition(const Board &b)
{
    if (!engine_) return false;

    listening_ = true;
    return engine_->startAnalysis(b, 1);
}

