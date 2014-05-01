/** @file ushienginetester.cpp

    @brief

    <p>(c) 2014, stefan.berke@modular-audio-graphics.com</p>

    <p>created 5/1/2014</p>
*/

#include <QDir>

#include "ushienginetester.h"

USHIEngineTester::USHIEngineTester(QObject *parent)
    :   QObject(parent)
{
}

USHIEngineTester::~USHIEngineTester()
{

}




bool USHIEngineTester::startEngine_(int stm)
{
    QProcess * process = new QProcess(this);

    if (!process) return false;

    process->setReadChannel(QProcess::StandardOutput);
    process->setWorkingDirectory(QDir(filename_[stm]).absolutePath());

    connect(process, SIGNAL(started()), SLOT(eStart1_()));
    connect(process, SIGNAL(error(QProcess::ProcessError)), SLOT(eError1_(QProcess::ProcessError)));
    connect(process, SIGNAL(readyReadStandardOutput()), SLOT(ePoll1_()));
    connect(process, SIGNAL(finished(int, QProcess::ExitStatus)), SLOT(eFinish1_()));

    process->start(filename_[stm]);

    return true;
}


void USHIEngineTester::eStart_(int stm)
{

}

void USHIEngineTester::eFinish_(int stm)
{

}

void USHIEngineTester::ePoll_(int stm)
{

}

void USHIEngineTester::eError_(int stm, QProcess::ProcessError)
{

}
