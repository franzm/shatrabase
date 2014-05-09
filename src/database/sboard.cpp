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

#include "sboard.h"
#include <QDebug>
#include <sstream>
#include <iomanip>

SBoard getStandardPosition();

SBoard standardPosition = getStandardPosition();
SBoard clearedPosition;


bool SBoardInitRun;
void SBoardInit();

/* Initialize a new ShatraBoard, ensure global data has been initialized */
SBoard::SBoard()
{
    initState();
    if (!SBoardInitRun) SBoardInit();
    m_sntm = Black;
    m_moveNumber = 1;
}

bool SBoard::hasNoMoves() const
{
    return m_ml.count() == 0; // NB must have tried to generate moves! :)
}

Result SBoard::gameResult() const
{
    return !m_biyAt[Black]? WhiteWin :
           !m_biyAt[White]? BlackWin : Draw;
}

int SBoard::pieceCount(Color color) const
{
    int num = 0;
        for (int i=fsq; i<=lsq; ++i)
        {
            const ubyte p = m_sb[NB[i]];
            if ((color == White && p>=WhiteBatyr && p<=WhiteShatra)
             || (color == Black && p>=BlackBatyr && p<=BlackShatra))
                ++num;
        }
    return num;
}

QString SBoard::moveToLann(const Move& move) const
{
    QString lann;
    int from = move.from();
    int to = move.to();

    if (move.isPassMove()) return "pass";//QObject::tr("pass");

    switch (g_notation)
    {
    case true:
        if (!move.wasInSequence())
        {
            if (pieceTypeAt(from) != Shatra)
            {
                lann = pieceToChar((Piece)(pieceTypeAt(from)));
            }
            lann += '`' + (from>>4);
            lann += QString::number(from & 0xf);
        }
        lann += move.isCapture()? ':' : '-';
        lann += '`' + (to>>4);
        lann += QString::number(to & 0xf);
        break;
    default:
        if (!move.wasInSequence())
        {
            lann += QString::number(BN[from]);
        }
        lann += move.isCapture()? ':' : '-';
        lann += QString::number(BN[to]);
    }
    
    if (move.isPromotion())
    {
        lann += pieceToChar((Piece)move.promoted());
    }

    if (move.capturesBiy())
        lann += 'x'; // game over

    return lann;
}

QString SBoard::moveToFullLann(const Move& move) const
{
    if (!move.wasInSequence())
    {
        QString dots = toMove() == White? "." : "...";
        return QString("%1%2%3").arg(m_moveNumber).
            arg(dots).arg(moveToLann(move));
    }
    else return moveToLann(move);
}

void SBoard::clear()
{
    *this = clearedPosition;
}

void SBoard::setStandardPosition()
{
    *this = standardPosition;
}

void SBoard::clearEnPassantSquare()
{
    m_epSquare = NoSquare;
}

void SBoard::setEnPassantSquare(const int at)
{
    m_epSquare = at;
}

void SBoard::setTransitAt(const Square at)
{
    if (at == NoSquare)
        m_transit = at;
    else
        m_transit = NB[at];
}

void SBoard::fillOffboard()
{
    m_offBoard[WhiteBatyr] = m_offBoard[BlackBatyr] = 1;
    m_offBoard[WhiteTura] = m_offBoard[BlackTura] = 2;
    m_offBoard[WhiteYalkyn] = m_offBoard[BlackYalkyn] = 2;
    m_offBoard[WhiteBiy] = m_offBoard[BlackBiy] = 1;
    m_offBoard[WhiteShatra] = m_offBoard[BlackShatra] = 11;
}

bool SBoard::isMovable(const int from) const
{
    if (!m_movesLoaded) return false;
    int i = m_ml.count();

    while (--i >= 0)
        if (m_ml[i].from() == from) return true;

    return false;
}

bool SBoard::canMoveTo(const int from, const int to) const
{
    int i = m_ml.count();

    while (--i >= 0)
        if (m_ml[i].from() == from &&
            m_ml[i].to() == to) return true;

    return false;
}

bool SBoard::setAt(const int at, const Piece p, bool urg=false)
{ // NB board coords
    Q_ASSERT_X(isValidPiece(p), "setAt()",
               QString("invalid piece %1").arg(p).toStdString().c_str());

    if (isDefunkt(p))
    {
        m_sb[at] = p; return true; // NB m_dfs.push(NB[s]) in setup
    }
    PieceType pt = pieceType(p);
    if (pt == None || m_offBoard[p] == 0) return false;
    
    m_sb[at] = p;
    if (urg) setUrgentAt(at);

    Color _color = pieceColor(p);
    ++m_pieceCount[_color];
    
    if(pt == Biy) m_biyAt[_color] = at;
    --m_offBoard[p];
    return true; // NB remember temdek count in position setup
}

void SBoard::removeAt(const int at) // also board coords
{
    Piece p = Piece(m_sb[at] & 0xf);
    m_sb[at] = Empty;

    if (isDefunkt(p)) p = Empty;
    if (p == Empty) return;

    PieceType pt = pieceType(p);

    Color _color = pieceColor(p);
    --m_pieceCount[_color];
    
    if(pt == Biy) m_biyAt[_color] = NoSquare;
    ++m_offBoard[p];

}

bool SBoard::isValidSPN(const QString& spn) const
{
    return SBoard().SPNToBoard(spn);
}

bool SBoard::fromSPN(const QString& spn)
{
    if (isValidSPN(spn))
    {
        SPNToBoard(spn);
        if (inSequence()) generate(CAPTC, transitAt());
        else generate(MOVES, fsq, lsq);
        m_movesLoaded = true;
        return true;
    }
    return false;
}

BoardStatus SBoard::validate() const
{
    int piece, r = StatusUnknown, wb = 0, bb = 0;

    for (int i = fsq; i <= lsq; i++)
    {
        piece = m_sb[NB[i]];
        if (piece)
        {
            Piece p = Piece(piece);
            if (p == WhiteBiy) ++wb;
            if (p == BlackBiy) ++bb;
        }
    }
    if (wb && bb)
    {
        r = !wb? NoWhiteBiy :
            !bb? NoBlackBiy :
            (wb > 1 || bb > 1)? TooManyBiys :
            Valid;
    }

    return BoardStatus(r); // will do something here...
}

bool SBoard::epPossible(int sq, Color side) const // should be Square?
{
    bool okay = false;
    int bsq = NB[sq];

    switch (side)
    {
    case White :
        if (pieceAt(bsq + n) != BlackShatra)
            break;
        if (pieceAt(bsq + ne) == WhiteShatra
         && pieceAt(bsq + sw) == Empty)
            { okay = true; break; }
        if (pieceAt(bsq + nw) == WhiteShatra
         && pieceAt(bsq + se) == Empty)
            { okay = true; break; }
        if (pieceAt(bsq + e) == WhiteShatra
         && pieceAt(bsq + w) == Empty)
            { okay = true; break; }
        if (pieceAt(bsq + w) == WhiteShatra
         && pieceAt(bsq + e) == Empty)
            { okay = true; break; }
        if (pieceAt(bsq + se) == WhiteShatra
         && pieceAt(bsq + nw) == Empty)
            { okay = true; break; }
        if (pieceAt(bsq + sw) == WhiteShatra
         && pieceAt(bsq + ne) == Empty)
            { okay = true; break; }
        break;
    case Black :
        if (pieceAt(bsq + s) != WhiteShatra)
            break;
        if (pieceAt(bsq + ne) == BlackShatra
         && pieceAt(bsq + sw) == Empty)
            { okay = true; break; }
        if (pieceAt(bsq + nw) == BlackShatra
         && pieceAt(bsq + se) == Empty)
            { okay = true; break; }
        if (pieceAt(bsq + e) == BlackShatra
         && pieceAt(bsq + w) == Empty)
            { okay = true; break; }
        if (pieceAt(bsq + w) == BlackShatra
         && pieceAt(bsq + e) == Empty)
            { okay = true; break; }
        if (pieceAt(bsq + se) == BlackShatra
         && pieceAt(bsq + nw) == Empty)
            { okay = true; break; }
        if (pieceAt(bsq + sw) == BlackShatra
         && pieceAt(bsq + ne) == Empty)
            { okay = true; break; }
    }
    return okay;
}

 // Why QString throws asserts for access past end of string and
 // refuses to return a real c++ char type is beyond him (me too)
class SaneString : public QString
{
public:
    SaneString(const QString& q) : QString(q) {}
    char operator[](const int index) const {
        if (index < this->length())
            return QString::operator[](index).toLatin1();
        return 0;
    }
};
 // square numbers after which a slash is expected
int spn_slash[10] = { 9, 10, 17, 24, 31, 38, 45, 52, 53 };

bool SBoard::SPNToBoard(const QString& qspn)
{
    SaneString spn(qspn);
    if (qspn.length() < 21) return false; // conceivable minimum SPN length?
    int i = 0, j = fsq;
    int k_on[2] = { 0, 0 };
    int tw = 0, tb = 0, sp = 0, sl = 0;
    char c = spn[0];
    bool found, wt, bt, urg = false;
    initState();
    m_moveNumber = 1;
    fillOffboard();

    while (j <= lsq)
    {
        if (sp > 0)
        {
            while(sp--) m_sb[NB[j++]] = Empty;
            c = spn[++i]; if (c == ' ') continue;
        }
        if (c == '/')
        {
            if (j != spn_slash[sl++] + 1) return false;
            c = spn[++i];
        }
        bt = j >= gateAt[Black]; wt = j <= gateAt[White];
        if (isNum(c)) { sp = c - '0'; continue; }


        if (c == '!')
        {
            urg = true;
            c = spn[++i];
        }
        else urg = false;

        switch (c) // will return false if one too many of any
        {          // piece type is specified by the SPN string
        case 'Q':  // nb style borrowed from chessx
            if (!setAt(NB[j++], WhiteBatyr, urg)) return false;
            if (wt) ++tw; break;
        case 'R':  // maybe faster than a loop check (?)
            if (!setAt(NB[j++], WhiteTura, urg))  return false;
            if (wt) ++tw; break;
        case 'B': // but a real pain if needing translation at some point
            if (!setAt(NB[j++], WhiteYalkyn, urg)) return false;
            if (wt) ++tw; break;
        case 'K':
            if (!setAt(NB[j++], WhiteBiy, urg))  return false;
            if (wt) ++tw; ++k_on[White]; break;
        case 'S':
            if (!setAt(NB[j++], WhiteShatra, urg)) return false;
            if (wt) ++tw; break;
        case 'q':
            if (!setAt(NB[j++], BlackBatyr, urg)) return false;
            if (bt) ++tb; break;
        case 'r':
            if (!setAt(NB[j++], BlackTura, urg))  return false;
            if (bt) ++tb; break;
        case 'b':
            if (!setAt(NB[j++], BlackYalkyn, urg)) return false;
            if (bt) ++tb; break;
        case 'k':
            if (!setAt(NB[j++], BlackBiy, urg))  return false;
            if (bt) ++tb; ++k_on[Black]; break;
        case 's':
            if (!setAt(NB[j++], BlackShatra, urg)) return false;
            if (bt) ++tb; break;
            // NB must add defunkt to dfstack...
        case 'y':
            setAt(NB[j], WasBatyr); m_dfs.push(NB[j++]);  break; // batYr
        case 'u':
            setAt(NB[j], WasTura); m_dfs.push(NB[j++]);   break; // tUra
        case 'i':
            setAt(NB[j], WasYalkyn); m_dfs.push(NB[j++]); break; // bIshop
        case 'p':
            setAt(NB[j], WasShatra); m_dfs.push(NB[j++]); break; // Pawn

        default: if (j < lsq) return false; // dodgy character
        }
        c = spn[++i];
    }
    if (k_on[White] != 1 || k_on[Black] != 1) return false;
    found = false;
 // side to move, obligatory
    while(!found)
    {
        c = spn[++i];
        switch(c)
        {
        case ' ': break;
        case 'w': m_stm = White; found = true; break;
        case 'b': m_stm = Black; found = true;
        }
    }
    if (!found) return false; found = false;
    m_sntm = !m_stm;
 // either temdek on or off, optional
    do
    {
        c = spn[++i];
        switch(c)
        {
        case ' ': break;
        case '-': found = true; break; // NB single hyphen cancels any field
        case 'T':
            m_temdek[White] = tw; found = true; break;
        case 't':
            m_temdek[Black] = tb; found = true;
        }
    } while(!found || (c != ' '));
    if (!found) return false; found = false;
 // transit square of a piece in capture sequence
    while(!found)
    {
        c = spn[++i];
        switch(c)
        {
        case ' ': break;
        case '-': found = true; break;
        default:
            j = i;
            while (isNum(c)) c = spn[++i]; 
            m_transit = NB[spn.mid(j, i - j).toInt()];
            found = true;
        }
    }
    if (!found) return false; found = false;
 // square of possible en passant capture
    while(!found)
    {
        c = spn[++i];
        switch(c)
        {
        case ' ': break;
        case '-': found = true; break;
        default:
            j = i;
            while (isNum(c)) c = spn[++i];
            int eps = spn.mid(j, i - j).toInt();
            if (epPossible(eps, Color(m_stm)))
            {
                m_epSquare = NB[eps];
                //found = true;
            }
            // XXX ignore illegal ep square
            //else return false;
            found = true;
        }
    }
    if (!found) return false; found = false;
  // half move clock
    while(!found)
    {
        c = spn[++i];
        switch(c)
        {
        case ' ': break;
        case '-': found = true; break;
        default:
            j = i;
            while (isNum(c)) c = spn[++i];
            m_halfMoves = spn.mid(j, i - j).toInt();
            found = true;
        }
    }
    if (!found) return false; found = false;
    
 // move number specification optional
    while (!isNum(c) && c != 0) c = spn[++i];
    if (c == 0) return true; // m_movenumber preset to 1
    if (!isNum(c)) return false;
    m_moveNumber = spn.mid(i).toInt();    
    if (m_moveNumber < 1) return false;
       
    return true;
}

QString nums = "0123456789";

QString SBoard::toSPN() const
{
    int piece, i, j = 0, sp = 0;
    QString spn;
    for (i = fsq; i <= lsq; i++)
    {
        piece = m_sb[NB[i]];
        if (piece)
        {
            if (sp)
            {
                spn += nums[sp]; sp = 0;
            }
            if (piece & URGENT) spn += '!';
            spn += pieceToChar(Piece(piece & 0xf));
        }
        else ++sp;
        if (i == spn_slash[j])
        {
            if (sp)
            {
                spn += nums[sp]; sp = 0;
            }
            spn += '/'; ++j;
        }
    }
    if (sp) spn += nums[sp]; // add in any trailing empty squares

 // side to move    
    spn += ' '; spn += m_stm == White? 'w' : 'b'; spn += ' ';

 // temdeks
    if (temdekOn(White)) spn += 'T'; if (temdekOn(Black)) spn += 't';
    if (temdekOff(White) && temdekOff(Black)) spn += '-'; spn += ' ';
 // piece in sequence
    if (inSequence()) spn += QString::number(transitAt());
    else spn += '-'; spn += ' ';
 // ep square
    if (m_epSquare) spn += QString::number(enPassantSquare());
    else spn += '-'; spn += ' ';
 // half move clock
    if (m_halfMoves) spn += QString::number(m_halfMoves);
    else spn += '-'; spn += ' ';
 // move number
    spn += QString::number(m_moveNumber);

//    qDebug() << spn;

    return spn;
}

/* MOVE GENERATION */
 // rules change for shatras on different ranks
inline int SBoard::sPhi(int s)
{
    switch (m_stm)
    {
    case White:
        if (Rank(s) < 7) return 0; // 5 or 6
        if (Rank(s) == 7) return 1;
        return 2;
    case Black:
        if (Rank(s) > 8) return 0; // 10 or 9
        if (Rank(s) == 8) return 1;
        return 2;
    }
    return 255; // (would be error if we got here)
}
 // waiting will promote if slider captured (for sntm!)
inline int SBoard::promoWaiting()
{
    switch (m_stm)
    {
    case White:
        if (m_sb[0x31] == BlackShatra) return 0x31;
        if (m_sb[0x41] == BlackShatra) return 0x41;
        if (m_sb[0x51] == BlackShatra) return 0x51;
        break;
    case Black:
        if (m_sb[0x3e] == WhiteShatra) return 0x3e;
        if (m_sb[0x4e] == WhiteShatra) return 0x4e;
        if (m_sb[0x5e] == WhiteShatra) return 0x5e;
    }
    return NoSquare;
}
 // capture blocked by temdek
inline bool SBoard::prohibited(int to, PieceType p)
{
//    bool fortW = Rank(to) < (p == Biy? 4 : 5);
//    bool fortB = Rank(to) > (p == Biy? 11 : 10);
    bool fortW = Rank(to) < (5  - (p == Biy));
    bool fortB = Rank(to) > (10 + (p == Biy));

    if (m_stm) { if (fortB && temdekOn(Black)) return true; }
    else         if (fortW && temdekOn(White)) return true;

    return false;
}
 // already generated as drop move
inline bool SBoard::duplicate(int to, bool fort)
{
    if (fort == White) { if (Rank(to) > 4 && Rank(to) < 8) return true; }
    else if (Rank(to) < 11 && Rank(to) > 7) return true;
    return false;
}
// for captures within fortresses
inline void SBoard::doCFlags(int from, int to, int cp)
{
    if (m_promoWait[m_sntm])
        if (pieceTypeAt(cp) <= Yalkyn) m_b |= PROMO_sntm;

    switch (m_stm)
    {
    case White:
        if (Rank(to) <= 4)
        {
            if (temdekOn(White)) { if (Rank(from) <= 4)  m_b |= FLIP_URGENT; }
        }
        else if (from == m_urgent[White]) m_b |= FLIP_URGENT;

        if (temdekOn(White))
            if (Rank(from) < 5 && Rank(to) > 4) m_b |= DECTDK;

        if (Rank(cp) > 10 && temdekOn(Black)) m_b |= DECTDK_sntm;
        break;

    case Black:
        if (Rank(to) >= 11)
        {
            if (temdekOn(Black)) { if (Rank(from) >= 11)  m_b |= FLIP_URGENT; }
        }
        else if (from == m_urgent[Black]) m_b |= FLIP_URGENT;

        if (temdekOn(Black))
            if (Rank(from) > 10 && Rank(to) < 11) m_b |= DECTDK;

        if (Rank(cp) < 5  && temdekOn(White)) m_b |= DECTDK_sntm;
    }
}
 // if dropping from home fort, add decTemdek flag if T on
inline bool SBoard::getDrops(int s, PieceType piece)
{
    bool t = s <= gateAt[White];
    int to, n = 0, at = NB[s];
    int h = t? 11 : 32; // top left corners of the two halves
    t ^= (m_stm != White); // our own fortress?
    if (t)
    {
        if (temdekOn(m_stm)) m_b |= DECTDK; // and Temdek on?
    }
    else if (piece == Shatra)
        return false; // shatras can't drop from enemy fort

    for (int i = 0; i < 21; i++)
    {    
        to = NB[h + i]; // Numeric to Board coords
        if (isVacant(to))
        {
            m_ml.add().genMove(at, to, piece, m_b); ++n;
        }
    }
    return n > 0;
}
 // 'teleports' from tower squares!
inline void SBoard::getPorts(int s)
{
    int to, at = NB[s], h = s < 31? 1 : 54; // top left of fortresses

    for (int i = 0; i < 9; i++)
    {
        to = NB[h + i];
        if (isVacant(to))
        {
            m_ml.add().genMove(at, to, Shatra, Rank(to) == sFinal[m_stm]?
                (m_b | PROMO) : m_b);
        }
    }
}
 // main move generator (for single vector)
inline void SBoard::getMoves(int at, PieceType piece, D d, bool doneFort)
{
    int to = at; bool s2 = false, fort;
 //   if (doneFort)
    fort = Rank(at) > 10;

    while (isVacant(to += d))
    {
        if (piece == Shatra)
        {
            if (Rank(to) == sFinal[m_stm])
                m_ml.add().genMove(at, to, Shatra, m_b | PROMO);
            else
            {
                s2 ^= true; // for two-square moves
                m_ml.add().genMove(at, to, Shatra, m_b);
            }
            if (Rank(at) != sFirst[m_stm] || !s2) break; // 2-move
        }
        else // other piece types
        {
            if(doneFort)
            {
                 if(!duplicate(to, fort))
                    m_ml.add().genMove(at, to, piece, m_b);
            }
            else {
                if (!prohibited(to, piece))
                    m_ml.add().genMove(at, to, piece, m_b);
            }
        }
        if (piece == Biy) break;
    }
}
 // evasions for biy only
inline void SBoard::getEvasions()
{
    bool doneFort = false;
    int at = m_biyAt[m_stm];
    int s = BN[at];

    if (isBiyOnTemdek(s) || isInFortress(s))
        doneFort = getDrops(s, Biy);

    for (int d = 0; d < 8; d++)
        getMoves(at, Biy, dir[d], doneFort);
}
 // capture -- checks for continuation, and flags any such in Move object
inline bool SBoard::getCapture
    (int at, PieceType piece, D d, int r, bool inFort)
{
    int to = at, to2, v_at, dd, pr, flags = m_b;
    D d2, dx = noD; bool c(false), ep(false); // c is capture found
    m_ext.clear();

    if (piece >= Biy) to += d; // biy or shatra, one square only
    else while (isVacant(to += d));
    
    ep = (to == m_epSquare && piece == Shatra); // increased legibility?

    if ((isOppositeColor(to) || ep) && isVacant(to + d)
      && (inFort || !prohibited(to + d, piece)))
    {
        c = true; // we got one
    
        v_at = ep? 
            (m_epVictim = to + (toMove() == White? -1 : 1)) : to;
        
        PieceType v = pieceTypeAt(v_at); // v is victim
        to += d;
        do
        {  // now look for a possible capture-continuation
            m_ext.append(to); pr = r;  // store to, reload piece rules
            
            for (dd = 0; dd < 8; dd++) // compass
            {
                d2 = dir[dd]; // d2 will scan the 'rays'
                if (!(pr & 1) || d2 == dx || d2 == -d)
                {
                	pr >>= 1; continue; // vector not available
                }
                to2 = to;
                if (piece >= Biy) to2 += d2;
                else while (isVacant(to2 += d2));
                if (!inFort && prohibited(to, piece)) // necessary check..
                    continue;

                if (isOppositeColor(to2)
                  && isVacant(to2 + d2) && to2 != v_at && v != Biy
                  && (inFort || !prohibited(to2 + d2, piece))) // here too
                {
                    doCFlags(at, to, v_at);
                    m_ml.add().genCapt(at, to, piece, v_at, v, m_b | C_CONT);
                    return c; // will continue - so no promo for shatras,
                }             // only one square allowed for sliders
                pr >>= 1;
            }
            dx = d; // block original vector after first loop
        } while (isVacant(to += d) && piece < Biy); // if slider, new to
        
        dd = -1;
        while (++dd < m_ext.count())
        {
            to = m_ext[dd];
            if (!inFort && prohibited(to, piece))
                return c;
            m_b = flags; doCFlags(at, to, v_at);
            
            m_ml.add().genCapt(at, to, piece, v_at, v,
                (piece == Shatra && Rank(to) == sFinal[m_stm])?
                    m_b | PROMO : m_b);
        }
    }    
    return c;
}                
 // cc==true  1) single square capture-continuation only (from)
 //    false  2) captures, if none generate moves for one square (from)
  // generate all move types
//           3) captures, if none generate all moves (fsq, lsq)
int SBoard::generate(bool cc, int first, int last) // last defaults to 0
{
    int s, at, r, pr, bstm( m_stm<<2 );
    bool c( !cc ), inFort, doneFort;
    m_promoWait[m_sntm] = promoWaiting();
    m_caps[0] = m_caps[1] = false;
    m_epVictim = NoSquare;
    m_ml.clear();
    if (m_biyAt[m_stm] == NoSquare) return 0; // biy was captured

    do
    {           
        for (s = (c? fsq : first);
             s <= (c? lsq : (last != NoSquare? last : first)); ++s)
        {
            at = NB[s];
            m_b = 0;
            if (isVacant(at) || isOppositeColor(at)) continue;

            Piece p = Piece(m_sb[at] & 0xf);
            PieceType pt = pieceType(p);
            inFort = isInHomeFort(s, m_stm);

 // see top of sboard.h for piece rules
            pr = r = pt != Shatra? pRules[pt - Batyr] : c || cc?
                        255 : sRules[m_stm][sPhi(at)];

            if (c || cc) // captures and continuations
            {
                if (cc && pt == Biy) // pass move for biy already in sequence
                    m_ml.add().genMove(at, at, Biy, 0);

                // (we would prefer not to have to do this...)
                D td1( noD ), td2( noD );
                if (m_epSquare && pt == Shatra)
                { // force ep to be examined first
                    if (m_epSquare == at + (m_stm? sw : ne) ||
                        m_epSquare == at + (m_stm? se : nw))
                    {   td1 = (D)(m_epSquare - at);
                        if (getCapture(at, pt, td1, pr, false))
                        { // if the ep succeeded, block two directions
                            td2 = (D)(m_epVictim - at);
                            m_epVictim = NoSquare;
                            m_caps[1] = true;// non-biy capture
                        }
                    }
                }

                for (int d = 0; d < 8; ++d) // eight compass points
                {
                    if (!r) break; // finished all available vectors
                    if (td1 == dir[d] || td2 == dir[d]) continue;
                    m_b = bstm; // byte flags, preset to STM for each vector

                    if (r & 1)
                        if (getCapture(at, pt, dir[d], pr, inFort))
                            m_caps[pt == Biy? 0 : 1] = true;
                    r >>= 1;
                }
            }
            else // drop, teleport or ordinary moves
            {
                m_b = bstm; doneFort = false;
                
                if (isInFortress(s) || isBiyOnTemdek(s)) doneFort = getDrops(s, pt);
                else if (pt == Shatra)
                    if (s == lTower[m_sntm] || s == rTower[m_sntm]) getPorts(s);

                if (!(doneFort && isReserve(s)))
                {
                    for (int d = 0; d < 8; ++d)
                    {
                        if (!r) break;
                        m_b = bstm;

                        if (r & 1) getMoves(at, pt, dir[d], doneFort);
                        r >>= 1;
                    }
                }
            }
        }
        c ^= true;
        if (!c) // if we found captures, we are done
        {
            if (m_ml.count())
            {
                if (onlyBiyCaptures()) getEvasions();
                else if (m_epVictim)
                    m_sb[m_epVictim] = m_stm? WhiteShatra : BlackShatra;
                return m_ml.count();
            }
            else if (m_allurgent & bFort[m_stm]) // no captures, check for urgent pieces
            {
                bb au = m_allurgent;
                Square usq;
                m_b |= FLIP_URGENT;
                do {
                    at = NB[usq = bitScanForward(au)];
//                    if (m_urgent[m_stm] == NoSquare)
//                        m_urgent[m_stm] = at;
                    getDrops(usq, PieceType(m_sb[at]));
                    
                } while (au &= (au - 1));
                return m_ml.count();
            }
        }
    } while (!c); // 2nd time round for move generation
    return m_ml.count();
}

 // for all macro lovers :)
#define getAt                       \
    if (alph) temp = (c - '`')<<4,  \
        c = *s++;                   \
    else temp = 0;                  \
    at = c - '0';                   \
    c = *s++;                       \
    if (isNum(c)) {                 \
        at = (at * 10) + (c - '0'); \
        c = *s++;                   \
    }                               \
    at += temp

 // parseMove takes a string (QString) in either alphnum or num notation
Move SBoard::parseMove(const QString& algebraic)
{
    const QByteArray& bs(algebraic.toLatin1());
    const char* lann = bs.constData();
    const char* s = lann;
    char c = *s++;
    Move move; // zero is illegal move
    int from, to, at, temp, type = Shatra, alph;

    if (c != ':')
    {    
        switch (c)
       {
        case 'Q': type = Batyr;  c = *s++; break;
        case 'R': type = Tura;   c = *s++; break;
        case 'B': type = Yalkyn; c = *s++; break;
        case 'K': type = Biy;    c = *s++; break;
 // pass option
        case 'p':
            if (algebraic != "pass") return move;//QObject::tr("pass")) return move;
            to = m_biyAt[m_stm]; // NB woz *m_transit* ???
            return prepareMove(to, to);

        default: 
            if (isUpper(c)) return move;
        }

        alph = (int)isAlpha(c); // string is alphanumeric or numeric?

        getAt;
        from = alph? at : NB[at];
        //if (from > 144) return Move();
        if (!BN[from]) return move;

        if (alph && type != pieceTypeAt(from)) return move;
        if (c != '-' && c != ':') return move;
    }
    else
    {
        from = m_to; // NB ***from must = last to***
        if (from == NoSquare) return move;
        alph = (int)isAlpha(*s);
    }
    c = *s++;
    getAt;
    to = alph? at : NB[at]; if (!BN[to]) return move;

    type = Empty;

    temp = strlen(lann) - (s - lann);
    if (temp >= 0 && c != 'x' && c != 'X')
    {
        if (c == '=' || c == '(') c = *s++; // allow for "=Q" & "(Q)"
        switch (toupper(c))
        {
        case 'q': case 'Q': type = Batyr; break;
        case 'r': case 'R': type = Tura; break;
        case 'b': case 'B': type = Yalkyn; break;
        default: return move;
        }
    }

    move = prepareMove(from, to);
    if (move.isPromotion())
    {
        if (type)
            move.setPromoted(m_offBoard[type + PC[m_stm]]? type : 0);
        else move.setPromoted(Empty);
    }
    return move;
}
#undef getAt

bool SBoard::doMove(const Move& m)
{
    ++m_halfMoves;
 //   if (m_halfMoves > 50)
//        bool debug = true; // XXX
    m_from = m.from();
    m_to = m.to();

    int p = m_sb[m_from];
    Piece piece = Piece(p & 0xf);
    m_sb[m_from] = Empty; m_sb[m_to] = p; // NB pass (from == to)
    m_epSquare = m_transit = m_latePromo = NoSquare;

    if (m.flipsUrgent())
        doUrgentAt(m);
    else if (p & URGENT)
    {
        doUrgentAt(m);
        setUrgentAt(m_to);
    }
    
    if (m.decsTemdek())
    {
        --m_temdek[m_stm]; // boardview may delay this
        m_temdekPending[m_stm] = temdekOff(m_stm);
    }
    else if (pieceType(piece) == Shatra)
    {
        if (abs(m_to - m_from) == 2
        && (Rank(m_from) == 5 || Rank(m_from) == 10)
        && File(m_to) > 1 && File(m_to) < 7
        && !m.isCapture())
            m_epSquare = m_stm? m_to + 1 : m_to - 1;
            
        if (m.isPromotion())
        {
            ++m_offBoard[piece];
            m_sb[m_to] = piece = m.promotedPiece();
            --m_offBoard[piece];
            m_halfMoves = 0;
        }
    }

    if (m.isCapture())
    {
        m_halfMoves = 0;
        Piece victim = m.capturedPiece();
        PieceType v = pieceType(victim);
        int victim_at = m.capturedAt();
        if (m_sb[victim_at] & URGENT)
            clearUrgentAt(victim_at);
        m_sb[victim_at] = Empty; --m_pieceCount[m_sntm];
        if (v == Biy) m_biyAt[m_sntm] = Empty;

        if (m.isPromoSntm())
        {
            m_promoWait[m_sntm] = promoWaiting();
            ++m_offBoard[Shatra + PC[m_stm]];
            m_sb[m_promoWait[m_sntm]] = victim; // no offboard change :)
            m_latePromo = m_promoWait[m_sntm];
        }
        else ++m_offBoard[victim];
        
        if (m.decsTemdekSntm()) --m_temdek[m_sntm];

        if (m.willContinue())
        {
            m_sb[victim_at] = Was + v;
            m_dfs.push(victim_at);
            m_transit = m_to;
        }
        else
        {
            m_sb[victim_at] = Empty;
            m_temdekPending[m_stm] = false;
        }
    }
    else m_temdekPending[m_stm] = false;

    if (pieceType(piece) == Biy)
    {       
        m_biyAt[m_stm] = m_to;
        if (temdekOn(m_stm))
            if ((m_to == gateAtB[m_stm]) && (m_to != m_from))
                ++m_temdek[m_stm];
    }
    m_lstm = m_stm;
    if (!inSequence())
    { // NB if we were inseq, clear up defunkts
        if (m.wasInSequence())
            while (m_dfs.count()) m_sb[m_dfs.remove()] = Empty;
            
        if (m_stm == Black) ++m_moveNumber;        
        swapToMove(); // NB this and below are the only two places...
        return false;
    }
    m_sntm = m_stm ^ 1;
    return true;
}
 // NB undoMove is incapable of resurrecting a list of captured pieces
void SBoard::undoMove(const Move& m)
{
    if (m_halfMoves) --m_halfMoves;
    m_from = m.from();
    m_to = m.to();

    int p = m_sb[m_to];
    Piece piece = Piece(p & 0xf);
    m_sb[m_to] = Empty; m_sb[m_from] = p;

    m_lstm = m_stm;
    if (!inSequence())
    {
        swapToMove(); // NB ...where m_stm is updated ***emit state change???
        if (m_stm == Black) --m_moveNumber;
    }
    m_sntm = m_stm ^ 1; // side not to move
    
    if (m.decsTemdek()) ++m_temdek[m_stm];
    else if (m.isPromotion())
    {
        ++m_offBoard[piece];
        m_sb[m_from] = piece = m_stm? BlackShatra : WhiteShatra;
        --m_offBoard[piece];        
    }
    if (m.isCapture())
    {
        Piece victim = m.capturedPiece();
        int victim_at = m.capturedAt();
        m_sb[victim_at] = victim; ++m_pieceCount[m_sntm];
        if (pieceType(victim) == Biy) m_biyAt[m_sntm] = victim_at;
        
        if (m.isPromoSntm())
        {
            --m_offBoard[Shatra + PC[m_stm]];
            m_sb[m.o] = Shatra + PC[m_stm];
        }
        else --m_offBoard[victim];

        if (m.decsTemdekSntm()) ++m_temdek[m_sntm];
    }
    if (pieceType(piece) == Biy)
    {       
        m_biyAt[m_stm] = m_from;
        if (temdekOn(m_stm))
            if ((m_to == gateAtB[m_stm]) && (m_to != m_from))
                --m_temdek[m_stm];
    }
    m_epSquare = m.oldEPAvail();
    m_transit = m.oldTransit();
    m_urgent[m_stm] = m.oldUrgent();
    if (m_urgent[m_stm])
        m_allurgent |= bSq(urgentAt());
    if (m_dfs.count()) m_dfs.pop();

    m_to = inSequence()? m_from : NoSquare;
    m_from = NoSquare;
}

Move SBoard::prepareMove(const int from, const int to)
{
    if (!m_movesLoaded)
    {
        generate(inSequence(), BN[from]);
    }
    Move move;
    int p = -1; bool found = false;
    while (++p < m_ml.count())
    {
        if (from == m_ml[p].from() && to == m_ml[p].to())
        {
            found = true; move = m_ml[p]; break;
        }
    }
    if (found)
    {
        move.e |= m_epSquare;
        move.x |= m_transit;
        move.g |= m_urgent[m_stm];
        move.o |= m_latePromo;
    }
    return move;
}

Move SBoard::prepareMove(const int from, const int to) const // board coords
{
    Move move;
    if (!m_movesLoaded) return move;
    int p = -1; bool found = false;
    while (++p < m_ml.count())
    {
        if (from == m_ml[p].from() && to == m_ml[p].to())
        {
            found = true; move = m_ml[p]; break;
        }
    }
    if (found)
    {
        move.e |= m_epSquare;
        move.x |= m_transit;
        move.g |= m_urgent[m_stm];
        move.o |= m_latePromo;
    }
    return move;
}

bool SBoard::canBeReachedFrom(const SBoard& target) const
{
    if (m_pieceCount[White] > target.m_pieceCount[White] ||
        m_pieceCount[Black] > target.m_pieceCount[Black] ||
        m_offBoard[WhiteShatra] < target.m_offBoard[WhiteShatra] ||
        m_offBoard[BlackShatra] < target.m_offBoard[BlackShatra])
        return false;
    return true;
}

void SBoard::getReachableSquares(Square sfrom, std::vector<Square>& vec) const
{
    if (!m_movesLoaded) return;

    int from = NB[sfrom];
    for (int i = 0; i < m_ml.count(); ++i)
    {
        if (m_ml[i].from() == from)
            vec.push_back(BN[m_ml[i].to()]);
    }
}

void SBoard::getMoveSquares(std::vector<SquareMove>& vec) const
{
    if (!m_movesLoaded) return;

    for (int i=0; i<m_ml.count(); ++i)
    {
        vec.push_back(SquareMove(BN[m_ml[i].from()], BN[m_ml[i].to()]));
    }
}



 // NB the following are not member functions
/* Init board values before starting */
void SBoardInit()
{ 
    SBoardInitRun = true;
    standardPosition.fromSPN
        ("SQSSRSBRB/K/SSSSSSS/7/7/7/7/sssssss/k/brbsrssqs w Tt - - - 1");
}

SBoard getStandardPosition()
{
    SBoard b;
    b.fromSPN
        ("SQSSRSBRB/K/SSSSSSS/7/7/7/7/sssssss/k/brbsrssqs w Tt - - - 1");
        //("SQSSRSBRB/K/SSSSSSS/1s3s1/4s2/7/7/2sss1s/k/brbsrssqs w Tt - - - 1");
        //("SQSSRSBRB/K/SSS2SS/7/7/4S2/3S3/sssssss/k/brbsrssqs w Tt - - - 1");
    return b;
}


QString SBoard::dumpAll(const SBoard * comp) const
{
    std::stringstream str;

#define SB_SB_COMP(what__) \
    (comp && comp->what__ != what__ ? "*" : "")

#define SB_SB_DEBUG(what__) \
    str << std::left << std::setw(20) << #what__ " " << SB_SB_COMP(what__) << what__ << std::endl;

#define SB_SB_DEBUG_A(what__, num__) \
    str << std::left << std::setw(20) << #what__ " "; \
    for (int i=0; i<(num__); ++i) str << SB_SB_COMP(what__[i]) << what__[i] << " "; \
    str << std::endl;

#define SB_SB_DEBUG_A_T(what__, num__, typecast__) \
    str << std::left << std::setw(20) << #what__ " "; \
    for (int i=0; i<(num__); ++i) str << SB_SB_COMP(what__[i]) << (typecast__)what__[i] << " "; \
    str << std::endl;

    SB_SB_DEBUG_A_T(m_sb, 144, int);
    SB_SB_DEBUG(m_movesLoaded);
    SB_SB_DEBUG(m_stm);
    SB_SB_DEBUG(m_transit);
    SB_SB_DEBUG(m_allurgent);
    SB_SB_DEBUG(m_sntm);
    SB_SB_DEBUG(m_lstm);
    SB_SB_DEBUG(m_from);
    SB_SB_DEBUG(m_to);
    SB_SB_DEBUG_A(m_biyAt, 2);
    SB_SB_DEBUG_A(m_temdek, 2);
    SB_SB_DEBUG(m_epSquare);
    SB_SB_DEBUG(m_epVictim);
    SB_SB_DEBUG(m_latePromo);
    SB_SB_DEBUG(m_moveNumber);
    SB_SB_DEBUG(m_halfMoves);
    SB_SB_DEBUG_A(m_offBoard,12);
    SB_SB_DEBUG_A(m_pieceCount,2);
    SB_SB_DEBUG_A(m_promoWait,2);
    SB_SB_DEBUG(m_b);
    SB_SB_DEBUG_A(m_urgent,2);
    SB_SB_DEBUG_A(m_caps,2);
    SB_SB_DEBUG_A(m_temdekPending,2);
    //urstack<int,16> m_dfs;     // stack for defunkt pieces - NB no real undo
    //urvct<int,16> m_ext;       // vector for capture extensions
    //BoardList m_ml;            // list of generated moves

#undef SB_SB_COMP
#undef SB_SB_DEBUG
#undef SB_SB_DEBUG_A
#undef SB_SB_DEBUG_A_T

    return QString::fromStdString(str.str());
}
