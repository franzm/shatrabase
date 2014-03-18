#ifndef __MOVE_H__
#define __MOVE_H__

#include "common.h"

class SBoard;

class Move
{
public:
    /* Default constructor, creates an empty move */
    Move();
    /* Move entry constructor - only from and to squares set, others zeroed */
    Move(const Square from, const Square to);

    /* Set promotion piece. Default=Q, use this to change piece afterwards */
    void setPromotionPiece(PieceType type);
    /* Set promotion piece */
    void setPromoted(const int type);

    // Query
    /* Return square piece sits on before move */
    Square from() const;
    /* Return square piece sits on after move */
    Square to() const;
    
	/* Get the piece type moving NB not colorized */
	Piece pieceMoved() const;
    /* Return square of captured piece  */
    Square capturedAt() const;
    /* Return previous en passant square  */
    Square oldEPAvail() const;
    /* Return previous square of piece in transit */
    Square oldTransit() const;
    /* Return previous square of piece marked urgent */
    Square oldUrgent() const;
    /* Return side-to-move bit as stm */
    int sideMoving() const;
	/* Piece captured from the opponent by this move */
	Piece capturedPiece() const;
	/* If promotion get piece. Result undefined if no promotion */
	Piece promotedPiece() const;
	/* If promotion get piecetype. 0 = no promotion */
    uint promoted() const;
	
	/* Check if move is capture */
	bool isCapture() const;	
	/* Check whether move is a promotion */
	bool isPromotion() const;
	/* Check whether move needs to toggle urgent bit */
	bool flipsUrgent() const;
	/* Check whether capture move has a continuation */
	bool willContinue() const;
    /* Check whether current move was a continuation */
    bool wasInSequence() const;
    /* Check whether current move captures a biy */
    bool capturesBiy() const;
    /* Check whether move needs to decrement stm's temdek */
	bool decsTemdek() const;
	/* Check whether capture creates a promotion for sntm */
	bool isPromoSntm() const;
	/* Check whether move needs to decrement sntm's temdek */
	bool decsTemdekSntm() const;
	/* Check if from = to */
	bool isPassMove() const;
	/* Check if move is legal */
	bool isLegal() const;
	
	/* Convert to algebraic notation (e2e4, g8f6 etc.) */
	QString toAlgebraic() const;

    /* Return true if this move was made by given color */
	bool operator==(const Color& color) const;
    /* Return true if this move was NOT made by given color */
	bool operator!=(const Color& color) const;
    /* Return true if this move was made by given piece */
	bool operator==(const Piece& p) const;
    /* Return true if this move was NOT made by given piece */
	bool operator!=(const Piece& p) const;

    /* Moves are considered the same, only if they match exactly */
	friend bool operator==(const Move& m1, const Move& m2);
    /* Required for keeping moves in some map-like structures */
	friend bool operator<(const Move& m1, const Move& m2);

friend class SBoard; friend class Game;
private:
    /* Generate capture move */
    void genCapt(int from, int to, int piece,
        int capture, int victim, int flags);
    /* Generate all other move types */
    void genMove(int from, int to, int piece, int flags);
    /* Set type of piece moving */
    void setPieceType(const int p);
	/* Set type of piece captured */
    void setCaptureType(const int v);

    union {
        struct {
            union {
                struct {
                    ubyte f; // from
                    ubyte t; // to
                    ubyte c; // captured (or 0)
                    ubyte b; // bit flags
                };
                uint m;
            };
            union {
                struct {
                    ubyte e;  // old en passant
                    ubyte g;  // old urgent
                    ubyte x;  // old transit
                    ubyte o;  // old promo sntm
                };
                uint u;
            };
        };
        quint64 mu;
    };
};
 // NB we also pack moving and captured piece types
 // in the spare MSBs of six of the above bytes
 // we could if we like pack the u word data into two bytes
 // e (3 bits) g (5) x (1) o (3) and captured piece (3)
inline Move::Move()
    : mu(0)
{}
inline Move::Move(const Square from, const Square to)
    : f(from), t(to), c(0), b(0), u(0)
{}

inline void Move::genCapt(int from, int to, int p, int c_at,
                          int v, int flags)
{
    f = from, t = to, c = c_at, b = flags;
    setPieceType(p);
    setCaptureType(v);
}

inline void Move::genMove(int from, int to, int p, int flags)
{
    f = from, t = to, c = 0, b = flags;
    setPieceType(p);
}

inline int Move::sideMoving() const
{
	return (b & STM)>>2;
}

inline void Move::setPromotionPiece(const PieceType type)
{
    setPromoted((uint)type);
}

inline void Move::setPromoted(const int type)
{
    b &= ~PROMO; b |= type;
}

inline bool Move::isCapture() const
{
    return (c & 0x7f) != 0;
}
	
inline bool Move::capturesBiy() const
{
    return pieceType(capturedPiece()) == Biy;
}

inline bool Move::isPromotion() const
{
	return (b & PROMO) != 0;
}
	
inline bool Move::flipsUrgent() const
{
	return (b & FLIP_URGENT) != 0;
}
	
inline bool Move::willContinue() const
{
	return (b & C_CONT) != 0;
}
	
inline bool Move::wasInSequence() const
{
    return (x & 0x7f) != NoSquare;
}

inline bool Move::decsTemdek() const
{
	return (b & DECTDK) != 0;
}
	
inline bool Move::isPromoSntm() const
{
	return (b & PROMO_sntm) != 0;
}
	
inline bool Move::decsTemdekSntm() const
{
	return (b & DECTDK_sntm) != 0;
}

inline bool Move::isPassMove() const
{
	return f == t;
}

inline bool Move::isLegal() const
{
    return m != 0; // ahem
}

inline Square Move::from() const
{
    return f & 0x7f;
}

inline Square Move::to() const
{
    return t & 0x7f;
}

inline Square Move::capturedAt() const
{
    return c & 0x7f;
}

inline Square Move::oldEPAvail() const
{
    return e & 0x7f;
}

inline Square Move::oldTransit() const
{
    return x & 0x7f;
}

inline Square Move::oldUrgent() const
{
    return g;
}

inline void Move::setCaptureType(const int v)
{
    if (v & 1) e |= 0x80;
    if (v & 2) g |= 0x80;
    if (v & 4) x |= 0x80;
}

inline Piece Move::capturedPiece() const
{
    int v = 0;
    if (e & 0x80) v |= 1;
    if (g & 0x80) v |= 2;
    if (x & 0x80) v |= 4;
    return Piece(v + (b & STM? 0 : 5));
}

inline void Move::setPieceType(const int p)
{
    if (p & 1) f |= 0x80;
    if (p & 2) t |= 0x80;
    if (p & 4) c |= 0x80;
}

inline Piece Move::pieceMoved() const
{
    int p = 0;
    if (f & 0x80) p |= 1;
    if (t & 0x80) p |= 2;
    if (c & 0x80) p |= 4;
    return Piece(p + (b & STM? 0 : 5));
}

inline Piece Move::promotedPiece() const
{
   return Piece((b & 3) + ( b & STM? 5 : 0));
}

inline uint Move::promoted() const
{
	return b & 3;
}

inline bool operator==(const Move& m1, const Move& m2)
{
	return m1.mu == m2.mu;
}

inline bool operator<(const Move& m1, const Move& m2)
{
	return m1.m < m2.m;
}

inline bool Move::operator==(const Color& color) const
{
    return color == (b & STM? Black : White);
}

inline bool Move::operator!=(const Color& color) const
{
	return !(*this == color);
}

inline bool Move::operator==(const Piece& p) const
{
    return p == pieceMoved();
}

inline bool Move::operator!=(const Piece& p) const
{
	return !(*this == p);
}

inline QString Move::toAlgebraic() const
{
    if (!isLegal()) return "?";

    QString alg;
    alg += '`' + (f>>4);
    alg += QString::number(f & 0xf);
    alg += '`' + (t>>4);
    alg += QString::number(t & 0xf);
    return alg;
}

#endif // MOVE_H
