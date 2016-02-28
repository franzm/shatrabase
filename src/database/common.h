/***************************************************************************
 *                           Shatrabase                                    *
 *   (C) 2012-2014 AFKM <francis@afkm.com>                                 *
 *                   derived from ChessX with thanks                       *
 *   (C) 2005 William Hoggarth <whoggarth@users.sourceforge.net>           * 
 *   (C) 2006-2009 Michal Rudolf <mrudolf@kdewebdev.org>                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef __COMMON_H__
#define __COMMON_H__

#include <QMap>
#include <QString>
//#include <QStringList>

namespace SHATRA {

typedef unsigned char ubyte;
typedef unsigned char Square;
typedef quint64 bb;

struct SquareMove
{
    Square from, to;
    SquareMove(Square from, Square to) : from(from), to(to) { }
};

// masks for board pieces
//#define DEFUNKT         0x40        // doing this a new way
const int URGENT = 0x80;

// masks for flags byte
const ubyte PROMO = 0x03;           // detect promo (PieceType 1-3)

const ubyte STM = 0x04;             // side to move
const int DECTDK = 0x08;            // decrement temdek counter for stm
const int FLIP_URGENT = 0x10;       // must drop on next non-capture move
                                    // or capture out
const int DECTDK_sntm = 0x20;       // capture in enemy fortress    (   NB  }
const int PROMO_sntm = 0x40;        // for slider capture/autopromo ( 'sntm'}
const int C_CONT = 0x80;            // capture continuation from this move

const int NUM = false;
const int ALG = true;
const int MOVES = false;
const int CAPTC = true;


#define x 0x20
const ubyte SB[144] = {	// empty board

    x,  x,  x,  x,  x,  x,  x,  x,  x,  x,  x,  x,  x,  x,  x,  x,
    x,  x,  x,  x,  x,  0,  0,  0,  0,  0,  0,  x,  x,  x,  x,  x,      // a-file
    x,  x,  x,  x,  x,  0,  0,  0,  0,  0,  0,  x,  x,  x,  x,  x,      // b-file
    x,  0,  0,  0,  x,  0,  0,  0,  0,  0,  0,  x,  0,  0,  0,  x,      // c-file       E
    x,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  x,      // d-file     N + S
    x,  0,  0,  0,  x,  0,  0,  0,  0,  0,  0,  x,  0,  0,  0,  x,      // e-file       W
    x,  x,  x,  x,  x,  0,  0,  0,  0,  0,  0,  x,  x,  x,  x,  x,      // f-file
    x,  x,  x,  x,  x,  0,  0,  0,  0,  0,  0,  x,  x,  x,  x,  x,      // g-file
    x,  x,  x,  x,  x,  x,  x,  x,  x,  x,  x,  x,  x,  x,  x,  x
};
#undef x

#define x 0
const int BN[144] = { // table converts board coords to numerical system

    x,  x,  x,  x,  x,  x,  x,  x,  x,  x,  x,  x,  x,  x,  x,  x,
    x,  x,  x,  x,  x,  17, 24, 31, 38, 45, 52, x,  x,  x,  x,  x,
    x,  x,  x,  x,  x,  16, 23, 30, 37, 44, 51, x,  x,  x,  x,  x,
    x,  3,  6,  9,  x,  15, 22, 29, 36, 43, 50, x,  56, 59, 62, x,
    x,  2,  5,  8,  10, 14, 21, 28, 35, 42, 49, 53, 55, 58, 61, x,
    x,  1,  4,  7,  x,  13, 20, 27, 34, 41, 48, x,  54, 57, 60, x,
    x,  x,  x,  x,  x,  12, 19, 26, 33, 40, 47, x,  x,  x,  x,  x,
    x,  x,  x,  x,  x,  11, 18, 25, 32, 39, 46, x,  x,  x,  x,  x,
    x,  x,  x,  x,  x,  x,  x,  x,  x,  x,  x,  x,  x,  x,  x,  x
};
#undef x

const int NB[64] = {  // numeric indices into array of board squares
                      //  also useful for conversion to alphanumeric
    0x00,       0x51, 0x41, 0x31,    // eg 0x51 is 'e1', 0x15 is 'a5'
                0x52, 0x42, 0x32,
                0x53, 0x43, 0x33,
                      0x44,
    0x75, 0x65, 0x55, 0x45, 0x35, 0x25, 0x15,
    0x76, 0x66, 0x56, 0x46, 0x36, 0x26, 0x16,
    0x77, 0x67, 0x57, 0x47, 0x37, 0x27, 0x17,
    0x78, 0x68, 0x58, 0x48, 0x38, 0x28, 0x18,
    0x79, 0x69, 0x59, 0x49, 0x39, 0x29, 0x19,
    0x7a, 0x6a, 0x5a, 0x4a, 0x3a, 0x2a, 0x1a,
                      0x4b,
                0x5c, 0x4c, 0x3c,
                0x5d, 0x4d, 0x3d,
                0x5e, 0x4e, 0x3e
};


enum Piece
{
    EmptyPiece,
        WhiteBatyr, WhiteTura, WhiteYalkyn, WhiteBiy, WhiteShatra,
        BlackBatyr, BlackTura, BlackYalkyn, BlackBiy, BlackShatra,
        WasBatyr, WasTura, WasYalkyn, InvalidPiece, WasShatra
};

enum PieceType { NoPiece, Batyr, Tura, Yalkyn, Biy, Shatra };
enum Color { White, Black, NoColor };
enum BoardLimits { fsq = 1, lsq = 62 };
enum ShatraVersion { Unspecified, Original, Extended };

extern ShatraVersion g_version;

extern bool g_loading;
extern bool g_searching;
extern bool g_notation;
extern bool g_numRev; // coord numerals flipped
extern bool g_inRev;
extern bool g_newGame;
extern bool g_nChanged;
extern bool g_autoResultOnLoad;
extern bool g_resModified;
extern int  g_nErrors;
extern int  g_aveNodes;
extern int  g_totalNodes;

const int MaxGen = 256; // actual max appears to be 250
const int ConstPieceTypes = WasShatra + 1;
const int Was = WasBatyr - Batyr;
const int PC[2] = { EmptyPiece, WhiteShatra }; // piece 'color constant'
const Square InvalidSquare = 255;
const Square NoSquare = 0; // also used for board coord..
const Square gateAt[2] = { 10, 53 };
const int gateAtB[2] = { 0x44, 0x4b }; // ..otherwise int
const Square sFirst[2] = { 5, 10 };
const Square sFinal[2] = { 14, 1 };
const Square lTower[2] = { 17, 46 };
const Square rTower[2] = { 11, 52 };

/* Return opposite color */
inline Color oppositeColor(const Color c)
    { return c == White ? Black : White; }

/* Return true if given piece is White */
inline bool isWhite(const Piece p)
    { return p <= WhiteShatra && p > EmptyPiece; }

/* Return true if given piece is Black */
inline bool isBlack(const Piece p)
    { return p >= BlackBatyr && p <= BlackShatra; }

/* Return true if given piece is merely a shadow */
inline bool isDefunkt(const Piece p)
    { return p >= WasBatyr && p <= WasShatra; }

/* Return PieceType of given Piece */
inline PieceType pieceType(const Piece p)
    { return PieceType(isWhite(p) ? p : (isBlack(p) ? p -5 : NoPiece)); }

/* Return Color of given Piece */
inline Color pieceColor(const Piece p)
    { return isBlack(p) ? Black :
             isWhite(p) ? White : NoColor; }

/* Verify that given Piece is within ranges */
inline bool isValidPiece(const Piece p)
    { return isWhite(p) || isBlack(p) || isDefunkt(p); }

/* Return true if given piece is in either fortress */
inline bool isInFortress(const int s)
    { return s > gateAt[Black] || s < gateAt[White]; }

/* Return true if given piece is in either gate or fortress [v1]*/
inline bool isInFortGate(const int s)
    { return s >= gateAt[Black] || s <= gateAt[White]; }

/* Return true if given piece is in its home fortress */
inline bool isInHomeFort(const int s, const int stm)
    { return stm == White? s < gateAt[White] : s > gateAt[Black]; }
    
/* Return true if given piece is in its home gate or fortress [v1] */
inline bool isInHomeGF(const int s, const int stm)
    { return stm == White? s <= gateAt[White] : s >= gateAt[Black]; }

/* Return true if given piece is in the opponent's gate or fortress [v1] */
inline bool isInOppGF(const int s, const int stm)
    { return stm == White? s >= gateAt[Black] : s <= gateAt[White]; }

/* Rank and File :) */
inline int Rank(int s) { return s & 0x0f; }
inline int File(int s) { return s>>4; }

/* Return bitboard representing (notation) square number */
inline bb bSq( const Square sq)
    { return (bb)1<<sq; }

/* character parsing */
inline bool isUpper(const char c) { return c >= 'A' && c <= 'Z'; }
inline bool isAlpha(const char c) { return c >= 'a' && c <= 'z'; }
inline bool isNum(const char c)   { return c >= '0' && c <= '9'; }

// Pre-Increment
//Piece& operator++(Piece& w);

// Post-Increment
//Piece operator++(Piece& w, int);

const char strSquareNames[62][4] = {
    "e1", "d1", "c1", "e2", "d2", "c2", "e3", "d3",
    "c3", "d4", "g5", "f5", "e5", "d5", "c5", "b5",
    "a5", "g6", "f6", "e6", "d6", "c6", "b6", "a6",
    "g7", "f7", "e7", "d7", "c7", "b7", "a7", "g8",
    "f8", "e8", "d8", "c8", "b8", "a8", "g9", "f9",
    "e9", "d9", "c9", "b9", "a9", "g10", "f10", "e10",
    "d10", "c10", "b10", "a10", "d11", "e12", "d12",
    "c12", "e13", "d13", "c13", "e14", "d14", "c14"
};

enum MoveType
{
    StandardMove,
    PromotionBatyr = Batyr, PromotionTura = Tura,
    PromotionYalkyn = Yalkyn, EnPassant, DoubleAdvance
};

enum ResultType { NoResult, Win, Loss };
enum Result { ResultUnknown, WhiteWin, Draw, BlackWin };

//#define MIN(a,b) (b^((a^b) & -(a<b)))
//#define MAX(a,b) (a^((a^b) & -(a<b)))
//#define SWAP(a,b) (((a)^=(b)), ((b)^=(a)), ((a)^=(b)))

QString startPosition();

/* Returns a string representation of a result */
QString resultString(Result result);

static const QString TagNameSPN      = "SPN";
static const QString TagNameSetUp    = "Setup";
static const QString TagNameSource   = "Source";
static const QString TagNameResult   = "Result";
static const QString TagNameWhite    = "White";
static const QString TagNameBlack    = "Black";
static const QString TagNameEvent    = "Event";
static const QString TagNameSite     = "Site";
static const QString TagNameRound    = "Round";
static const QString TagNameDate     = "Date";
static const QString TagNamePlyCount = "PlyCount";
static const QString TagNameWhiteElo = "WhiteElo";
static const QString TagNameBlackElo = "BlackElo";
static const QString TagNameTimeControl = "TimeControl";

const QString StandardTags[7] = {"Event", "Site", "Date", "Round", "White", "Black", "Result"};

typedef quint32 TagIndex;
typedef quint32 ValueIndex;
typedef quint32 GameId;

} // namespace SHATRA

#endif


