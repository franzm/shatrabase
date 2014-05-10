/** @file positionbase.cpp

    @brief

    <p>(c) 2014, stefan.berke@modular-audio-graphics.com</p>

    <p>created 5/9/2014</p>
*/

#include <iostream>

#include <QSaveFile>

#include "positionbase.h"

PositionBase::PositionBase(QObject *parent) :
    QObject(parent)
{
}

void PositionBase::clear()
{
    data_.clear();
    num_ = 0;
}

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

        if (g.result() == ResultUnknown)
        {
            qDebug() << "skipping unknown result game " << i;
            continue;
        }

        if (g.result() == Draw)
        {
            qDebug() << "skipping draw result game " << i;
            continue;
        }

        addGame(g);
    }
}

void PositionBase::addGame(const Game &g)
{
    qDebug() << "adding game";

    SBoard b;
    b.setStandardPosition();

    for (int i=1; ; ++i)
    {
        const Move m = g.move(i);
        if (!m.isLegal())
        {
            qDebug() << "break at ply " << i;
            break;
        }

        if (!b.parseMove(m.toNumeric()).isLegal())
        {
            qDebug() << "error executing move " << m.toNumeric() << " (ply " << i << ")";
            break;
        }

        b.doMove(m);
        b.fromSPN(b.toSPN());

        if (i>=5)
            addPosition(b, g.result());
    }
}

void PositionBase::addPosition(const SBoard & b, Result r)
{
    // append an entry
    size_t pos = data_.size();
    data_.resize(data_.size() + entryLength());
    // and go to start of it
    float * data = &data_[pos];

    *data++ = r == WhiteWin? 0.9 : 0.1;

    for (int i=0; i<62; ++i)
    {
        const Piece p = b.pieceAt(i+1);
        *data++ = isValidPiece(p)? 1 : 0;
    }

    for (int i=0; i<62; ++i)
    {
        const Piece p = b.pieceAt(i+1);
        *data++ = isWhite(p)? 1 : 0;
    }

    num_++;
}


bool PositionBase::saveFile(const QString filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly))
        return false;

    QDataStream out(&file);
    for (size_t i=0; i<data_.size(); ++i)
        out << data_[i];

    return true;
}

bool PositionBase::loadFile(const QString filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly))
        return false;

    clear();

    float f;
    QDataStream in(&file);
    while (!in.atEnd())
    {
        in >> f;
        data_.push_back(f);
    }

    return true;
}
