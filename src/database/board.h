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

#ifndef __BOARD_H__
#define __BOARD_H__

#include "sboard.h"
#include "rand64.h"
#include "arrayhashkey.h"

/*
   This class is actually only responsible for calculating a hash for each
   position; it derives from SBoard to provide all other functionality.
   Operations that affect the hash values are implemented here, for others
   the derived methods are used (except where coord conversion is needed).
*/

class Board : public SBoard
{
public:
    /* Empty constructor. Creates empty board with no pieces. */
    Board();

    /* Resets board, restoring startup position */
    void setStandardPosition();

    /* Sets position from SPN, returns true if SPN was loaded */
    bool fromSPN(const QString& SPN);

    /* Clear board of all pieces and other state */
    void clear();

    /* Sets side to move - mostly for setting up position */
    void setToMove(Color c);
    
 // the next six methods convert numeric and board coords
 // the first two also update the hash value
    /* Set a piece on the given square */
    void setAt(Square s, Piece p, bool urg=false);
    /* Remove a piece from a given square */
    void removeFrom(Square s);

    /* Return a full Move() object given only from and to squares */
    Move prepareMove(const Square& from, const Square& to) const;
    /* Check movable from for BoardView */
    bool isMovable(const Square from) const;
    /* Check movable from-to, for BoardView */
    bool canMoveTo(const Square from, const Square to) const;
    /* Set en passant square, for Setup */
    void setEnPassantSquare(const Square s);
    /* Return piece at a given numeric square */
    Piece pieceAt(const Square s) const;

    /* Make standard move */
    inline bool doMove(const Move& m) { return doIt(m, false); }
    /* Undo standard move */
    inline void undoMove(const Move& m) { doIt(m, true); }
    
    /* Return hash value of current board position */
    quint64 getHashValue() const { return m_hashValue; }

    /* Compare two boards for equality, based on hash values */
    bool operator == (const Board& b) const
    {
        return m_hashValue == b.getHashValue();
    }

    /* Compare two boards for differences, based on hash values */
    bool operator != (const Board& b) const
    {
        return m_hashValue != b.getHashValue();
    }

    /* Sets the squareAnnotation*/
    void setSquareAnnotation(QString squareAnnotation)
    {
        m_squareAnnotation = squareAnnotation;
    }

    /* Gets the square annotation*/
    QString squareAnnotation()
    {
        return m_squareAnnotation;
    }

    /* Sets the arrowAnnotation*/
    void setArrowAnnotation(QString arrowAnnotation)
    {
        m_arrowAnnotation = arrowAnnotation;
    }

    /* Gets the square annotation*/
    QString arrowAnnotation()
    {
        return m_arrowAnnotation;
    }

private:
    quint64 m_hashValue;
    
    QString m_squareAnnotation;
    QString m_arrowAnnotation;

    /* Make or undo move on board, calculate hash value for new position */
    bool doIt(const Move& m, bool undo);
    /* Recalculate hash completely */
    void createHash();
    /* Update hash value for piece sitting on given square */
    void hashPiece(Square s, Piece p);
    /* Update hash value for side to move */
    void hashToMove();
    /* Update hash value for possible en passant square */
    bool hashEpSquare();
    /* Update hash value for a current urgent square */
    bool hashUrgentSquare(Square s);
    /* Update hash value for all current urgent squares (for createHash()) */
    void hashUrgentSquares();
    /* Update hash value for a moving urgent piece (rare, capture only) */
    void hashUrgentMoving(const Move& m);
    /* Update hash value for piece in transit */
    void hashTransitSquare();
    /* Update hash value for a Temdek removed (for either side) */
    void hashTemdek(const Move& m, bool undo);
    void hashTemdek(int side);
};

extern const Board standardStartBoard;
#endif

