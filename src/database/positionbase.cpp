/** @file positionbase.cpp

    @brief

    <p>(c) 2014, stefan.berke@modular-audio-graphics.com</p>

    <p>created 5/9/2014</p>
*/

#include <iostream>
#include <fstream>

#include "positionbase.h"


namespace
{
    #define x 0
    const int boardPos[14*7] = {

        x,  x,  x,  x,  17, 24, 31, 38, 45, 52, x,  x,  x,  x,
        x,  x,  x,  x,  16, 23, 30, 37, 44, 51, x,  x,  x,  x,
        3,  6,  9,  x,  15, 22, 29, 36, 43, 50, x,  56, 59, 62,
        2,  5,  8,  10, 14, 21, 28, 35, 42, 49, 53, 55, 58, 61,
        1,  4,  7,  x,  13, 20, 27, 34, 41, 48, x,  54, 57, 60,
        x,  x,  x,  x,  12, 19, 26, 33, 40, 47, x,  x,  x,  x,
        x,  x,  x,  x,  11, 18, 25, 32, 39, 46, x,  x,  x,  x,
    };
    #undef x

    const char pieceChar[] = {
        '.',
        'Q', 'T', 'Y', 'B', 'S',
        'q', 't', 'y', 'b', 's' };
}


PositionBase::PositionBase()
{
    clear();
}

void PositionBase::clear()
{
    entries_.clear();
    num_ = 0;
}

#ifdef SBASE_PRESENT

void PositionBase::addDatabase(Database &db)
{
    qDebug() << "adding database with " << db.count() << "games";

    for (int i=0; i<db.count(); ++i)
    {
        Game g;
        if (!db.loadGame(i, g))
        {
            qDebug() << "error loading game " << i;
            continue;
        }

        if (g.moveCount() < 5)
        {
            qDebug() << "skipping short game " << i;
            continue;
        }

        if (g.result() == SHATRA::ResultUnknown)
        {
            qDebug() << "skipping unknown result game " << i;
            continue;
        }
        /*
        if (g.result() == SHATRA::Draw)
        {
            qDebug() << "skipping draw result game " << i;
            continue;
        }*/

        addGame(g);
    }

    qDebug() << "added" << num_ << "positions";
}

void PositionBase::addGame(const Game &g)
{
    qDebug() << "adding game";

    SHATRA::SBoard b;
    b.setStandardPosition();

    int delta = 0;
    for (int i=1; ; ++i)
    {
        const SHATRA::Move m = g.move(i);
        if (!m.isLegal())
        {
            qDebug() << "break at ply " << i;
            break;
        }
        /*
        if (!b.parseMove(m.toNumeric()).isLegal())
        {
            qDebug() << "error executing move " << m.toNumeric() << " (ply " << i << ")";
            break;
        }*/

        b.doMove(m);
        /*
        if (!b.fromSPN(b.toSPN()))
        {
            qDebug() << "error executing move " << m.toNumeric() << " (ply " << i << ")";
            break;
        }*/

        if (i>=5 && !b.inSequence() && delta <= 0)
        {
            addPosition(b, g.result());
            delta = 4;
        }
        --delta;
    }
}

void PositionBase::addPosition(const SHATRA::SBoard& b, SHATRA::Result r)
{
    // append an entry
    size_t pos = entries_.size();
    entries_.resize(entries_.size() + entrySize());
    // and go to start of it
    uint8_t * data = &entries_[pos];

    memset(data, 0, sizeof(uint8_t) * entrySize());

    // reward
    *data++ = r == SHATRA::WhiteWin ? 1 : r == SHATRA::BlackWin ? 2 : 0;

#if 1
    for (int i=SHATRA::fsq; i<=SHATRA::lsq; ++i)
    {
        *data++ = b.pieceAt(SHATRA::NB[i]);
    }
    std::cout << std::endl;
#else
    // own positions
    for (int piece = SHATRA::WhiteBatyr; piece <= SHATRA::WhiteShatra; ++piece)
    for (int i=0; i<14*7; ++i)
    {
        int bp = boardPos[i];
        if (bp == 0)
            *data++ = 0;
        else
            *data++ = b.pieceAt(bp) == piece;
    }

    // other positions
    for (int piece = SHATRA::BlackBatyr; piece <= SHATRA::BlackShatra; ++piece)
    for (int i=0; i<14*7; ++i)
    {
        int bp = boardPos[i];
        if (bp == 0)
            *data++ = 0;
        else
            *data++ = b.pieceAt(bp) == piece;
    }
#endif

    ++num_;

    //dumpEntry(num_-1);
}

#endif

bool PositionBase::saveFile(const std::string& filename)
{
    std::ofstream out;
    out.open(filename.c_str(), std::ios_base::out);
    if (!out.is_open())
        return false;

    out << entries_.size();
    out.write(reinterpret_cast<const char*>(&entries_[0]), entries_.size());

    return true;
}

bool PositionBase::loadFile(const std::string& filename)
{
    std::ifstream in;
    in.open(filename.c_str(), std::ios_base::in);
    if (!in.is_open())
        return false;

    clear();

    size_t num;
    in >> num;
    entries_.resize(num);
    in.read(reinterpret_cast<char*>(&entries_[0]), num);

    return true;
}

void PositionBase::dumpEntry(size_t index, std::ostream& out) const
{
    const uint8_t* e = entry(index);

    out << "result: " << (int)e[0] << "\n";
    for (int j=0; j<14; ++j)
    {
        for (int i=0; i<7; ++i)
        {
            int bp = boardPos[(6-i)*14+j];
            if (bp == 0)
                out << "  ";
            else
                out << pieceChar[e[bp]] << " ";
        }
        out << std::endl;
    }
}
