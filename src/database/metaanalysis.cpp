/** @file metaanalysis.cpp

    @brief

    <p>(c) 2014, stefan.berke@modular-audio-graphics.com</p>

    <p>created 5/8/2014</p>
*/

#include "metaanalysis.h"
#include "settings.h"

MetaAnalysis::MetaAnalysis(QObject *parent) :
    QObject(parent)
{
}


bool MetaAnalysis::startAnalysis(const Board& board, int nv,
                   const Engine::SearchSettings & settings)
{
    if (isAnalyzing())
        stopAnalysis();

    // create the main engine
    Engine * e;
    engines_.push_back( e = createEngine_() );

    e->startAnalysis(board, nv, settings);

    return true;
}

void MetaAnalysis::stopAnalysis()
{
    foreach(Engine * e, engines_)
    {
        e->stopAnalysis();
        e->deleteLater();
    }
    engines_.clear();
}


Engine * MetaAnalysis::createEngine_()
{
    Engine * e = Engine::newEngine(0); // XXX
    e->setParent(this);

    //connect(e, SIGNAL(activated()), SLOT(slotEngineActivated()));
    connect(e, SIGNAL(error(QProcess::ProcessError)), SLOT(slotEngineDeactivated_()));
    connect(e, SIGNAL(deactivated()), SLOT(slotEngineDeactivated()));
    connect(e, SIGNAL(analysisUpdated(const Analysis&)),
               SLOT(slotEngineAnalysis_(const Analysis&)));
    /*if (m_engineDebug)
        connect(m_engine, SIGNAL(engineDebug(Engine*,Engine::DebugType,QString)),
            m_engineDebug, SLOT(slotEngineDebug(Engine*,Engine::DebugType,QString)));
    */
    e->activate(true);
    return e;
}

void MetaAnalysis::slotEngineDeactivated_()
{
    Engine * e = qobject_cast<Engine*>(sender());
    if (!e) return;


}

void MetaAnalysis::slotEngineAnalysis_(const Analysis &a)
{
    Engine * e = qobject_cast<Engine*>(sender());
    if (!e) return;

    emit analysisUpdated(a);
}
