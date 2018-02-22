/***************************************************************************
 *                           Shatrabase                                    *
 *   (C) 2012-2014 AFKM <francis@afkm.com>                                 *
 *                   derived from ChessX with thanks                       *
 *   (C) 2010 Michal Rudolf <mrudolf@kdewebdev.org>                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "analysis.h"
#include "board.h"

Analysis::Analysis()
{
	clear();
}

Analysis& Analysis::operator=(const Analysis& rhs)
{
    if (this != &rhs)
    {
        m_score     = rhs.m_score;
        m_depth     = rhs.m_depth;
        m_msec      = rhs.m_msec;
        m_resultIn  = rhs.m_resultIn;
        m_rtype     = rhs.m_rtype;
        m_numpv     = rhs.m_numpv;
        m_nodes     = rhs.m_nodes;
        m_nps       = rhs.m_nps;
        m_variation = rhs.m_variation;
    }
    return *this;
}

void Analysis::clear()
{
    m_score = m_msec = m_depth = 0;
    m_resultIn = m_rtype = 0;
    m_nodes = m_nps = 0;
    m_numpv = 1;
	m_variation.clear();
}


bool Analysis::isValid() const
{
	return !m_variation.isEmpty() && m_depth > 0 && m_msec > 0;
}

int Analysis::mpv() const
{
	 return m_numpv;
}

void Analysis::setNumpv(int n)
{
	 m_numpv = n;
}

int Analysis::time() const
{
	return m_msec;
}

void Analysis::setTime(int msec)
{
	m_msec = msec;
}

int Analysis::score() const
{
	return m_score;
}

void Analysis::setScore(int score)
{
	m_score = score;
}

int Analysis::depth() const
{
	return m_depth;
}

void Analysis::setDepth(int depth)
{
	m_depth = depth;
}


quint64 Analysis::nodes() const
{
	return m_nodes;
}

void Analysis::setNodes(quint64 nodes)
{
//    qDebug()<<"setnodes " << nodes;
	m_nodes = nodes;
}

quint64 Analysis::nodesPerSecond() const
{
    return m_nps;
}

void Analysis::setNodesPerSecond(quint64 nodes)
{
//    qDebug()<<"setnodes " << nodes;
    m_nps = nodes;
}

SHATRA::MoveList Analysis::variation() const
{
	return m_variation;
}

void Analysis::setVariation(const SHATRA::MoveList& variation)
{
	m_variation = variation;
}

bool Analysis::isWin() const
{
    return m_resultIn != 0 && m_rtype == SHATRA::Win;
}

bool Analysis::isLoss() const
{
    return m_resultIn != 0 && m_rtype == SHATRA::Loss;
}

bool Analysis::isResult() const
{
    return m_resultIn != 0;
}

int Analysis::movesToResult() const
{
    return m_resultIn;
}

void Analysis::setMovesToResult(int distance, int rtype)
{
    m_resultIn = distance;
    m_rtype = rtype;
}

int Analysis::packResult() const
{
    return (m_rtype<<8) + m_resultIn + 0x40000000;
}	

QString Analysis::toString(const SHATRA::Board& board) const
{
    SHATRA::Board testBoard = board;
	QString out;

    if (isResult()) {
        QString color = isWin()? "000080" : "800000";
        QString text = isWin()? tr("Win in") : isLoss()? tr("Loss in") : "";
		out = QString("<font color=\"#%1\"><b>%2 %3</b></font> ")
                .arg(color).arg(text).arg(movesToResult());
	}
    else if (score() > 0)
		out = QString("<font color=\"#000080\"><b>+%1</b></font> ").arg(score() / 100.0, 0, 'f', 2);
	else out = QString("<font color=\"#800000\"><b>%1</b></font> ").arg(score() / 100.0, 0, 'f', 2);

	int moveNo = testBoard.moveNumber();
    bool white = testBoard.toMove() == SHATRA::White;
    QString moveText;
    SHATRA::Move move;
    for (int i = 0; i < variation().count(); i++) {
        move = variation()[i];
		if (white && !move.wasInSequence())
			moveText += QString::number(moveNo++) + ". ";
		else  if (moveText.isEmpty())
			moveText += QString::number(moveNo++) + "... ";
		moveText += testBoard.moveToLann(move);
		moveText += " ";
		testBoard.doMove(move);
		if (!move.wasInSequence()) white = !white;
	}
    out += " <a href=\""+QString::number(-m_numpv)+"\">[+]</a> ";
    out += " <a href=\""+QString::number(m_numpv)+"\">[*]</a> ";
	out += moveText;
    QTime t(0,0,0,0);
    t = t.addMSecs(time());
    QString elapsed = t.toString(tr("h:mm:ss"));
    out += tr(" (depth %1, %2)").arg(depth()).arg(elapsed);

	return out;
}
