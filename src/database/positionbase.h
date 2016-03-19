/** @file positionbase.h

    @brief

    <p>(c) 2014, stefan.berke@modular-audio-graphics.com</p>

    <p>created 5/9/2014</p>
*/

#ifndef POSITIONBASE_H
#define POSITIONBASE_H

#include <stdint.h>
#include <vector>
#include <iostream>

#ifdef SBASE_PRESENT
#   include "game.h"
#   include "database.h"
#else
    namespace SHATRA {
        enum Piece
        {
            EmptyPiece,
                WhiteBatyr, WhiteTura, WhiteYalkyn, WhiteBiy, WhiteShatra,
                BlackBatyr, BlackTura, BlackYalkyn, BlackBiy, BlackShatra,
                WasBatyr, WasTura, WasYalkyn, InvalidPiece, WasShatra
        };
    }
#endif


/** Container for positions and scores.
    Experimental thing for training NNs */
class PositionBase
{
public:

    explicit PositionBase();

    int numPositions() const { return num_; }
    int entrySize() const { return 1 + 62; }

    const uint8_t * entry(size_t index) const { return &entries_[index * entrySize()]; }


    /** Each position is coded as,
        1 reward +
        for each side: 5 (14*7) planes for each piece,
        shatra boards are placed inside the 14*7 rectangle
        */
    static int stateSize() { return 1 + (14*7)*5*2; }

    void dumpEntry(size_t index, std::ostream& out = std::cout) const;

    void clear();

#ifdef SBASE_PRESENT
    void addDatabase(Database&);

    void addGame(const Game&);

    void addPosition(const SHATRA::SBoard&, SHATRA::Result);
#endif

    bool saveFile(const std::string& filename);
    bool loadFile(const std::string& filename);

protected:

    std::vector<uint8_t> entries_;
    int num_;
};

#endif // POSITIONBASE_H
