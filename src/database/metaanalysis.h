/** @file metaanalysis.h

    @brief

    <p>(c) 2014, stefan.berke@modular-audio-graphics.com</p>

    <p>created 5/8/2014</p>
*/

#ifndef METAANALYSIS_H
#define METAANALYSIS_H

#include <QObject>
#include <QVector>

#include "engine.h"
#include "board.h"
#include "analysis.h"

class MetaAnalysis : public QObject
{
    Q_OBJECT
public:
    explicit MetaAnalysis(QObject *parent = 0);

    bool isAnalyzing() const { return running_; }

signals:

    void activated();
    void deactivated();

    /** Fired when any engine has produced some analysis */
    void analysisUpdated(const Analysis& analysis);

    void engineDebug(Engine*,Engine::DebugType,QString);

public slots:

    /** Creates the main engine */
    bool activate(int engineIndex);

    bool startAnalysis(const Board& board, int nv = 1,
                       const Engine::SearchSettings & settings = Engine::SearchSettings());

    void stopAnalysis();

protected slots:

    void slotEngineDeactivated_();
    void slotEngineAnalysis_(const Analysis&);

protected:

    Engine * createEngine_(int index);
    void initAnalyses_(int num);

    QVector<Engine*> engines_;
    QVector<Analysis>
            /** pv line for each mpv of main engine */
            analyses_,
            /** single pv for each additional engine */
            analyses2_;
    Board board_;

    bool running_;
};

#endif // METAANALYSIS_H
