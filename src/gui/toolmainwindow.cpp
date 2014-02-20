/****************************************************************************
 *                           Shatrabase                                     *
 *   (C) 2012-2014 AFKM <francis@afkm.com>                                  *
 *                   derived from ChessX with thanks                        *
 *   Copyright (C) 2012 by Jens Nissen jens-chessx@gmx.net                  *
 ****************************************************************************/

#include "settings.h"
#include "toolmainwindow.h"
#include <QtCore>

ToolMainWindow::ToolMainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setWindowFlags(Qt::Widget);
}

void ToolMainWindow::saveConfig()
{
    // saving subwindows does not work anymore as of Qt 4.4
}

void ToolMainWindow::slotReconfigure()
{
    // restoring subwindows does not work anymore as of Qt 4.4
}

