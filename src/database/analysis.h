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

#ifndef ANALYSIS_H
#define ANALYSIS_H

#include <QtCore>
#include "movelist.h"

namespace SHATRA { class Board; }

/** The analysis class contains engine analysis. */
class Analysis
{
public:
    Analysis();
    /* Reset values. */
    void clear();
    /* Check if analysis is valid. */
    bool isValid() const;
    /* order of the variarion in the variation list */
    int mpv() const;
    /* Set order of the variation. */
    void setNumpv(int n);
    /* Time elapsed in miliseconds. */
    int time() const;
    /* Set time in miliseconds. */
    void setTime(int msec);
    /* Evaluation in centipawns. */
    int score() const;
    /* Set evaluation in centipawns. */
    void setScore(int score);
    /* Depth in plies. */
    int depth() const;
    /* Set depth in plies. */
    void setDepth(int depth);
    /* Nodes spent on analysis. */
    quint64 nodes() const;
    /* Set nodes spent on analysis. */
    void setNodes(quint64 nodes);
    /* Nodes per second spent on analysis. */
    quint64 nodesPerSecond() const;
    /* Set nodes per second spent on analysis. */
    void setNodesPerSecond(quint64 nodes);
    /* Main variation. */
    SHATRA::MoveList variation() const;
    /* Set main variation. */
    void setVariation(const SHATRA::MoveList& variation);
    /* Is win. */
    bool isWin() const;
    /* Is loss. */
    bool isLoss() const;
    /* Is win or loss. */
    bool isResult() const;
    /* @return moves to win or loss */
    int movesToResult() const;
    /* Set moves to win or loss. */
    void setMovesToResult(int distance, int rtype);
    /* Convert analysis to formatted text. */
    int packResult() const;
    /* Win or Loss info packed into score - unpack in PlayGameWidget */
    QString toString(const SHATRA::Board& board) const;
    /* Assignment operator */
    Analysis& operator=(const Analysis& rhs);
private:
    int m_score;
    int m_depth;
    int m_msec;
    int m_resultIn;
    int m_rtype;
    int m_numpv;
    quint64 m_nodes, m_nps;
    SHATRA::MoveList m_variation;
    Q_DECLARE_TR_FUNCTIONS(Analysis)
};

#endif // ANALYSIS_H
