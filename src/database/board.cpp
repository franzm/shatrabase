/***************************************************************************
 *                           Shatrabase                                    *
 *   (C) 2012-2014 AFKM <francis@afkm.com>                                 *
 *                   derived from ChessX with thanks                       *
 *   (C) 2005 Kamil Przybyla <kamilprz@poczta.onet.pl>                     *
 *   (C) 2005-2006 William Hoggarth <whoggarth@users.sourceforge.net>      *
 *   (C) 2005-2006 Marius Roets <roets.marius@gmail.com>                   *
 *   (C) 2005-2009 Michal Rudolf <mrudolf@kdewebdev.org>                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "board.h"

quint64 RAND_VALUES[MAX_PIECES][MAX_SQUARES];
quint64 RAND_EN_PASSANT[MAX_EN_PASSANT_SQUARES];
quint64 RAND_URGENT[MAX_URGENT_SQUARES];
quint64 RAND_TRANSIT[MAX_SQUARES];
quint64 RAND_TEMDEK[2];
quint64 START_HASH;
quint64 RAND_TO_MOVE;

bool BoardInitRun;
void BoardInit();

Board::Board()
        : m_hashValue(0)
{
    if (!BoardInitRun) BoardInit();
    m_squareAnnotation = "";
}

void Board::clear()
{
    SBoard::clear();
    m_hashValue = 0;
    m_squareAnnotation = "";
}

void Board::setStandardPosition()
{
    SBoard::setStandardPosition();

    if (!BoardInitRun)
    {
    	createHash();
        START_HASH = m_hashValue;
    	BoardInitRun = true;
    }
    else m_hashValue = START_HASH;
    m_squareAnnotation = ""; // can gen movelist here
}

bool Board::fromSPN(const QString& SPN)
{
    if (SBoard::fromSPN(SPN)) {
        createHash();
        m_squareAnnotation = ""; // can gen movelist here
        return true;
    }
    return false;
}

void Board::setAt(Square s, Piece p) // need to gen movelist?
{
    hashPiece(s, pieceAt(s));
    SBoard::setAt(NB[s], p);
    hashPiece(s, p);
}

void Board::removeFrom(Square s) // need to gen movelist?
{
    hashPiece(s, pieceAt(s));
    removeAt(NB[s]);
}

Move Board::prepareMove(const Square& from, const Square& to) const
{
	return SBoard::prepareMove(NB[from], NB[to]);
}

Piece Board::pieceAt(const Square s) const
{
	return SBoard::pieceAt(NB[s]);
}

bool Board::isMovable(const Square from) const
{
	return SBoard::isMovable(NB[from]);
}

bool Board::canMoveTo(const Square from, const Square to) const
{
    return SBoard::canMoveTo(NB[from], NB[to]);
}

void Board::setToMove(Color c)
{
    if (toMove() != c)
        hashToMove();
    SBoard::setToMove(c);
}

void Board::setEnPassantSquare(const Square s)
{
    SBoard::setEnPassantSquare(NB[s]);
}

bool Board::doIt(const Move& m, bool undo)
{

    if (!m.isLegal()) return false;
    m_movesLoaded = false;

    hashEpSquare(); // 'unhash' old squares
    hashTransitSquare();
    
    hashPiece(m.from(), m.pieceMoved());
    if (m.isPromotion()) {
        hashPiece(m.to(), m.promotedPiece());
    } else hashPiece(m.to(), m.pieceMoved());

    if (m.isCapture()) hashPiece(m.to(), m.capturedPiece());
    
    if (undo)
        SBoard::undoMove(m);
    else SBoard::doMove(m);

    if (!sameSide()) hashToMove();
    hashEpSquare();
    hashTransitSquare();
    if (m.flipsUrgent()) hashUrgentSquare();
    else if (pieceMoving(m) & URGENT) hashUrgentMoving(m);
    hashTemdek(m, undo);

    if (!g_loading)
    {
        if (inSequence()) generate(CAPTC, transitAt());
        else generate(MOVES, fsq, lsq);
        m_movesLoaded = true;
    }
    return true;
}

inline void Board::hashPiece(Square s, Piece p)
{
    if ((p > Empty) && (p < InvalidPiece))
        m_hashValue ^= RAND_VALUES[p - 1][BN[s]];
}

inline void Board::hashEpSquare()
{
    Square s = enPassantSquare();
    if (s == NoSquare) return;

    s = (s >= 19 && s <= 23)? s - 19 :
          (s >= 40 && s <= 44)? s - 35 : InvalidSquare;

    if (s != InvalidSquare)
        m_hashValue ^= RAND_EN_PASSANT[s];
}

inline void Board::hashTransitSquare()
{
    Square s = transitAt();
    if (s != NoSquare) m_hashValue ^= RAND_TRANSIT[s];
}

inline void Board::hashUrgentSquare()
{
    Square s = urgentAt();
    if (s == NoSquare) return;

    s = (s < 10)? s : (s > 53)? s - 44 : InvalidSquare;

    if (s != InvalidSquare) m_hashValue ^= RAND_URGENT[s];
}

inline void Board::hashUrgentMoving(const Move& m)
{
    Square s = BN[m.from()];
    if (s == NoSquare) return;
    s = (s < 10)? s : (s > 53)? s - 44 : InvalidSquare;
    
    if (s != InvalidSquare) m_hashValue ^= RAND_URGENT[s];
    
    s = BN[m.to()];
    if (s == NoSquare) return;
    s = (s < 10)? s : (s > 53)? s - 44 : InvalidSquare;
    
    if (s != InvalidSquare) m_hashValue ^= RAND_URGENT[s];
}

inline void Board::hashTemdek(const Move& m, bool undo)
{
    int side = m.sideMoving();
    if (m.decsTemdek() &&
        (undo? temdekLast(side) : temdekOff(side)))
    {
        hashTemdek(side);
    }
    if (m.decsTemdekSntm() &&
        (undo? temdekLast(!side) : temdekOff(!side)))
    {
        hashTemdek(!side);
    }
}

inline void Board::hashTemdek(int side)
{
    m_hashValue ^= RAND_TEMDEK[side];
}

inline void Board::hashToMove()
{
	m_hashValue = m_hashValue ^ RAND_TO_MOVE;
}

void Board::createHash()
{
    m_hashValue = 0;
    for (int i = fsq; i <= lsq; ++i) hashPiece(i, pieceAt(i));

    if (toMove() == Black) hashToMove();

    hashEpSquare();
    hashTransitSquare();
    hashUrgentSquare();
    if (temdekOff(White)) hashTemdek(White);
    if (temdekOff(Black)) hashTemdek(Black);
}

void BoardInit()
{
    arrayHashkey aH;
/*
 // can return #fixed (never seen with this seed or any other tried)
    int fixed;
    fixed  = aH.initHashkeys(RAND_VALUES[0], MAX_PIECES, MAX_SQUARES);
    fixed += aH.initHashkeys(RAND_EN_PASSANT, MAX_EN_PASSANT_SQUARES);
    fixed += aH.initHashkeys(RAND_URGENT, MAX_URGENT_SQUARES);
    fixed += aH.initHashkeys(RAND_TRANSIT, MAX_SQUARES);
    fixed += aH.initHashkeys(RAND_TEMDEK, 2);
    Q_ASSERT(fixed == 0);
*/
    aH.initHashkeys(RAND_VALUES[0], MAX_PIECES, MAX_SQUARES);
    aH.initHashkeys(RAND_EN_PASSANT, MAX_EN_PASSANT_SQUARES);
    aH.initHashkeys(RAND_URGENT, MAX_URGENT_SQUARES);
    aH.initHashkeys(RAND_TRANSIT, MAX_SQUARES);
    aH.initHashkeys(RAND_TEMDEK, 2);

    RAND_TO_MOVE = RAND_VALUES[0][0];
}

Board getStandardStartBoard()
{
    Board b;
    b.setStandardPosition();
    return b;
}

const Board standardStartBoard = getStandardStartBoard();

