/***************************************************************************
 *                           Shatrabase                                    *
 *   (C) 2012-2014 AFKM <francis@afkm.com>                                 *
 *                   derived from ChessX with thanks                       *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

/** @file ushienginetester.h

    @brief fast low-level ushi engine vs. engine testsuite

    <p>created 5/1/2014</p>
*/

#ifndef USHIENGINETESTER_H
#define USHIENGINETESTER_H

#include <QObject>
#include <QProcess>
#include <QString>

#include "common.h"

class USHIEngineTester : public QObject
{
    Q_OBJECT

public:
    explicit USHIEngineTester(QObject * parent = 0);
    virtual ~USHIEngineTester();

    // -------- setup ----------

    void setBinary(int stm, QString filename) { filename_[stm] = filename; }

    // ------- execute ---------

    bool start();

private slots:

    void eStart1_() { eStart_(White); }
    void eStart2_() { eStart_(Black); }

    void eFinish1_() { eFinish_(White); }
    void eFinish2_() { eFinish_(Black); }

    void eError1_(QProcess::ProcessError e) { eError_(White, e); }
    void eError2_(QProcess::ProcessError e) { eError_(Black, e); }

    void ePoll1_() { ePoll_(White); }
    void ePoll2_() { ePoll_(Black); }

private:

    bool startEngine_(int stm);

    // ----- slots -----

    void eStart_(int stm);
    void eFinish_(int stm);
    void eError_(int stm, QProcess::ProcessError);
    void ePoll_(int stm);

    QString filename_[2];
    QProcess engine_[2];
};

#endif // USHIENGINETESTER_H
