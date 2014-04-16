/***************************************************************************
 *                           Shatrabase                                    *
 *   (C) 2012-2014 AFKM <francis@afkm.com>                                 *
 *                   derived from ChessX with thanks                       *
 *   (C) 2005-2006 William Hoggarth <whoggarth@users.sourceforge.net>      *
 *   (C) 2008-2009 Michal Rudolf <mrudolf@kdewebdev.org>                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "settings.h"
#include "engine.h"
#include "ushiengine.h"

/*** Engine ***/

Engine::Engine(const QString& name,
			 const QString& command,
             bool bTestMode,
			 const QString& directory,
			 QTextStream* logStream)
{
    SB_ENGINE_DEBUG("Engine::Engine("<<name<<","<<command<<","<<bTestMode<<","<<directory<<")");

	m_name = name;
	m_command = command;
    m_bTestMode = bTestMode;
	m_logStream = logStream;
	m_process = 0;
	m_active = false;
	m_analyzing = false;
	m_directory = directory;
}

Engine* Engine::newEngine(int index)
{
    return newEngine(index,false);
}

Engine* Engine::newEngine(EngineList& engineList, int index, bool bTestMode)
{
    //SB_ENGINE_DEBUG("Engine::newEngine("<<index<<","<<bTestMode<<")");

    Engine *engine = 0;

    QString name      = engineList[index].name;
    QString command   = engineList[index].command;
    QString options   = engineList[index].options;
    QString directory = engineList[index].directory;
//    EngineData::EngineProtocol protocol = engineList[index].protocol;

    if (command.contains(' '))
        command = QString("\"%1\"").arg(command);
    QString exe = QString("%1 %2").arg(command).arg(options);

//    if (protocol == EngineData::WinBoard)
//        engine = new WBEngine(name, exe, bTestMode, directory);
//    else
    engine = new USHIEngine(name, exe, bTestMode, directory);

    engine->m_mapOptionValues = engineList[index].m_optionValues;
    return engine;
}

Engine* Engine::newEngine(int index, bool bTestMode)
{
    //SB_ENGINE_DEBUG("Engine::newEngine("<<index<<","<<bTestMode<<")");

	Engine *engine = 0;

    AppSettings->beginGroup("/Engines/");
	QString key(QString::number(index));
	QString name = AppSettings->value(key + "/Name").toString();
	QString command = AppSettings->value(key + "/Command").toString();
	QString options = AppSettings->value(key + "/Options").toString();
	QString directory = AppSettings->value(key + "/Directory").toString();
	QString protocol = AppSettings->value(key + "/Protocol").toString();

	if (command.contains(' '))
		command = QString("\"%1\"").arg(command);
	QString exe = QString("%1 %2").arg(command).arg(options);

//	if (protocol == "WinBoard")
//        engine = new WBEngine(name, exe, bTestMode, directory);
//    else
    engine = new USHIEngine(name, exe, bTestMode, directory);

    AppSettings->getMap(key + "/OptionValues", engine->m_mapOptionValues);
    AppSettings->endGroup();

	return engine;
}

Engine::~Engine()
{
    SB_ENGINE_DEBUG("Engine::~Engine()");

    //deleteProcess_();
}

void Engine::deleteProcess_()
{
    SB_ENGINE_DEBUG("Engine::deleteProcess_()");
    if (m_process)
    {
        m_process->waitForFinished(200);
        m_process->deleteLater();
        m_process = 0;
    }
    m_active = false;
}

void Engine::setLogStream(QTextStream* logStream)
{
	m_logStream = logStream;
}

void Engine::activate(bool do_wait)
{
    SB_ENGINE_DEBUG("Engine::activate("<<do_wait<<")");

    if (m_process)
    {
		return;
	}

    m_process = new QProcess(this);
    if (m_process)
    {
        m_process->setReadChannel(QProcess::StandardOutput);
        if (!m_directory.isEmpty())
            m_process->setWorkingDirectory(m_directory);
        connect(m_process, SIGNAL(started()), SLOT(protocolStart()));
        connect(m_process, SIGNAL(error(QProcess::ProcessError)), SLOT(processError(QProcess::ProcessError)));
        connect(m_process, SIGNAL(readyReadStandardOutput()), SLOT(pollProcess()));
        connect(m_process, SIGNAL(finished(int, QProcess::ExitStatus)), SLOT(processExited()));
        if (do_wait)
            m_process->waitForStarted(1000);
        else
            m_process->start(m_command);
        SB_ENGINE_DEBUG("Engine::activate: process started");
    }
}

void Engine::deactivate()
{
    SB_ENGINE_DEBUG("Engine::deactivate()");

    if (m_active)
    {
        protocolEnd();
        if (m_process)
        {
            SB_ENGINE_DEBUG("Engine::deactivate(): m_process->waitForFinished(...)");
            if (!m_process->waitForFinished(200))
            {
                SB_ENGINE_DEBUG("Engine::deactivate(): m_process->waitForFinished(...) timed out: KILLING NOW");
                deleteProcess_();
            }
            else
                SB_ENGINE_DEBUG("Engine::deactivate(): m_process->waitForFinished(...) ok");
        }
    }
}

bool Engine::isRunning() const
{
    return m_process;
}

bool Engine::isActive() const
{
	return m_active;
}

bool Engine::isAnalyzing() const
{
	return m_analyzing;
}

void Engine::send(const QString& message)
{
    //SB_ENGINE_DEBUG("Engine::send("<<message<<")");

    // debug out
    engineDebug(this, D_ToEngine, message);

	QString out(message);
	out.append('\n');
	if (m_process && !message.isEmpty())
		m_process->write(out.toLatin1());
}

void Engine::setActive(bool active)
{
    if (active && !m_active)
    {
		m_active = true;
		emit activated();
    }
    else
    {
        if (!active && m_active)
        {
			setAnalyzing(false);
			m_active = false;
			emit deactivated();
		}
	}
}

void Engine::setAnalyzing(bool analyzing)
{
    if (analyzing)
    {
		m_analyzing = true;
		emit analysisStarted();
    }
    else
    {
        if (!analyzing && m_analyzing)
        {
			m_analyzing = false;
			emit analysisStopped();
		}
	}
}

void Engine::sendAnalysis(const Analysis& analysis)
{
    SB_ENGINE_DEBUG("Engine::sendAnalysis() variation.size()=" << analysis.variation().size());

	emit analysisUpdated(analysis);
}

void Engine::setMpv(int mpv)
{
	m_mpv = mpv;
}

bool Engine::waitForResponse(int wait_ms)
{
    if (!m_process) return false;

    QTime time;
    time.start();
    while (!m_process->canReadLine())
    {
        QThread::msleep(10);
        if (time.elapsed() > wait_ms)
        {
            engineDebug(this, D_Error, tr("engine timeout after %1 ms").arg(wait_ms));
            return false;
        }
    }
    return true;
}


void Engine::pollProcess()
{
	QString message;

    while (m_process && m_process->canReadLine())
    {
        message = m_process->readLine().simplified();

        // debug out
        //SB_ENGINE_DEBUG("Engine::pollProcess():" << message);
        engineDebug(this, D_FromEngine, message);

        // send to derived class
        processMessage(message);
    }
}

QString Engine::processErrorText(QProcess::ProcessError errMsg)
{
    switch (errMsg)
    {
    case QProcess::FailedToStart:
        return tr("failed to start engine"); break;
    case QProcess::Crashed:
        return tr("the engine has crashed"); break;
    case QProcess::Timedout:
        return tr("the engine timed out"); break;
    case QProcess::WriteError:
        return tr("could not speak to engine"); break;
    case QProcess::ReadError:
        return tr("could not read from engine"); break;
    default:
        return tr("an unknown error occured with engine"); break;
    }
}

void Engine::processError(QProcess::ProcessError errMsg)
{
    SB_ENGINE_DEBUG("Engine::processError("<<errMsg<<")");

    engineDebug(this, D_Error, processErrorText(errMsg));

    setActive(false);
    deleteProcess_();

    emit error(errMsg);
}

void Engine::processExited()
{
    SB_ENGINE_DEBUG("Engine::processExited()");

	setActive(false);
    emit deactivated();

    m_process = 0;
    //deleteProcess_();
}


