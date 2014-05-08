/** @file metaanalysis.cpp

    @brief

    <p>(c) 2014, stefan.berke@modular-audio-graphics.com</p>

    <p>created 5/8/2014</p>
*/

#include "metaanalysis.h"
#include "settings.h"

MetaAnalysis::MetaAnalysis(QObject *parent)
    :    QObject    (parent),
         running_   (false)
{
}


bool MetaAnalysis::activate(int index)
{
    if (isAnalyzing())
        stopAnalysis();

    // create the main engine
    Engine * e = createEngine_(index);
    engines_.push_back( e );

    connect(e, SIGNAL(activated()), SIGNAL(activated()));

    e->activate();

    running_ = true;

    return true;
}

bool MetaAnalysis::startAnalysis(const Board& board, int nv,
                                const Engine::SearchSettings & settings)
{
//    qDebug() << "MetaAnalysis::startAnalysis(...)";

    if (engines_.empty())
        return false;

//    qDebug() << "MetaAnalysis:: engine_[0]->startAnalysis(...)";
    board_ = board;
    initAnalyses_(nv);

    engines_[0]->startAnalysis(board, nv, settings);

    return true;
}

void MetaAnalysis::stopAnalysis()
{
    running_ = false;

    foreach(Engine * e, engines_)
    {
        if (e)
        {
            e->stopAnalysis();
            e->deleteLater();
        }
    }
    engines_.clear();
}


Engine * MetaAnalysis::createEngine_(int index)
{
    Engine * e = Engine::newEngine(index); // XXX
    e->setParent(this);

    connect(e, SIGNAL(error(QProcess::ProcessError)), SLOT(slotEngineDeactivated_()));
    connect(e, SIGNAL(deactivated()), SLOT(slotEngineDeactivated_()));
    connect(e, SIGNAL(analysisUpdated(const Analysis&)),
                 SLOT(slotEngineAnalysis_(const Analysis&)));
    connect(e, SIGNAL(engineDebug(Engine*,Engine::DebugType,QString)),
               SIGNAL(engineDebug(Engine*,Engine::DebugType,QString)));

    return e;
}

void MetaAnalysis::slotEngineDeactivated_()
{
    Engine * e = qobject_cast<Engine*>(sender());
    if (!e) return;

    // first one is main
    if (!engines_.empty() && engines_[0] == e)
        emit deactivated();
}

void MetaAnalysis::slotEngineAnalysis_(const Analysis &a)
{
    Engine * e = qobject_cast<Engine*>(sender());
    if (!e || engines_.empty()) return;

    if (e == engines_[0])
    {
        // ignore out of range mpv
        int n = a.mpv()-1;
        if (n<0 || n>=analyses_.count())
            return;

        // store first time
        if (!analyses_[n].isValid())
            analyses_[n] = a;
        // or update
        else
        {
            //if (analyses_[n].variation()[0]
            //        != a.variation()[0])
        }

        emit analysisUpdated(a);
    }
}


void MetaAnalysis::initAnalyses_(int num)
{
    analyses_.resize(num);
    for (int i=0; i<num; ++i)
        analyses_[i] = Analysis();
    analyses2_.resize(num);
    for (int i=0; i<num; ++i)
        analyses2_[i] = Analysis();
}
