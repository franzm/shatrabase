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
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef __SBOARD_H__
#define __SBOARD_H__

#include "boardlist.h"
#include "urstack.hpp"

class Move;

enum BoardStatus
{
    Valid, NoWhiteBiy, NoBlackBiy, TooManyBiys, InvalidEnPassant
};

enum D
{
    noD,

    n  =  1, e  = 16,
    s  = -n, w  = -e,

    ne = n+e, se = s+e,
    sw = s+w, nw = n+w

};

const D dir[8] = { n, ne, e, se, s, sw, w, nw };

#define x 0x20
const ubyte SB[144] =
{
    x,  x,  x,  x,  x,  x,  x,  x,  x,  x,  x,  x,  x,  x,  x,  x,
    x,  x,  x,  x,  x,  0,  0,  0,  0,  0,  0,  x,  x,  x,  x,  x,
    x,  x,  x,  x,  x,  0,  0,  0,  0,  0,  0,  x,  x,  x,  x,  x,
    x,  0,  0,  0,  x,  0,  0,  0,  0,  0,  0,  x,  0,  0,  0,  x,
    x,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  x,
    x,  0,  0,  0,  x,  0,  0,  0,  0,  0,  0,  x,  0,  0,  0,  x,
    x,  x,  x,  x,  x,  0,  0,  0,  0,  0,  0,  x,  x,  x,  x,  x,
    x,  x,  x,  x,  x,  0,  0,  0,  0,  0,  0,  x,  x,  x,  x,  x,
    x,  x,  x,  x,  x,  x,  x,  x,  x,  x,  x,  x,  x,  x,  x,  x
};
#undef x

#define x 0
const int BN[144] =   // convert board squares to numerical system
{
    x,  x,  x,  x,  x,  x,  x,  x,  x,  x,  x,  x,  x,  x,  x,  x,
    x,  x,  x,  x,  x,  46, 39, 32, 25, 18, 11, x,  x,  x,  x,  x,
    x,  x,  x,  x,  x,  47, 40, 33, 26, 19, 12, x,  x,  x,  x,  x,
    x,  60, 57, 54, x,  48, 41, 34, 27, 20, 13, x,  7,  4,  1,  x,
    x,  61, 58, 55, 53, 49, 42, 35, 28, 21, 14, 10, 8,  5,  2,  x,
    x,  62, 59, 56, x,  50, 43, 36, 29, 22, 15, x,  9,  6,  3,  x,
    x,  x,  x,  x,  x,  51, 44, 37, 30, 23, 16, x,  x,  x,  x,  x,
    x,  x,  x,  x,  x,  52, 45, 38, 31, 24, 17, x,  x,  x,  x,  x,
    x,  x,  x,  x,  x,  x,  x,  x,  x,  x,  x,  x,  x,  x,  x,  x
};
#undef x

const int NB[64] =   // numeric indices into array of board squares
{                    //  also useful for conversion to alphanumeric
    0x00,       0x3e, 0x4e, 0x5e, // eg 0x15 is 'a5', 0x4a is 'd10'
                0x3d, 0x4d, 0x5d,
                0x3c, 0x4c, 0x5c,
                      0x4b,
    0x1a, 0x2a, 0x3a, 0x4a, 0x5a, 0x6a, 0x7a,
    0x19, 0x29, 0x39, 0x49, 0x59, 0x69, 0x79,
    0x18, 0x28, 0x38, 0x48, 0x58, 0x68, 0x78,
    0x17, 0x27, 0x37, 0x47, 0x57, 0x67, 0x77,
    0x16, 0x26, 0x36, 0x46, 0x56, 0x66, 0x76,
    0x15, 0x25, 0x35, 0x45, 0x55, 0x65, 0x75,
                      0x44,
                0x33, 0x43, 0x53,
                0x32, 0x42, 0x52,
                0x31, 0x41, 0x51
};

// magic numbers :)
const int sRules[2][3] = {{ 1, 131, 199 }, { 16, 56, 124 }};
const int pRules[4] = { 255, 85, 170, 255 };
//const int dirBlk[8] = { 1, 2, 4, 8, 16, 32, 64, 128 };
/*
sRules[White][0] = 1;      // 00000001 bit zero is north, go clockwise
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

class SBoard
{
public:
    SBoard();

    // Play moves on board
    //
    /* Play given move, updating board state appropriately */
    bool doMove(const Move&);
    /* Return board to state prior to given move */
    void undoMove(const Move&);

    // Setup board
    //
    /* Initialize memory and board layout */
    void initState();
    /* Remove all pieces and state from board */
    void clear();
    /* Set move number in game */
    void setMoveNumber(uint moveNumber);
    /* Set initial shatra game position on the board */
    void setStandardPosition();
    /* Set the given piece on the board at the given square */
    bool setAt(const int at, const Piece p);
    /* Remove any piece sitting on given square */
    void removeAt(const int at);
    /* Toggle urgent bit, set m_urgent accordingly */
    void doUrgentAt(const int at);
    /* Set the side to move to the given color */
    void setToMove(const Color& c);
    /* Swap the side to move */
    void swapToMove();
    /* Setup board according to SPN string */
    bool SPNToBoard(const QString& qspn);
    /* Check for valid SPN string, call the above */
    bool fromSPN(const QString& SPN);
    /* Return a SPN string based on current board position */
    QString toSPN() const;
    /* Set En Passant Square */
    void setEnPassantSquare(const int at);
    /* Remove En Passant privilege */
    void clearEnPassantSquare();
    /* Fill offboard stash with full complement */
    void fillOffboard();

    // Move factories
    //
    /* Parse LAN representation of move, and return full Move() object */
    Move parseMove(const QString& algebraic);
    /* Return a full Move() object given only from-to (board notation) */
    Move prepareMove(const int from, const int to) const;
    Move prepareMove(const int from, const int to);

    // Query
    //
    /* Is piece sitting on given square moveable? */
    bool isMovable(const int from) const;
    /* Can a piece move from one square to another? */
    bool canMoveTo(const int from, const int to) const;
    /* Is a promotion piece available? */
    bool canPromoteTo(const PieceType pt) const;
    /* Is there a piece currently in transit? */
    bool inSequence() const;
    /* Is the given square empty? */
    bool isVacant(const int at) const;
    /* Is the given piece in its home fortress? */
    bool isReserve(const Square s) const;
    /* Is the given piece marked urgent? */
    bool isUrgent(const Square s) const;
    /* Return true if the side to move has no moves */
    bool hasNoMoves() const;
    /* Return piece at given square number */
    Piece pieceAt(const int at) const;
    /* Return piece type at given square number */
    PieceType pieceTypeAt(const int at) const;
    /* Return piece moving plus any flags (call before or after moving) */
    int pieceMoving(Move m) const;
    /* Return the current move number in the game */
    int moveNumber() const;
    /* Return color of current side to move */
    Color toMove() const;
    /* Return true if side to move = last side to move */
    bool sameSide() const;
    /* Return true if position can follow target position */
    bool canBeReachedFrom(const SBoard& target) const;
    /* Return true if position is same, no Move# in determination */
    bool positionIsSame(const SBoard& target) const;
    /* Return square where En passant capture may occur, or "NoSquare" */
    Square enPassantSquare() const;
    /* Return square where piece is in transit */
    Square transitAt() const;
    /* Return square where piece *is* marked 'urgent' */
    Square urgentAt() const;
    /* Return square where piece *was* marked 'urgent' */
    Square oldUrgent() const;
    /* Return square where piece was in transit */
    Square oldTransit() const;
    /* Return true if Biy on home square and Temdek on */
    bool isBiyOnTemdek(const Square s) const;
    /* Return true if given Temdek is on */
    bool temdekOn(int side) const;
    /* Return true if given Temdek is off */
    bool temdekOff(int side) const;
    /* Return true if given Temdek set to one */
    bool temdekLast(int side) const;
    /* Return number of moves in board movelist */
    int numMoves() const;

    // Long algebraic or numeric format
    //
    /* Return a LANN string representation of given move */
    QString moveToLann(const Move& move) const;
    /* Return a LANN string representing a given move with move number. */
    QString moveToFullLann(const Move& move) const;
/* Return ASCII character for given piece to be used in SPN */
    QChar pieceToChar(const Piece piece) const;

    // Validation
    //
    /* Check current position and return "Valid" or problem */
    BoardStatus validate() const;
	/* Return true if given SPN can be parsed */
	bool isValidSPN(const QString& spn) const;
    
    /* Generate all move types, third parameter optional */
    int generate(bool cc, int first, int last = NoSquare);

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
    /* Used to determine possible captures */
    inline bool isOppositeColor(int at) const;
    /* Biy can make evasion only if no other piece can capture */
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
  private:
    int m_sntm;                // side not to move
    int m_lstm;                // last side not to move :) (okay, not not)
    int m_from;
    int m_to;
    int m_ksq[2];              // locations of the biys
    int m_temdek[2];           // temdek counters
    int m_epSquare;            // pre-calculated for hash routine
    int m_epVictim;            // ep victim square
    int m_latePromo;           // square of last late promotion
    int m_moveNumber;          // move number in game (inc after black moves)
    int m_offBoard[12];        // pieces are either on or off the board
    int m_pieceCount[2];       // simple counter for onboard pieces
    int m_urgent;              // location of a piece marked urgent
    int m_promoWait[2];        // could need 3 for each side??? :)
    int m_b;                   // flags, better than passing it round
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
    return BN[m_urgent];
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
	return s == temdekAt[m_stm] && temdekOn(m_stm);
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
        s <= temdekAt[Black] :
        s >= temdekAt[White];
}

inline bool SBoard::isUrgent(const Square s) const
{
    return (m_sb[NB[s]] & URGENT) != 0;
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

inline int SBoard::pieceMoving(Move m) const
{
    return m_sb[m.from()] != Empty? m_sb[m.from()] : m_sb[m.to()];
}

inline void SBoard::doUrgentAt(const int at)
{
    m_sb[at] ^= URGENT;
    m_urgent = m_sb[at] & URGENT? at : NoSquare;
}

inline bool SBoard::canPromoteTo(PieceType pt) const
{
    int p = pt + PC[m_stm];
    if (m_offBoard[p] > 0) return true;
    return false;
}

inline QChar SBoard::pieceToChar(const Piece piece) const
{
    return piece > WasShatra? '?' : " QRBKSqrbksyui?p"[piece];
}                     // the 'yalkyns united independent party'

inline void SBoard::initState()
{
    memset(this, 0, sizeof(SBoard)); // right down here where
    memcpy(m_sb, SB, sizeof(SB));    // no one will notice it
}

#endif // __SBOARD_H__
