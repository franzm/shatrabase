/***************************************************************************
 *                           Shatrabase                                    *
 *   (C) 2012-2014 AFKM <francis@afkm.com>                                 *
 *                   derived from ChessX with thanks                       *
 *   (C) 2006-2007 Sean Estabrooks                                         *
 *   (C) 2007-2009 Michal Rudolf <mrudolf@kdewebdev.org>                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef __USHIENGINE_H__
#define __USHIENGINE_H__

#include <QString>

#include "engine.h"

/*
  The USHIEngine class will provide an interface to shatra engines.
*/
class USHIEngine : public Engine
{
public:
	/** Constucts an engine with a given path/command, and log stream */
	USHIEngine(const QString& name,
		const QString& command,
        bool bTestMode,
        const QString& directory = QString());

    /** Sets the Shatra version - 1 Original, 2 Extended **/
    void setShatraVersion();

    /** Analyses the the given position.
        If @p movetime_ms > 0, limit the analysis time to given millisecs.
        If @p max_ply > 0, only the specified number of plies will be searched. */
    bool startAnalysis(const Board& board, int nv,
                       const SearchSettings & settings = SearchSettings());

	/** Stops any analysis */
	void stopAnalysis();

	/** Update number of displayed lines. Restarts engine. */
	virtual void setMpv(int mpv);

    virtual bool providesMvp() { return true; }

    void test()
    {
        send("position spn SQSSRSBRB/K/SSSSSSS/7/7/7/7/sssssss/k/brbsrssqs w Tt - - - 1");
        send("go");
    }
protected:
	/** Performs any initialisation required by the engine protocol */
	void protocolStart();

	/** Performs any shutdown procedure required by the engine protocol */
	void protocolEnd();

	/** Processes messages from the engine */
	void processMessage(const QString& message);

private:
	/** Parses analysis */
	void parseAnalysis(const QString& message);

    /** Parse bestmove output */
    void parseBestmove(const QString& message);

    /** Parse option string */
    void parseOptions(const QString &message);

	Board m_board;

	QString m_position;
	QString m_waitingOn;
    SearchSettings m_settings;
	bool m_quitAfterAnalysis;
};

#endif // __USHIENGINE_H__
