/** @file positionbase.h

    @brief

    <p>(c) 2014, stefan.berke@modular-audio-graphics.com</p>

    <p>created 5/9/2014</p>
*/

#ifndef POSITIONBASE_H
#define POSITIONBASE_H

#include <QObject>
#include "game.h"
#include "database.h"

/** Container for positions and scores. */
class PositionBase : public QObject
{
    Q_OBJECT
public:
    explicit PositionBase(QObject *parent = 0);

    static int entryLength() { return 1+62*2; }
    int numPositions() const { return num_; }
    const float * position(int index) const { return &data_[index * entryLength()]; }

signals:

public slots:

    void clear();

    void addDatabase(Database&);

    void addGame(const Game&);

    void addPosition(const SHATRA::SBoard&, SHATRA::Result);

    bool saveFile(const QString filename);
    bool loadFile(const QString filename);

protected:

    std::vector<float> data_;
    int num_;
};

#endif // POSITIONBASE_H
