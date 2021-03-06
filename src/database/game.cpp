/***************************************************************************
 *                           Shatrabase                                    *
 *   (C) 2012-2014 AFKM <francis@afkm.com>                                 *
 *                   derived from ChessX with thanks                       *
 *   (C) 2005-2006 Marius Roets <roets.marius@gmail.com>                   *
 *   (C) 2005-2009 Michal Rudolf <mrudolf@kdewebdev.org>                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include <QtDebug>
#include <QFile>
#include "game.h"


Game::Game()
{
    clearTags();
    clear();
    setModified(false);
}

Game::Game(const Game& game)
{
    *this = game;
    setModified(false);
    m_gameComment = "";
}

Game& Game::operator=(const Game & game)
{
    if (this != &game)
    {
        clearTags();
        clear();
        //assign non pointer variables
        m_tags = game.m_tags;
        m_startingBoard = game.m_startingBoard;
        m_variationStartAnnotations = game.m_variationStartAnnotations;
        m_annotations = game.m_annotations;
        m_squareAnnotations = game.m_squareAnnotations;
        m_arrowAnnotations = game.m_arrowAnnotations;
        m_clkAnnotations = game.m_clkAnnotations;

        m_currentNode = game.m_currentNode;
        m_startPly = game.m_startPly;
        m_currentBoard = game.m_currentBoard;
        m_gameComment = game.m_gameComment;

        m_moveNodes = game.m_moveNodes;

        setModified(true);
    }
    return *this;
}

Game::~Game()
{
}

MoveId Game::addMove(const SHATRA::Move& move, const QString& annotation, NagSet nags)
{
    MoveNode node;
    MoveId previousNode = m_currentNode;

    node.nextNode = NO_MOVE;
    node.previousNode = m_currentNode;
    node.parentNode = m_moveNodes[m_currentNode].parentNode;
    node.move = move;
    node.nags = nags;
    node.ply = ply() + 1;
    node.stm = m_currentBoard.toMove();
    m_moveNodes.append(node);
    m_currentNode = m_moveNodes.size() - 1;
    if (!annotation.isEmpty())
        setAnnotation(annotation);
    m_moveNodes[previousNode].nextNode = m_currentNode;
    m_currentBoard.doMove(move);
    setModified(true);

    return m_currentNode;
}

MoveId Game::addMove(const QString& lannMove, const QString& annotation, NagSet nags)
{
    SHATRA::Move move = m_currentBoard.parseMove(lannMove);
    if (move.isLegal())
        return addMove(move, annotation, nags);
    return NO_MOVE;
}

bool Game::currentNodeHasVariation(SHATRA::Square from, SHATRA::Square to) const
{
    if (m_currentNode == NO_MOVE) return false;

    QList<MoveId> vs = m_moveNodes[m_currentNode].variations;
    QList<MoveId>::iterator i;
    for (i = vs.begin(); i != vs.end(); ++i)
    {
        SHATRA::Move m = move(*i);
        if( m.from() == from && m.to() == to )
        {
            return true;
        }
    }
    return false;
}

bool Game::currentNodeHasMove(SHATRA::Square from, SHATRA::Square to) const
{
    if (currentNodeHasVariation(from,to))
    {
        return true;
    }
    int node;
    node = m_moveNodes[m_currentNode].nextNode;
    if (node == NO_MOVE)
        return true;
    SHATRA::Move m = m_moveNodes[node].move ;
    if( m.from() == from && m.to() == to )
    {
        return (m_moveNodes[node].nextNode != NO_MOVE);
    }
    return false;
}

// does the next main move or one of the variations go from square from to square to
// if so make it on the board
bool Game::findNextMove(SHATRA::Square from, SHATRA::Square to,
                        SHATRA::PieceType promotionPiece)
{
    int node;
    node = m_moveNodes[m_currentNode].nextNode;
    if( node != NO_MOVE ) {
        SHATRA::Move m = m_moveNodes[node].move ;
        if( m.from() == from && m.to() == to &&
                ((promotionPiece == SHATRA::NoPiece) ||
                    ((m.isPromotion() && (pieceType( m.promotedPiece()) == promotionPiece)))))
        {
            forward();
            return true;
        }
        else
        {
            QList<MoveId> vs = m_moveNodes[m_currentNode].variations;
            QList<MoveId>::iterator i;
            for (i = vs.begin(); i != vs.end(); ++i)
            {
                SHATRA::Move m = move(*i);
                if( m.from() == from && m.to() == to &&
                        ((promotionPiece == SHATRA::NoPiece) || ((m.isPromotion()
                            && (pieceType( m.promotedPiece()) == promotionPiece)))))
                {
                    moveToId(*i);
                    return true;
                }
            }
         }
    }
    return false;
}

void Game::getAllPlies(SHATRA::MoveList& moves)
{
    moves.clear();
    for (int i=0; i<plyCount(); ++i)
    {
        SHATRA::Move m = move(i+1);
        if (m.isLegal())
            moves.append(m);
    }
}

bool Game::replaceMove(const SHATRA::Move& move, const QString& annotation, NagSet nags)
{
    int node;
    node = m_moveNodes[m_currentNode].nextNode;
    setModified(true);
    if (node == NO_MOVE) {
        addMove(move, annotation, nags);
        return true;
    }

    //replace node data with new move
    m_moveNodes[node].move = move;
    m_moveNodes[node].nags = nags;
    setAnnotation(annotation, node);

    //remove any following nodes after replaced move by disconnecting them from the tree
    forward();
    truncateVariation();
    backward();

    return true;
}

bool Game::replaceMove(const QString& lannMove, const QString& annotation, NagSet nags)
{
    return replaceMove(m_currentBoard.parseMove(lannMove), annotation, nags);
}

MoveId Game::addVariation(const SHATRA::Move& move, const QString& annotation, NagSet nags)
{
    MoveId previousNode = m_currentNode;
    MoveId saveNextNode = m_moveNodes[m_currentNode].nextNode;
    MoveId node = addMove(move, annotation, nags);
    m_moveNodes[m_currentNode].parentNode = previousNode;
    m_moveNodes[previousNode].variations.append(node);
    m_moveNodes[previousNode].nextNode = saveNextNode;

    return (m_moveNodes.size() - 1);
}

MoveId Game::addVariation(const SHATRA::MoveList& moveList, const QString& annotation)
{
    if (moveList.isEmpty())
        return NO_MOVE;
    MoveId currentPosition = currentMove();
    MoveId varStart;
    int start = 1;
    if (!atLineEnd()) {
        varStart = addVariation(moveList.first());
    }
    else if (!atGameStart()) {
        SHATRA::Move oldMove = move();
        backward();
        varStart = addVariation(oldMove);
        start = 0;
    }
    else varStart = addMove(moveList.first());
    moveToId(varStart);
    for (int i = start; i < moveList.count(); ++i) {
        addMove(moveList[i]);
        forward();
    }
    if (!annotation.isEmpty())
        setAnnotation(annotation);
    moveToId(currentPosition);
    return varStart;
}

MoveId Game::addVariation(const QString& lannMove, const QString& annotation, NagSet nags)
{
    MoveId previousNode = m_currentNode;
    MoveId saveNextNode = m_moveNodes[m_currentNode].nextNode;
    MoveId node = addMove(lannMove, annotation, nags);
    if (node == NO_MOVE) return node;

    m_moveNodes[m_currentNode].parentNode = previousNode;
    m_moveNodes[previousNode].variations.append(node);
    m_moveNodes[previousNode].nextNode = saveNextNode;

    return (m_moveNodes.size() - 1);
}

bool Game::promoteVariation(MoveId variation)
{
    if (isMainline(variation))
        return false;

    MoveId currentNode = m_currentNode; // Save current move

    // Find first move of the variation
    while (!atLineStart(variation))
        variation = m_moveNodes[variation].previousNode;
    MoveId parent = m_moveNodes[variation].parentNode;

    // Link old main line to parent 
    reparentVariation(m_moveNodes[parent].nextNode, parent);
    // Link new main line to parent's parent
    reparentVariation(variation, m_moveNodes[parent].parentNode);

    // Swap main line and the variation
    int index = m_moveNodes[parent].variations.indexOf(variation);
    qSwap(m_moveNodes[parent].nextNode, m_moveNodes[parent].variations[index]);

    moveToId(currentNode);  // Restore current move
    return true;
}

bool Game::removeVariation(MoveId variation)
{
    // don't remove whole game
    if (!variation)
        return false;
    MoveId parentNode = m_moveNodes[variation].parentNode;
    removeNode(variation);
    moveToId(parentNode);

    QList<MoveId> &vars = m_moveNodes[m_currentNode].variations;
    vars.removeAt(vars.indexOf(variation));

    compact();
    return true;
}

void Game::truncateVariation(Position position)
{
    if (position == AfterMove) {
        int node = m_moveNodes[m_currentNode].nextNode;
        removeNode(node);
    }
    else if (position == BeforeMove && m_currentNode != 0)
    {
        if (atLineStart())
        {
            backward();
            forward();
        }
        MoveId current = m_currentNode;
        MoveNode firstNode;
        firstNode.nextNode = m_currentNode;
        firstNode.ply = m_moveNodes[m_currentNode].ply - 1;
        // Keep variation if truncating main line
        if (m_moveNodes[m_moveNodes[m_currentNode].previousNode].nextNode == m_currentNode) {
            firstNode.variations = m_moveNodes[m_moveNodes[m_currentNode].previousNode].variations;
            foreach(MoveId var, firstNode.variations) {
                reparentVariation(var, 0);
                m_moveNodes[var].previousNode = 0;
            }
        }
        m_moveNodes[0] = firstNode;
        m_moveNodes[m_currentNode].previousNode = 0;
        backward();
        m_startingBoard = m_currentBoard;
        if (m_startingBoard != SHATRA::standardStartBoard)
        {
            m_tags[SHATRA::TagNameSPN] = m_startingBoard.toSPN();
            m_tags[SHATRA::TagNameSetUp] = "1";
        }
        moveToId(current);
    }
    compact();
}

const SHATRA::Board& Game::board() const
{
    return m_currentBoard;
}

SHATRA::Board Game::startingBoard() const
{
    return m_startingBoard;
}

QString Game::toSPN() const
{
    return m_currentBoard.toSPN();
}

QString Game::gameComment() const
{
    return m_gameComment;
}

void Game::setGameComment(const QString& gameComment)
{
    m_gameComment = gameComment;
}

void Game::removeVariations()
{
    for (int i = 0; i < m_moveNodes.size(); ++i)
    {
        m_moveNodes[i].variations.clear();
    }
    compact();
    setModified(true);
}

void Game::removeComments()
{
    m_gameComment.clear();
    m_variationStartAnnotations.clear();
    m_annotations.clear();
    m_squareAnnotations.clear();
    m_arrowAnnotations.clear();
    m_clkAnnotations.clear();
    for (int i = 0; i < m_moveNodes.size(); ++i)
    {
        m_moveNodes[i].nags.clear();
    }
    compact();
    setModified(true);
}

bool Game::isMainline(MoveId moveId) const
{
    if (moveId == 0) return true;
    MoveId node = nodeValid(moveId);
    if (node == NO_MOVE)
        return false;
    else
        return m_moveNodes[node].parentNode == NO_MOVE;
}

SHATRA::Result Game::result() const
{
    if (m_tags["Result"] == "1-0") {
        return SHATRA::WhiteWin;
    } else if (m_tags["Result"] == "1/2-1/2") {
        return SHATRA::Draw;
    } else if (m_tags["Result"] == "0-1") {
        return SHATRA::BlackWin;
    } else {
        return SHATRA::ResultUnknown;
    }
}

int Game::numberOfSiblings(MoveId moveId) const
{
    MoveId node = nodeValid(moveId);
    if (node == NO_MOVE)
        return 0;
    MoveId parentNode = m_moveNodes[node].parentNode;
    if (parentNode == NO_MOVE)
        return 0;
    return m_moveNodes[parentNode].variations.size();
}

bool Game::atLineStart(MoveId moveId) const
{
    MoveId node = nodeValid(moveId);
    if (node == NO_MOVE) 
        return false;
    return (m_moveNodes[node].previousNode == m_moveNodes[node].parentNode)
        || m_moveNodes[node].previousNode == 0;
}

bool Game::atGameStart(MoveId moveId) const
{
    return (nodeValid(moveId) == 0);
}

bool Game::atGameEnd(MoveId moveId) const
{
    return (atLineEnd(moveId) && isMainline(moveId));
}

bool Game::atLineEnd(MoveId moveId) const
{
    MoveId node = nodeValid(moveId);
    if (node != NO_MOVE) {
        if (m_moveNodes[node].nextNode == NO_MOVE) {
            return true;
        }
    }

    return false;
}

bool Game::setAnnotation(QString annotation, MoveId moveId, Position position)
{
    MoveId node = nodeValid(moveId);
    if (node == NO_MOVE)
        return false;

    setModified(true);

    QString specAnnot = specialAnnotation(annotation, "[%csl");
    if( position == Game::AfterMove || node == 0 )
    {
        setSquareAnnotation(specAnnot,node);
    }
    else
    {
        setSquareAnnotation(specAnnot,m_moveNodes[node].previousNode);
    }

    specAnnot = specialAnnotation(annotation, "[%cal");
    if( position == Game::AfterMove || node == 0)
    {
        setArrowAnnotation(specAnnot,node);
    }
    else
    {
        setArrowAnnotation(specAnnot,m_moveNodes[node].previousNode);
    }

    specAnnot = specialAnnotation(annotation, "[%clk");
    if( position == Game::AfterMove || node == 0)
    {
        setClkAnnotation(specAnnot,node);
    }
    else
    {
        setClkAnnotation(specAnnot,m_moveNodes[node].previousNode);
    }

    int moves;
    int comments;
    int nags;
    moveCount(&moves,&comments,&nags);
    if( moves <= 0 )
    {
        setGameComment(annotation);
        return true;
    }

    if (position == AfterMove) {
        if (annotation.isEmpty())
            m_annotations.remove(node);
        else m_annotations[node] = annotation;
    } 
    else if (canHaveStartAnnotation(node)) {    // Pre-move comment
        if (annotation.isEmpty())
            m_variationStartAnnotations.remove(node);
        else m_variationStartAnnotations[node] = annotation;
    }
    return true;
}

bool Game::setSquareAnnotation(QString squareAnnotation, MoveId node)
{

        if (squareAnnotation.isEmpty())
            m_squareAnnotations.remove(node);
        else m_squareAnnotations[node] = squareAnnotation;
    return true;
}

bool Game::appendSquareAnnotation(SHATRA::Square s, QChar colorCode)
{
    QString newAnnot;
    QString annot = squareAnnotation();
    QString sq = SHATRA::strSquareNames[s];
    if (annot.isEmpty())
    {
        if (colorCode != QChar(0))
        {
            newAnnot = QString("[%csl %1%2]").arg(colorCode).arg(sq);
        }
    }
    else
    {
        annot.replace(QRegExp(QString(".")+sq),"");
        if (colorCode != QChar(0))
        {
            newAnnot = QString("[%csl %1,%2%3]").arg(annot).arg(colorCode).arg(sq);
        }
        else
        {
            if (!annot.isEmpty())
            {
                newAnnot = QString("[%csl %1]").arg(annot);
            }
        }
        newAnnot.replace(" ,"," ");
        newAnnot.replace(",,",",");
        newAnnot.replace(",]","]");
    }
    setAnnotation(newAnnot);
    return true;
}

QString Game::squareAnnotation(MoveId moveId) const
{
    MoveId node = nodeValid(moveId);
    if (node == NO_MOVE)
        return "";

    QString annotation = m_squareAnnotations[node];
    if(annotation.isNull()) {
        return "";
    }
    return m_squareAnnotations[node];
}

bool Game::setArrowAnnotation(QString arrowAnnotation, MoveId node)
{
    if (arrowAnnotation.isEmpty())
        m_arrowAnnotations.remove(node);
    else
        m_arrowAnnotations[node] = arrowAnnotation;
    return true;
}

QString Game::arrowAnnotation(MoveId moveId) const
{
    MoveId node = nodeValid(moveId);
    if (node == NO_MOVE)
        return "";

    QString annotation = m_arrowAnnotations[node];
    if(annotation.isNull()) {
        return "";
    }
    return m_arrowAnnotations[node];
}

QString Game::timeAnnotation(MoveId moveId) const
{
    QString s = clkAnnotation(moveId);
    return s;
}

bool Game::setClkAnnotation(QString annotation, MoveId node)
{
    if (annotation.isEmpty())
        m_clkAnnotations.remove(node);
    else
        m_clkAnnotations[node] = annotation;
    return true;
}

QString Game::clkAnnotation(MoveId moveId) const
{
    MoveId node = nodeValid(moveId);
    if (node == NO_MOVE)
        return "";

    QString annotation = m_clkAnnotations[node];
    if(annotation.isNull()) {
        return "";
    }
    return m_clkAnnotations[node];
}

QString Game::annotation(MoveId moveId, Position position) const
{
    MoveId node = nodeValid(moveId);
    if (position == AfterMove)
        return m_annotations.value(node, "");
    else 
        return m_variationStartAnnotations.value(node, "");
}

bool Game::canHaveStartAnnotation(MoveId moveId) const
{
    MoveId node = nodeValid(moveId);
    return atLineStart(moveId) || atGameStart(m_moveNodes[node].previousNode);
}

bool Game::addNag(Nag nag, MoveId moveId)
{
    MoveId node = nodeValid(moveId);
    if (node != NO_MOVE) {
        m_moveNodes[node].nags.addNag(nag);
        setModified(true);
        return true;
    }
    return false;
}

bool Game::setNags(NagSet nags, MoveId moveId)
{
    MoveId node = nodeValid(moveId);
    if (node != NO_MOVE) {
        m_moveNodes[node].nags = nags;
        setModified(true);
        return true;
    }
    return false;
}

bool Game::clearNags(MoveId moveId)
{
    return setNags(NagSet(),moveId);
}

NagSet Game::nags(MoveId moveId) const
{
    MoveId node = nodeValid(moveId);
    if (node != NO_MOVE) {
        return m_moveNodes[node].nags;
    }
    return NagSet();
}

MoveId Game::nodeValid(MoveId moveId) const
{
    if (moveId == CURRENT_MOVE) {
        moveId = m_currentNode;
    }
    if ((moveId >= 0) && (moveId < m_moveNodes.size())) {
        if (m_moveNodes[moveId].removed) {
            return NO_MOVE;
        }
        return moveId;
    }
    return NO_MOVE;
}


int Game::moveCount() const
{
    if (nodeValid(1) == NO_MOVE)
        return 0;

    int moves = 1, node = 1, bstm = -1,
        // previous side to move
        pstm = -1;
    while (nodeValid(node) != NO_MOVE)
    {
        // current side to move
        int cstm = m_moveNodes[node].stm;

        // side to start
        if (bstm==-1)
            bstm = cstm;

        node = m_moveNodes[node].nextNode;

        // player changed
        if (cstm != pstm)
        {
            // if back to start color and not first move
            if (pstm != -1 && cstm == bstm)
                moves ++;
            pstm = cstm;
        }

    }
    return moves;
}

void Game::moveCount(int* moves, int* comments, int* nags) const
{
    *moves = *comments = *nags = 0;

    int node = 1;
    while (nodeValid(node) != NO_MOVE)
    {
        *moves += 1;
        if (m_moveNodes[node].nags.count() != 0) {
            *nags += 1;
        }
        node = m_moveNodes[node].nextNode;
    }

    // Count comments
    for (int i = 0; i < m_annotations.size(); ++i) *comments += 1;
    for (int i = 0; i < m_variationStartAnnotations.size(); ++i) *comments += 1;

}

int Game::ply(MoveId moveId) const
{
    MoveId node = nodeValid(moveId);
    if (node != NO_MOVE)
    {
        return m_moveNodes[node].ply;
    }
    return 0;
}

int Game::moveNumber(MoveId moveId) const
{
    MoveId node = nodeValid(moveId);
    if (node != NO_MOVE)
    {
//        if (int plyNum = ply(node))
//            return (m_startPly + plyNum - 1) / 2 + 1;
// kluge time, will adjust for ... in output.cpp
        if (ply(node)) // necessary?
        {
            return m_currentBoard.moveNumber();
        }
        return 0;
    }
    return -1;
}


int Game::plyCount() const
{
    int count = 0;
    int node = 0;

    while (node != NO_MOVE)
    {
        ++count;
        node = m_moveNodes[node].nextNode;
    }
    // Counted one too much, because we have to start at zero
    // (an empty game has no node 1)
    return count - 1;
}

bool Game::canMoveVariationUp(MoveId moveId) const
{
    if (isMainline()) return false;

    MoveId variation = variationNumber(moveId);
    MoveId parentNode = m_moveNodes[moveId].parentNode;

    const QList <MoveId>& v = m_moveNodes[parentNode].variations;
    int i = v.indexOf(variation);
    return (i > 0);
}

void Game::moveVariationUp(MoveId moveId)
{
    if (isMainline()) return;

    MoveId variation = variationNumber(moveId);
    MoveId parentNode = m_moveNodes[moveId].parentNode;

    QList <MoveId>& v = m_moveNodes[parentNode].variations;
    int i = v.indexOf(variation);
    if (i > 0)
    {
        v.swap(i, i-1);
        setModified(true);
    }
}

bool Game::canMoveVariationDown(MoveId moveId) const
{
    if (isMainline()) return false;

    MoveId variation = variationNumber(moveId);
    MoveId parentNode = m_moveNodes[moveId].parentNode;

    const QList <MoveId>& v = m_moveNodes[parentNode].variations;
    int i = v.indexOf(variation);
    return (i>=0 && (i+1) < v.count());
}

void Game::moveVariationDown(MoveId moveId)
{
    if (isMainline()) return;

    MoveId variation = variationNumber(moveId);
    MoveId parentNode = m_moveNodes[moveId].parentNode;

    QList <MoveId>& v = m_moveNodes[parentNode].variations;
    int i = v.indexOf(variation);
    if (i>=0 && (i+1) < v.count())
    {
        v.swap(i, i+1);
        setModified(true);
    }
}

MoveId Game::variationNumber(MoveId moveId) const
{
    if (isMainline()) return 0;
    MoveId node = nodeValid(moveId);
    MoveId parentNode = m_moveNodes[node].parentNode;

    while (m_moveNodes[node].previousNode != parentNode)
    {
        node = m_moveNodes[node].previousNode;
    }
    return node;
}

const QList<MoveId>& Game::variations() const
{
    return m_moveNodes[m_currentNode].variations;
}

int Game::variationCount(MoveId moveId) const
{
    MoveId node = nodeValid(moveId);
    if (node != NO_MOVE) {
        return m_moveNodes[node].variations.size();
    }
    return 0;
}

void Game::indicateAnnotationsOnBoard(MoveId moveId)
{
    QString annotation = squareAnnotation(moveId);
    m_currentBoard.setSquareAnnotation(annotation);

    annotation = arrowAnnotation(moveId);
    m_currentBoard.setArrowAnnotation(annotation);
}

void Game::moveToStart()
{
    m_currentNode = 0;
    m_currentBoard = m_startingBoard;

    indicateAnnotationsOnBoard(m_currentNode);
}

int Game::moveByPly(int diff)
{
    if (diff > 0) {
        return forward(diff);
    } else {
        return backward(-diff);
    }
}

void Game::moveToId(MoveId moveId)
{
    if (nodeValid(moveId) == NO_MOVE)
        return;

    //jump to node, travelling back to start adding the moves to the stack
    int node = moveId;
    QStack<SHATRA::Move> moveStack;
    while (node)
    {
        moveStack.push(m_moveNodes[node].move);
        node = m_moveNodes[node].previousNode;
    }

    //reset the board, then make the moves on the stack to create the correct position
    m_currentNode = moveId;
    m_currentBoard = m_startingBoard;
    while (!moveStack.isEmpty()) {
        m_currentBoard.doMove(moveStack.pop());
    }

    indicateAnnotationsOnBoard(moveId);
}

int Game::moveToPly(int ply)
{
    if (ply == 0) {
        moveToStart();
        return 0;
    }

    int currentPly = this->ply();
    int diff = ply - currentPly;

    if (diff > 0) {
        return currentPly + forward(diff);
    } else {
        return currentPly - backward(-diff);
    }
}

SHATRA::Move Game::move(MoveId moveId) const
{
    MoveId node = nodeValid(moveId);
    if (node != NO_MOVE) {
        return m_moveNodes[node].move;
    }
    return SHATRA::Move();
}

void Game::moveToEnd()
{
    // Move out of variations to mainline
    while (m_moveNodes[m_currentNode].parentNode != NO_MOVE) {
        moveToId(m_moveNodes[m_currentNode].parentNode);
    }
    // Now move forward to the end of the game
    while (m_moveNodes[m_currentNode].nextNode != NO_MOVE) {
        forward(1);
    }
}

int Game::forward(int count)
{
    int moved = 0;
    while ((m_moveNodes[m_currentNode].nextNode != NO_MOVE) && (moved < count))
    {
        m_currentNode = m_moveNodes[m_currentNode].nextNode;
        m_currentBoard.doMove(m_moveNodes[m_currentNode].move);
        ++moved;
    }

    indicateAnnotationsOnBoard(m_currentNode);

    return moved;
}

int Game::backward(int count)
{
    int moved = 0;
    if (SHATRA::g_loading)
    {
        while ((m_moveNodes[m_currentNode].previousNode >= 0) && (moved < count))
        {
            m_currentBoard.undoMove(m_moveNodes[m_currentNode].move);
            m_currentNode = m_moveNodes[m_currentNode].previousNode;
            ++moved;
        }
    }
    else
    {
        while ((m_moveNodes[m_currentNode].previousNode >= 0) && (moved < count))
        {
            m_currentNode = m_moveNodes[m_currentNode].previousNode;
            ++moved;
        }
        moveToId(m_currentNode);
    }

    indicateAnnotationsOnBoard(m_currentNode);

    return moved;
}

void Game::enterVariation(const MoveId& moveId)
{
    Q_ASSERT(variations().contains(moveId));
    m_currentBoard.doMove(m_moveNodes[moveId].move);
    m_currentNode = moveId;

    indicateAnnotationsOnBoard(m_currentNode);
}

void Game::removeNode(MoveId moveId)
{
    MoveId node = nodeValid(moveId);
    if (node != NO_MOVE) {
        setModified(true);

        if (variationCount(node)) {
            for (int i = 0; i < m_moveNodes[node].variations.size(); ++i) {
                removeNode(m_moveNodes[node].variations[i]);
            }
        }
        removeNode(m_moveNodes[node].nextNode);
        if (!atLineStart(node)) {
            m_moveNodes[m_moveNodes[node].previousNode].nextNode = NO_MOVE;
        }
        m_moveNodes[node].remove();
    }
}

void Game::clear()
{
    m_gameComment.clear();

    m_moveNodes.clear();
    m_variationStartAnnotations.clear();
    m_annotations.clear();
    m_squareAnnotations.clear();
    m_arrowAnnotations.clear();
    m_clkAnnotations.clear();

    m_startPly = 0;
    m_currentNode = 0;

    m_startingBoard.setStandardPosition();
    m_currentBoard = m_startingBoard;

    m_moveNodes.append(MoveNode());
    setModified(false);
}

void Game::clearTags()
{
    m_tags.clear();
    setModified(true);
}

QString Game::tag(const QString& tag) const
{
    return m_tags[tag];
}

const QMap<QString, QString>& Game::tags() const
{
    return m_tags;
}

void Game::setTag(const QString& tag, const QString& value)
{
    m_tags[tag] = value;
}

void Game::removeTag(const QString& tag)
{
    m_tags.remove(tag);
}

void Game::setStartingBoard(const SHATRA::Board& startingBoard)
{
    setStartingBoard(startingBoard.toSPN());
}

void Game::setStartingBoard(const QString& spn)
{
    clear();
    m_startingBoard.fromSPN(spn);
    if (m_startingBoard != SHATRA::standardStartBoard)
    {
        m_tags[SHATRA::TagNameSPN] = spn;
        m_tags[SHATRA::TagNameSetUp] = "1";
    }
    m_currentBoard = m_startingBoard;
    m_startPly = (m_startingBoard.moveNumber() - 1) * 2
                    + (m_startingBoard.toMove() == SHATRA::Black);
}

void Game::setResult(SHATRA::Result result)
{
    m_tags["Result"] = resultString(result);
    setModified(true);
}

QString Game::moveToLann(MoveStringFlags flags, NextPreviousMove nextPrevious, MoveId moveId)
{
    MoveId node = nodeValid(moveId);
    if (node != NO_MOVE && nextPrevious == NextMove)
        node = m_moveNodes[node].nextNode;
    if (node == NO_MOVE)
        return QString();

    MoveNode move;
    move = m_moveNodes[node];
    if (!(move.move.isLegal()))
        return QString();

    // Save current node
    MoveId saveNode = NO_MOVE;
    MoveId boardNode = m_moveNodes[node].previousNode;
    if (boardNode != m_currentNode) {
        saveNode = m_currentNode;
        moveToId(boardNode);
    }

    // Move number
    QString lann;
    if (m_currentBoard.toMove() == SHATRA::Black &&
        !move.move.wasInSequence() && flags & BlackNumbers)
        lann += QString::number(moveNumber(node)) + "... ";
    else if (m_currentBoard.toMove() == SHATRA::White &&
        !move.move.wasInSequence() && flags & WhiteNumbers)
        lann += QString::number(moveNumber(node)) + ". ";

    // Move and LANN
    lann += m_currentBoard.moveToLann(move.move);
    if (flags & Nags)
        lann += nags(node).toString();
        
    // Restore previous position
    if (saveNode != NO_MOVE)
        moveToId(saveNode);

    return lann;
}

void Game::dumpMoveNode(MoveId moveId)
{
    if (moveId == CURRENT_MOVE) {
        moveId = m_currentNode;
    }
    if (moveId != NO_MOVE) {
        qDebug() << "Move Id : " << moveId;
        qDebug() << "   Next node   : " << m_moveNodes[moveId].nextNode;
        qDebug() << "   Prev node   : " << m_moveNodes[moveId].previousNode;
        qDebug() << "   Parent node : " << m_moveNodes[moveId].parentNode;
        qDebug() << "   Nags        : " << m_moveNodes[moveId].nags.toString();
        qDebug() << "   Deleted     : " << m_moveNodes[moveId].removed;
        qDebug() << "   # Variations: " << m_moveNodes[moveId].variations.size();
        qDebug() << "   Variations  : " << m_moveNodes[moveId].variations;
        qDebug() << "   Move        : " << moveToLann(FullDetail, PreviousMove, moveId);
    }
}

void Game::dumpAllMoveNodes()
{
    qDebug() << "Current Node: " << m_currentNode;
    for (int i = 0; i < m_moveNodes.size(); ++i) {
        dumpMoveNode(i);
        if (!m_annotations.value(i).isEmpty()) {
            qDebug() << "   Annotation : " << m_annotations.value(i);
        }
    }
    int moves, comments, nags;
    moveCount(&moves, &comments, &nags);
    qDebug() << "Moves: " << moves << " Comments: " << comments << " Nags: " << nags;
}

int Game::findPosition(const SHATRA::Board& position)
{
    moveToStart();

    int current = m_currentNode;
    SHATRA::Board currentBoard(m_currentBoard);

    for (;;) {
        if (currentBoard == position && currentBoard.positionIsSame(position))
            return current;

        current = m_moveNodes[current].nextNode;
        if (current == NO_MOVE || !position.canBeReachedFrom(currentBoard))
            return NO_MOVE;

        currentBoard.doMove(m_moveNodes[current].move);
    }
    return NO_MOVE;
}

void Game::compact()
{
    int oldSize = m_moveNodes.size();
    QList <MoveNode> moveNodes;
    QVector <int> oldIdNewIdMapping(oldSize, NO_MOVE);

    for (int i = 0; i < oldSize; ++i) {
        if (!m_moveNodes[i].removed) {
            oldIdNewIdMapping[i] = moveNodes.size();
            moveNodes.append(m_moveNodes[i]);
        }
    }

    // update nodes links to other nodes in shrinked list (prev, next, variations)
    for (int i = 0, newSize = moveNodes.size(); i < newSize; ++i) {
        MoveNode& node = moveNodes[i];
#define GAME_UPDATE_MOVEID(aMoveId) if (aMoveId != NO_MOVE) aMoveId = oldIdNewIdMapping[aMoveId]
        GAME_UPDATE_MOVEID(node.nextNode);
        GAME_UPDATE_MOVEID(node.previousNode);
        GAME_UPDATE_MOVEID(node.parentNode);
        QList<MoveId>& vars = node.variations;
        for (int j = 0; j < vars.size(); ++j)
            GAME_UPDATE_MOVEID(vars[j]);
        vars.removeAll(NO_MOVE);
#undef  GAME_UPDATE_LINK
    }

    m_moveNodes.clear();
    m_moveNodes = moveNodes;
    m_currentNode = oldIdNewIdMapping[m_currentNode];
}

void Game::reparentVariation(MoveId variation, MoveId parent)
{
    if (variation != NO_MOVE)
        setModified(true);
    for (MoveId node = variation; node != NO_MOVE; node = m_moveNodes[node].nextNode)
        m_moveNodes[node].parentNode = parent;
}

QString Game::specialAnnotation(QString& annotation, QString specialMark ) const // [%csl  [%cal
{
    QString result = "";
    int specialAnnotationStart = annotation.indexOf(specialMark);
    // If we found a chessbase special annotation
    if(specialAnnotationStart >= 0 ) {

        int specialAnnotationDataStart = specialAnnotationStart + specialMark.length() + 1; //+ 1 for space after specialMark
        int specialAnnotationEnd = annotation.indexOf(']', specialAnnotationDataStart);

        if(specialAnnotationEnd > specialAnnotationDataStart )
        {
            result = annotation.mid(specialAnnotationDataStart,specialAnnotationEnd - specialAnnotationDataStart );
        };
    }
    return result;
}


