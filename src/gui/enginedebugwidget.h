/***************************************************************************

Copyright (C) 2014  stefan.berke @ modular-audio-graphics.com

This source is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either
version 3.0 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License
along with this software; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA

****************************************************************************/

#ifndef ENGINEDEBUGWIDGET_H
#define ENGINEDEBUGWIDGET_H

#include <QTextBrowser>

#include "engine.h"


class EngineDebugWidget : public QTextBrowser
{
    Q_OBJECT
public:
    explicit EngineDebugWidget(QWidget *parent = 0);

signals:

public slots:

    /** Add text to debug output */
    void slotEngineDebug(Engine * engine, Engine::DebugType t, const QString& str);

    void showContextMenu(const QPoint& pos);

};

#endif // ENGINEDEBUGWIDGET_H
