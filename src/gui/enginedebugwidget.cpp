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

#include "enginedebugwidget.h"

#include <QMenu>
#include <QAction>

EngineDebugWidget::EngineDebugWidget(QWidget *parent) :
    QTextBrowser(parent)
{
    // enable contextmenu
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(QPoint)),
                this, SLOT(showContextMenu(const QPoint&)));
}


void EngineDebugWidget::slotEngineDebug(Engine * engine, Engine::DebugType t, const QString& str)
{
    Q_UNUSED(engine);

    QString prefix;
    switch (t)
    {
    case Engine::D_ToEngine: setTextColor(Qt::black); prefix = "<-- "; break;
    case Engine::D_FromEngine: setTextColor(Qt::darkGreen); prefix = "--> "; break;
    case Engine::D_Error: setTextColor(Qt::darkRed); prefix = "*** "; break;
    case Engine::D_Debug: setTextColor(Qt::gray); prefix = "### "; break;
    }

    append(QString("%1: %2 %3")
           .arg(QTime::currentTime().toString())
           .arg(prefix)
           .arg(str));
}

void EngineDebugWidget::showContextMenu(const QPoint& pos)
{
    QMenu * m = new QMenu(this);

    QAction* a = m->addAction(tr("Clear"));
    connect(a, SIGNAL(triggered()), SLOT(clear()));

    m->exec(mapToGlobal(pos));

    m->deleteLater();
}
