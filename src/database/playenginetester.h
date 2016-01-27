/** @file playenginetester.h

    @brief

    <p>(c) 2014, stefan.berke@modular-audio-graphics.com</p>

    <p>created 5/11/2014</p>
*/

#ifndef PLAYENGINETESTER_H
#define PLAYENGINETESTER_H

#include <QObject>
#include "playgameengine.h"
#include "board.h"

class PlayEngineTester : public QObject
{
    Q_OBJECT
public:
    explicit PlayEngineTester(QObject *parent = 0);

signals:

public slots:

    void run();

protected slots:

    void engineReady_();
    void engineMove_(const SHATRA::Move&);
    void engineClueless_();
    void engineCrashed_(const QString&);
    void engineDebug_(Engine*, Engine::DebugType, const QString& msg);

protected:

    PlayGameEngine * engine_[2];
    Engine::SearchSettings settings_;

    SHATRA::Board board_;
};

#endif // PLAYENGINETESTER_H
