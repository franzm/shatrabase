#ifndef PLAYGAMEENGINE_H
#define PLAYGAMEENGINE_H

#include "board.h"

#include <QObject>
#include <QString>
#include <QProcess> // for QProcess::processError

#if 1
#   define SB_PLAY_DEBUG(stream_arg__) { qDebug() << stream_arg__; }
#else
#   define SB_PLAY_DEBUG(unused__) { }
#endif

class Analysis;
class Engine;


/** A class that wraps an Engine to play a game.
*/
class PlayGameEngine : public QObject
{
    Q_OBJECT
public:
    explicit PlayGameEngine(QObject *parent = 0);

    ~PlayGameEngine();

    /** Starts a new Engine. @p name is the name in the Engine list. */
    bool startEngine(const QString& name);

signals:

    /** The Engine returned a move */
    void moveMade(Move);

public slots:

    /** Sets new position and queries the Engine.
        Returns true when the Engine is (at least) requested to analyze. */
    bool setPosition(const Board&);

protected slots:

    // ---- callbacks from Engine ----

    void engineActivated_();
    void engineDeactivated_();
    void engineError_(QProcess::ProcessError);
    void engineAnalysis_(const Analysis&);

    // ________ PRIVATE AREA _________________

private:

    bool createEngine_();
    void destroyEngine_();

    // __________ PRIVATE MEMBER _____________

    Engine * engine_;

    /** Engines name as in engine list */
    QString name_;

    bool
    /** Stop the engine between each move? */
        stopBetweenMoves_,
    /** Discard any input from Engine if this is false. */
        listening_;
};

#endif // PLAYGAMEENGINE_H
