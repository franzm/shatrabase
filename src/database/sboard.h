/***************************************************************************
 *                           Shatrabase                                    *
 *   (C) 2012-2014 AFKM <francis@afkm.com>                                 *
 *                   derived from ChessX with thanks                       *
 *   (C) 2003 Sune Fischer                                                 *
 *   (C) 2005-2006 Marius Roets <roets.marius@gmail.com>                   *
 *   (C) 2005-2009 Michal Rudolf <mrudolf@kdewebdev.org>                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef __SBOARD_H__
#define __SBOARD_H__

#include <vector>

#include "boardlist.h"
#include "urstack.hpp"

class Move;

enum BoardStatus
{
    Valid, NoWhiteBiy, NoBlackBiy, TooManyBiys, InvalidEnPassant,
    StatusUnknown
};

enum D
{
    noD,

    s  =  1, w  = 16,
    n  = -s, e  = -w,

    ne = n+e, se = s+e,
    sw = s+w, nw = n+w

};

const D dir[8] = { s, sw, w, nw, n, ne, e, se };


// magic numbers :)
const int sRules[2][3] = {{ 1, 131, 199 }, { 16, 56, 124 }};
const int pRules[4] = { 255, 85, 170, 255 };
/*
sRules[White][0] = 1;      // 00000001 bit zero is south, go clockwise
sRules[White][1] = 131;    // 10000011 at rank 7
sRules[White][2] = 199;    // 11000111 at rank 8
sRules[Black][0] = 16;     // 00010000
sRules[Black][1] = 56;     // 00111000 at rank 8
sRules[Black][2] = 124;    // 01111100 at rank 7

pRules[0] = 255;           // 11111111 queen
pRules[1] = 85;            // 01010101 rook
pRules[2] = 170;           // 10101010 bishop
pRules[3] = 255;           // 11111111 king
*/
// de Bruijn
const quint64 magic = 0x0218a4da3b967abf; // the 7777
const quint64 h1    = 0x5555555555555555;
const quint64 h2    = 0x3333333333333333;
const quint64 h4    = 0x0F0F0F0F0F0F0F0F;
const quint64 v1    = 0x00FF00FF00FF00FF;
const quint64 v2    = 0x0000FFFF0000FFFF;

const Square magictable[64] =
{
    0,  1,  2,  7,  3, 13,  8, 31,
    4, 19, 14, 41,  9, 22, 32, 47,
    5, 29, 17, 20, 15, 53, 42, 55,
   10, 44, 26, 23, 37, 33, 48, 57,
   63,  6, 12, 30, 18, 40, 21, 46,
   28, 16, 52, 54, 43, 25, 36, 56,
   62, 11, 39, 45, 27, 51, 24, 35,
   61, 38, 50, 34, 60, 49, 59, 58,
};

#ifdef QT_OS_WIN
#   pragma warning(disable: 4146)
#endif
inline Square bitScanForward(quint64 b) {
    return magictable[((b & -b) * magic) >> 58];
}

const bb bFort[2] = { 0x00000000000003fe,0x7fc0000000000000 };

class SBoard
{
public:
    SBoard();

    // Play moves on board
    //
    /** Play given move, updating board state appropriately */
    bool doMove(const Move&);
    /** Return board to state prior to given move */
    void undoMove(const Move&);

    // Setup board
    //
    /** Initialize memory and board layout */
    void initState();
    /** Remove all pieces and state from board */
    void clear();
    /** Set move number in game */
    void setMoveNumber(uint moveNumber);
    /** Set initial shatra game position on the board */
    void setStandardPosition();
    /** Set the given piece on the board at the given square */
    bool setAt(const int at, const Piece p, bool urg=false);
    /** Sets the transit piece (Piece currently executing capture sequence) */
    void setTransitAt(const Square at = NoSquare);
    /** Remove any piece sitting on given square */
    void removeAt(const int at);
    /** Toggle urgent bit, set m_urgent data accordingly */
    void doUrgentAt(const Move& m);
    /** set urgent bit, set m_urgent data accordingly */
    void setUrgentAt(const int at);
    /** clear m_urgent and data */
    void clearUrgentAt(const int at);
    /** Set the side to move to the given color */
    void setToMove(const Color& c);
    /** Swap the side to move */
    void swapToMove();
    /** Setup board according to SPN string */
    bool SPNToBoard(const QString& qspn);
    /** Check for valid SPN string, call the above */
    bool fromSPN(const QString& SPN);
    /** Return a SPN string based on current board position */
    QString toSPN() const;
    /** Set En Passant Square */
    void setEnPassantSquare(const int at);
    /** Remove En Passant privilege */
    void clearEnPassantSquare();
    /** Fill offboard stash with full complement */
    void fillOffboard();

    // Move factories
    //
    /** Parse LAN representation of move, and return full Move() object */
    Move parseMove(const QString& algebraic);
    /** Return a full Move() object given only from-to (board notation) */
    Move prepareMove(const int from, const int to) const;
    Move prepareMove(const int from, const int to);

    // Query
    //
    /** Is piece sitting on given square moveable? */
    bool isMovable(const int from) const;
    /** Can a piece move from one square to another? */
    bool canMoveTo(const int from, const int to) const;
    /** Is a promotion piece available? */
    bool canPromoteTo(const PieceType pt) const;
    /** Is there a piece currently in transit? */
    bool inSequence() const;
    /** Is the given square empty? */
    bool isVacant(const int at) const;
    /** Is the given piece in its home fortress & temdek on? */
    bool isReserve(const Square s) const;
    /** Is the given piece marked urgent? */
    bool isUrgent(const Square s) const;
    /** Is the given Square a tower square? */
    bool isTower(const Square s) const;
    /** Return true if the side to move has no moves */
    bool hasNoMoves() const;
    /** Return result based on contents of m_biyAt[] */
    Result gameResult() const;
    /** Is game over? */
    bool isEnd() const { // XXX need to fix this Draw thing
        return !(gameResult() == Draw || gameResult() == ResultUnknown); }
    /** Return number of pieces of given color */
    int pieceCount(Color) const;
    /** Return piece at given square number */
    Piece pieceAt(const int at) const;
    /** Return piece type at given square number */
    PieceType pieceTypeAt(const int at) const;
    /** Return piece moving plus any flags (call before or after moving) */
    int pieceMoving(const Move m) const;
    /** Return the current move number in the game */
    int moveNumber() const;
    /** Return color of current side to move */
    Color toMove() const;
    /** Return true if side to move = last side to move */
    bool sameSide() const;
    /** Return true if position can follow target position */
    bool canBeReachedFrom(const SBoard& target) const;
    /** Return true if position is same, no Move# in determination */
    bool positionIsSame(const SBoard& target) const;
    /** Return square where En passant capture may occur, or "NoSquare" */
    Square enPassantSquare() const;
    /** Return square where piece is in transit */
    Square transitAt() const;
    /** Return square where piece *is* marked 'urgent' */
    Square urgentAt() const;
//    /** Return square where piece *was* marked 'urgent' */
//    Square oldUrgent() const;
//    /** Return square where piece was in transit */
//    Square oldTransit() const;
    /** Return true if Biy on home square and Temdek on */
    bool isBiyOnTemdek(const Square s) const;
    /** Return true if given Temdek is on */
    bool temdekOn(int side) const;
    /** Return true if given Temdek is off */
    bool temdekOff(int side) const;
    /** Return true if given Temdek set to one */
    bool temdekLast(int side) const;
    /** Return number of moves in board movelist */
    int numMoves() const;
    /** Return all reachable squares for the given square.
        The Squares will be push_back'd onto the vector. */
    void getReachableSquares(Square from, std::vector<Square>& vec) const;
    /** Returns all moves in tuples of SquareFrom and SquareTo.
        The moves will be push_back'd onto the vector */
    void getMoveSquares(std::vector<SquareMove>& vec) const;

    // Long algebraic or numeric format
    //
    /** Return a LANN string representation of given move */
    QString moveToLann(const Move& move) const;
    /** Return a LANN string representing a given move with move number. */
    QString moveToFullLann(const Move& move) const;
    /** Return ASCII character for given piece to be used in SPN */
    QChar pieceToChar(const Piece piece) const;

    // Validation
    //
    /** Check current position and return "Valid" or problem */
    BoardStatus validate() const;
    /** Return true if EP capture possible on given square */
    bool epPossible(int sq, Color side) const;
    /** Return true if given SPN can be parsed */
	bool isValidSPN(const QString& spn) const;
    
    /** Generate all move types, third parameter optional */
    int generate(bool cc, int first, int last = NoSquare);

    // Debugging
    //
    /** Return a debug dump with all the board state.
     *  If @p compare != 0, add an * at each value that's different */
    QString dumpAll(const SBoard * compare = 0) const;

  private:
    inline int sPhi(int s); // s is numeric square, others are board coords
    inline int promoWaiting();
    inline bool prohibited(int to, PieceType p);
    inline bool duplicate(int to, bool fort);
    inline void doCFlags(int from, int to, int cp);
    inline bool getDrops(int s, PieceType piece);
    inline void getPorts(int s);
    inline void getEvasions();
    inline bool getCapture(int at, PieceType piece, D d, int r, bool f);
    inline void getMoves(int at, PieceType piece, D d, bool df);
    /** Used to determine possible captures */
    inline bool isOppositeColor(int at) const;
    /** Biy can make evasion only if no other piece can capture */
    inline bool onlyBiyCaptures() const;

    ubyte m_sb[144];           // shatra board, MUST be first data member
  protected:
    // Extra state data
    bool m_movesLoaded;        // movelist exists for this board position
    int m_stm;                 // side to move
    int m_transit;             // square of piece in capture sequence
 // NB this will always be the same as m_to when going forwards, but also
 //  1) acts as a flag, and 2) allows us to shorten notation, eg :g10 but
 // TODO rethink undo data uint, could be ushort?
    bb  m_allurgent;           // bitboard of all pieces marked urgent
  //private:
public:
    int m_sntm;                // side not to move
    int m_lstm;                // last side to move
    int m_from;
    int m_to;
    int m_biyAt[2];            // locations of the biys
    int m_temdek[2];           // temdek counters
    int m_epSquare;            // pre-calculated for hash routine
    int m_epVictim;            // ep victim square
    int m_latePromo;           // square of last late promotion
    int m_moveNumber;          // move number in game (inc after black moves)
    int m_halfMoves;           // number of gameply since last capture or promotion
    int m_offBoard[12];        // pieces are either on or off the board
    int m_pieceCount[2];       // simple counter for onboard pieces
    int m_promoWait[2];        // could need 3 for each side??? :)
    int m_b;                   // flags, better than passing it round
    int m_urgent[2];           // location of moving piece marked urgent
    bool m_caps[2];            // used for "if no other piece can capture"
    bool m_temdekPending[2];   // delay temdek removal during capture sequence
    urstack<int,16> m_dfs;     // stack for defunkt pieces - NB no real undo
    urvct<int,16> m_ext;       // vector for capture extensions
  protected:
    BoardList m_ml;            // list of generated moves

};

inline int SBoard::moveNumber() const
{
    return m_moveNumber;
}

inline Color SBoard::toMove() const
{
    return Color(m_stm);
}

inline Square SBoard::enPassantSquare() const
{
    return BN[m_epSquare];
}

inline Square SBoard::transitAt() const
{
    return BN[m_transit];
}

inline Square SBoard::urgentAt() const
{
    return BN[m_urgent[m_stm]];
}

inline bool SBoard::inSequence() const
{
	return m_transit != NoSquare;
}

inline bool SBoard::isVacant(const int at) const
{
    return m_sb[at] == Empty;
}

inline bool SBoard::isBiyOnTemdek(const Square s) const
{
    return s == gateAt[m_stm] && temdekOn(m_stm);
}

inline bool SBoard::temdekOn(int side) const
{
    return m_temdek[side] > 0 || m_temdekPending[side];
}

inline bool SBoard::temdekOff(int side) const
{
    return m_temdek[side] == 0 && !m_temdekPending[side];
}

inline bool SBoard::temdekLast(int side) const
{
	return m_temdek[side] == 1;
}

inline void SBoard::setToMove(const Color& c)
{
    m_stm = c; m_sntm = !m_stm;
}

inline bool SBoard::sameSide() const
{
	return m_stm == m_lstm;
}

inline bool SBoard::onlyBiyCaptures() const
{
	return m_caps[0] && !m_caps[1];
}

inline bool SBoard::isOppositeColor(int at) const
{
	int p = m_sb[at] & 0x7f; return m_stm? // mask out urgent flag
	    p >= WhiteBatyr && p <= WhiteShatra :
        p >= BlackBatyr && p <= BlackShatra;
}

inline bool SBoard::isReserve(const Square s) const
{
    if (temdekOff(m_stm)) return false;
    return m_stm?
        s >= gateAt[Black] :
        s <= gateAt[White];
}

inline bool SBoard::isUrgent(const Square s) const
{
    return (m_sb[NB[s]] & URGENT) != 0;
}

inline bool SBoard::isTower(const Square s) const
{
    return (s == 11 || s == 17 || s == 46 || s == 52);
}

inline void SBoard::swapToMove()
{
    m_sntm = m_stm; m_stm ^= 1;
}

inline int SBoard::numMoves() const
{
	return m_ml.count();
}
 
inline void SBoard::setMoveNumber(uint moveNumber)
{
    m_moveNumber = moveNumber;
}

inline bool SBoard::positionIsSame(const SBoard& target) const
{
	int i, j;
	for (i = fsq; i <= lsq; i++)
	{
		j = NB[i]; if (m_sb[j] != target.m_sb[j]) return false;
	}
	return true;
}

inline Piece SBoard::pieceAt(const int at) const // NB board coords
{
    return (Piece)(m_sb[at] & 0xf); // remove possible flag(s)
}

inline PieceType SBoard::pieceTypeAt(const int at) const
{
    int p = m_sb[at] & 0xf;
    return (PieceType)(p - (p > WhiteShatra?  5 : 0));
}

inline int SBoard::pieceMoving(const Move m) const
{
    return m_sb[m.from()] != Empty? m_sb[m.from()] : m_sb[m.to()];
}

inline void SBoard::doUrgentAt(const Move& m)
{
    Square t = m.to(), f = m.from();
    m_sb[t] ^= URGENT;
    m_urgent[m_stm] = m_sb[t] & URGENT? t : NoSquare;
    m_allurgent ^= m_sb[t] & URGENT?
        bSq(BN[t]) : bSq(BN[f]);
}

inline void SBoard::setUrgentAt(const int at)
{
    m_sb[at] |= URGENT;
    m_allurgent |= bSq(BN[at]);
    m_urgent[m_stm] = at;
}

inline void SBoard::clearUrgentAt(const int at)
{
    m_allurgent ^= bSq(BN[at]);
    m_urgent[m_stm] = NoSquare;
}

inline bool SBoard::canPromoteTo(PieceType pt) const
{
    int p = pt + PC[m_stm];
    return (m_offBoard[p] > 0);
}

inline QChar SBoard::pieceToChar(const Piece piece) const
{
    return piece > WasShatra? '?' : " QRBKSqrbksyui?p"[piece];
}                     // the 'yalkyns united independent party'

inline void SBoard::initState()
{
    memset(this, 0, sizeof(SBoard)); // right down here where
    memcpy(m_sb, SB, sizeof(SB));    // no one will notice it
                                     // aha! saw it - sb
}

#endif // __SBOARD_H__
