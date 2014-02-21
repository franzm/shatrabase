/***************************************************************************
 *                           Shatrabase                                    *
 *   (C) 2012-2014 AFKM <francis@afkm.com>                                 *
 *                   derived from ChessX with thanks                       *
 *   (C) 2005 Michal Rudolf <mrudolf@kdewebdev.org>                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef BOARDPAINTER_H
#define BOARDPAINTER_H

#include <QGraphicsView>

#include "../database/common.h"

class QGraphicsScene;

class BoardTheme;

/** Basically a paint backend for BoardView which also
    translates between coordinates and squares/pieces. */
class BoardPainter : public QGraphicsView
{
    Q_OBJECT
public:
    explicit BoardPainter(BoardTheme * theme, QWidget *parent = 0);

signals:
    void signalSquareClicked(Square sq);

public slots:

    // ________ PROTECTED ____________
protected:

    // --------- events --------------

    void mousePressEvent(QMouseEvent *event);

    // --------- internal ------------

    void createBoard_();

    // ----------- member ------------

    BoardTheme * m_theme;

    QGraphicsScene * m_scene;

    int m_size;
};

#endif // BOARDPAINTER_H
