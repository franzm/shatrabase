#ifndef PLAYGAMEENGINE_H
#define PLAYGAMEENGINE_H

#include "board.h"
#include "engine.h" // for MoveList

#include <QObject>
#include <QString>
#include <QProcess> // for QProcess::processError
#include <QTime>
#include <QTimer>

#if 0
#   define SB_PLAY_DEBUG(stream_arg__) { qDebug() << (void*)this << stream_arg__; }
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

    /** Stops the Engine if running */
    void stop();

signals:

    /** Engine is activated */
    void ready();

    /** The Engine returned a move */
    void moveMade(Move);

    /** The Engine did not return a move in the maximum time allowed. */
    void engineClueless();

    /** What it says */
    void engineCrashed();

public slots:

    /** Sets new position and queries the Engine.
        Returns true when the Engine is (at least) requested to analyze. */
    bool setPosition(const Board&);

private slots:

    // ---- callbacks from Engine ----

    void engineActivated_();
    void engineDeactivated_();
    void engineError_(QProcess::ProcessError);
    void engineAnalysis_(const Analysis&);

    // ---- other callbacks ----------

    /** Send what's in bestMove_.
        If gotMove_ == false or bestMove_ is empty,
        engineClueless() will be emitted.
        Also stops or deactivates the engine. */
    void sendMoves_();

    // ________ PRIVATE AREA _________________

private:

    bool createEngine_();
    void destroyEngine_();

    /** Will send the board to engine and init the timer */
    bool startAnalysis_(const Board&);

    // __________ PRIVATE MEMBER _____________

    Engine * engine_;

    /** Engines name as in engine list */
    QString name_;

    /** current board to send */
    Board board_;

    /** Best move so far */
    MoveList bestMove_;

    /** For leaving the Engine some time to think */
    QTime waitTimer_;
    /** For forcing the Engine to quit */
    QTimer terminateTimer_;

    /** Time to wait in millisecs for the Engine answer */
    int minWaitTime_,
    /** Time after which the Engine gets stopped and the last move is returned. */
        maxWaitTime_;

    bool
    /** Already got a move from Engine (for maxWaitTime_) */
        gotMove_,
    /** Stop the engine between each move?
        This flag should probably NOT be changed after the engine was activated! */
        stopBetweenMoves_,
    /** Discard any input from Engine if this is false. */
        listening_,
    /** used to send position in stopBetweenMoves mode */
        sendPositionOnActivate_;
};

#endif // PLAYGAMEENGINE_H
