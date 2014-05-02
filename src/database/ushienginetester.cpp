/***************************************************************************
 *                           Shatrabase                                    *
 *   (C) 2012-2014 AFKM <francis@afkm.com>                                 *
 *                   derived from ChessX with thanks                       *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include <iostream>

#include <QDebug>
#include <QDir>
#include <QStringBuilder>

#include "ushienginetester.h"

#include "enginelist.h" // for USHIEngineTester::debugTest()
#include "output.h"


#define SB_PRINT(arg__) \
    { QString str__; QDebug deb__(&str__); deb__ << arg__; \
      std::cout << str__.toStdString() << std::endl; }

#define SB_ET_DEBUG(stm__, arg__) { SB_PRINT( ( (stm__)? " b " : "w " ) << arg__); }

USHIEngineTester::USHIEngineTester(QObject *parent)
    :   QObject(parent)
{
}

USHIEngineTester::~USHIEngineTester()
{

}


bool USHIEngineTester::startTests()
{
    if (!startEngine_(0))
        return false;
    if (!startEngine_(1))
        return false;

    return startGame_(White);
}

void USHIEngineTester::stopTests()
{
    stopEngine_(0);
    stopEngine_(1);
    emit stopped();
}


bool USHIEngineTester::startEngine_(int stm)
{
    QProcess * process = new QProcess(this);

    if (!process) return false;

    process_[stm] = process;

    process->setReadChannel(QProcess::StandardOutput);
    process->setWorkingDirectory(QDir(filename_[stm]).absolutePath());

    if (stm == White)
    {
        connect(process, SIGNAL(started()), SLOT(eStart1_()));
        connect(process, SIGNAL(error(QProcess::ProcessError)), SLOT(eError1_(QProcess::ProcessError)));
        connect(process, SIGNAL(readyReadStandardOutput()), SLOT(ePoll1_()));
        connect(process, SIGNAL(finished(int, QProcess::ExitStatus)), SLOT(eFinish1_()));
    }
    else
    {
        connect(process, SIGNAL(started()), SLOT(eStart2_()));
        connect(process, SIGNAL(error(QProcess::ProcessError)), SLOT(eError2_(QProcess::ProcessError)));
        connect(process, SIGNAL(readyReadStandardOutput()), SLOT(ePoll2_()));
        connect(process, SIGNAL(finished(int, QProcess::ExitStatus)), SLOT(eFinish2_()));
    }

    process->start(filename_[stm]);
    SB_ET_DEBUG(stm, "waiting for start");
    return process->waitForStarted(1000) && process->waitForReadyRead(1000);
}

void USHIEngineTester::stopEngine_(int stm)
{
    if (!process_[stm])
        return;

    send_(stm, "quit");

    SB_ET_DEBUG(stm, "waiting for finished");
    process_[stm]->waitForFinished(1000);
    process_[stm]->deleteLater();
    process_[stm] = 0;
    active_[stm] = 0;
}


void USHIEngineTester::eStart_(int stm)
{
    SB_ET_DEBUG(stm, "started");
    active_[stm] = true;
}

void USHIEngineTester::eFinish_(int stm)
{
    SB_ET_DEBUG(stm, "finished");
    active_[stm] = false;
}

void USHIEngineTester::ePoll_(int stm)
{
    QString message;

    while (process_[stm] && process_[stm]->canReadLine())
    {
        message = process_[stm]->readLine().simplified();

        processMessage_(stm, message);
    }
}

void USHIEngineTester::eError_(int stm, QProcess::ProcessError)
{
    stopEngine_(stm);
}

void USHIEngineTester::send_(int stm, const QString &msg)
{
    if (!process_[stm])
        return;

    process_[stm]->write((msg+"\n").toLatin1());
    process_[stm]->waitForBytesWritten();
    SB_ET_DEBUG(stm, "<--" << msg);
}


bool USHIEngineTester::startGame_(int startstm)
{
    board_.setStandardPosition();
    board_.setToMove((Color)startstm);
    game_.clear();

    // prepare new game for each
    for (int i=0; i<2; ++i)
    {
        send_(i, "ushinewgame");
        send_(i, "setoption name MultiPV value 1");
        send_(i, "setoption name USHI_AnalyseMode value false");
    }

    sendPosition_(board_.toMove(), board_.toSPN());

    return true;
}

void USHIEngineTester::sendPosition_(int stm, const QString &position)
{
//    send_(stm, "stop");
    send_(stm, "ushinewgame");
    send_(stm, "position spn " + position );
    send_(stm, "go movetime 1000");
    //send_(stm, "go depth 8");
}

void USHIEngineTester::processMessage_(int stm, const QString &msg)
{
    SB_ET_DEBUG(stm, "-->" << msg);

    // first word
    QString command = msg.section(' ', 0, 0);

    // on bestmove
    if (command == "bestmove")
    {
        QString info = msg.section(' ', 1, -1, QString::SectionSkipEmpty);
        QString moveText = info.section(' ', 0, 0, QString::SectionSkipEmpty);

        // check move
        Move move = board_.parseMove(moveText);
        if (!move.isLegal())
        {
            SB_ET_DEBUG(stm, "illegal move '"<<moveText<<"'"
                         << " move.stm="<<move.sideMoving());

            game_.setAnnotation(QString("illegal move %1").arg(moveText));

            stopTests();
            return;
        }

        // execute
        board_.doMove(move);
        // XXX hack to make board update correctly
        board_.fromSPN(board_.toSPN());

        // update game
        game_.addMove(moveText);

        // ask opponent (or multicapture)
        sendPosition_(board_.toMove(), board_.toSPN());
    }
}

int USHIEngineTester::debugTest(QApplication & app, QString bin)
{
/*    SBoard b;
    b.setStandardPosition();
    //b.setToMove(Black);
    Move m = b.parseMove("16-23");
    qDebug() << m.isLegal() << m.sideMoving();
    b.doMove(m);
    b.fromSPN(b.toSPN());
    qDebug() << b.toMove();
    m = b.parseMove("46-39");
    qDebug() << m.isLegal() << m.sideMoving();
    return 0;
    SQSSRSBRB/K/1SSSSSS/S6/7/2s4/7/ss1ssss/k/brbsrssqs w Tt - 41 2 2
*/    
//    const QString bin =
//        "/home/defgsus/prog/shatra/sources/build-sdev55-Desktop_Qt_5_1_1_GCC_64bit-Release/sdev55";

    // use defined engine if no override
    if (bin.isEmpty())
    {
        EngineList elist;
        elist.restore();
        bin = elist[0].command;
    }

    qDebug() << "--- TESTING SDEV<->SDEV with " << bin;

    // setup tester class
    USHIEngineTester match;
    match.connect(&match, SIGNAL(stopped()), &app, SLOT(quit()));
    match.setBinary(0, bin);
    match.setBinary(1, bin);

    if (!match.startTests())
    {
        qDebug() << "--- START TEST FAILED";
        return -1;
    }

    // start event loop
    app.exec();

    qDebug() << "--- END";

    // print game
    Output out(Output::Sgn);
    Game g(match.game());
    qDebug() << out.output(&g);

    return 0;
}
