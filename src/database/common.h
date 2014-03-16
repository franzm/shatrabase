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
#include <QStringList>

typedef unsigned char ubyte;
typedef unsigned char Square;

struct SquareMove
{
    Square from, to;
    SquareMove(Square from, Square to) : from(from), to(to) { }
};

// masks for board pieces
//#define DEFUNKT         0x40        // doing this a new way
#define URGENT          0x80

// masks for flags byte
#define PROMO           0x03        // detect promo (PieceType 1-3)

#define STM             0x04        // side to move
#define DECTDK          0x08        // decrement temdek counter for stm
#define FLIP_URGENT     0x10        // must drop on next non-capture move
                                    // or capture out
#define DECTDK_sntm     0x20        // capture in enemy fortress    (   NB  } 
#define PROMO_sntm      0x40        // for slider capture/autopromo ( 'sntm'}
#define C_CONT          0x80        // capture continuation from this move

#define NUM             false
#define ALG             true
#define MOVES           false
#define CAPTC           true

enum Piece
{
    Empty,
        WhiteBatyr, WhiteTura, WhiteYalkyn, WhiteBiy, WhiteShatra,
        BlackBatyr, BlackTura, BlackYalkyn, BlackBiy, BlackShatra,
        WasBatyr, WasTura, WasYalkyn, InvalidPiece, WasShatra
};

enum PieceType { None, Batyr, Tura, Yalkyn, Biy, Shatra };
enum Color { White, Black };
enum BoardLimits { fsq = 1, lsq = 62 };

extern bool g_loading;
extern bool g_searching;
extern bool g_notation;
extern bool g_nChanged;
extern bool g_autoResult0nLoad;
extern bool g_resModified;
extern int  g_nErrors;
extern int  g_aveNodes;
extern int  g_totalNodes;


const int ConstPieceTypes = WasShatra + 1;
const int Was = WasBatyr - Batyr;
const int PC[2] = { Empty, WhiteShatra }; // piece 'color constant'
const Square InvalidSquare = 255;
const Square NoSquare = 0; // also used for board coord..
const Square temdekAt[2] = { 10, 53 };
const int temdekAtB[2] = { 0x44, 0x4b }; // ..otherwise int
const Square sFirst[2] = { 5, 10 };
const Square sFinal[2] = { 14, 1 };
const Square lTower[2] = { 17, 46 };
const Square rTower[2] = { 11, 52 };

/* Return opposite color */
inline Color oppositeColor(const Color c)
    { return c == White ? Black : White; }

/* Return true if given piece is White */
inline bool isWhite(const Piece p)
    { return p <= WhiteShatra && p > Empty; }

/* Return true if given piece is Black */
inline bool isBlack(const Piece p)
    { return p >= BlackBatyr && p <= BlackShatra; }

/* Return true if given piece is merely a shadow */
inline bool isDefunkt(const Piece p)
    { return p >= WasBatyr && p <= WasShatra; }

/* Return PieceType of given Piece */
inline PieceType pieceType(const Piece p)
    { return PieceType(isWhite(p) ? p : (isBlack(p) ? p -5 : Empty)); }

/* Return Color of given Piece */
inline Color pieceColor(const Piece p)
    { return isBlack(p) ? Black : White; }

/* Verify that given Piece is within ranges */
inline bool isValidPiece(const Piece p)
    { return isWhite(p) || isBlack(p) || isDefunkt(p); }

/* Return true if given piece is in either fortress */
inline bool isInFortress(const int s)
    { return s > temdekAt[Black] || s < temdekAt[White]; }

/* Return true if given piece is in its home fortress */
inline bool isInHomeFort(const int s, const int stm)
    { return stm == White? s < temdekAt[White] : s > temdekAt[Black]; }

/* Rank and File :) */
inline int Rank(int s) { return s & 0x0f; }
inline int File(int s) { return s>>4; }

/* character parsing */
inline bool isUpper(const char c) { return c >= 'A' && c <= 'Z'; }
inline bool isAlpha(const char c) { return c >= 'a' && c <= 'z'; }
inline bool isNum(const char c) { return c >= '0' && c <= '9'; }

// Pre-Increment
//Piece& operator++(Piece& w);

// Post-Increment
//Piece operator++(Piece& w, int);

const char strSquareNames[62][4] = {
    "e1", "d1", "c1", "e2", "d2", "c2", "e3", "d3",
    "c3", "d4", "g5", "f5", "e5", "d5", "c5", "b5",
    "a5", "g6", "f6", "e6", "d6", "c6", "b6", "a6",
    "g7", "f7", "e7", "d7", "c7", "b7", "a7", "g8",
    "f8", "e8", "d8", "c8", "b8", "a9", "g9", "f9",
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

enum Result { ResultUnknown, WhiteWin, Draw, BlackWin };

//#define MIN(a,b) b^((a^b) & -(a<b))
//#define MAX(a,b) a^((a^b) & -(a<b))
//#define SWAP(a,b) (((a)^=(b)), ((b)^=(a)), ((a)^=(b)))

/* Returns a string representation of a result */
QString resultString(Result result);

static const QString TagNameSPN      = "SPN";
static const QString TagNameECO      = "ECO";
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

const QString StandardTags[7] = {"Event", "Site", "Date", "Round", "White", "Black", "Result"};

typedef quint32 TagIndex;
typedef quint32 ValueIndex;
typedef quint32 GameId;

#endif


