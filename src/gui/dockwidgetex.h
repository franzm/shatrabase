/***************************************************************************
 *                           Shatrabase                                    *
 *   (C) 2012-2014 AFKM <francis@afkm.com>                                 *
 *                   derived from ChessX with thanks                       *
 *   Copyright (C) 2012 by Jens Nissen jens-chessx@gmx.net                 *
 ***************************************************************************/

#ifndef DOCKWIDGETEX_H
#define DOCKWIDGETEX_H

#include <QDockWidget>
#include <QDebug>

class DockWidgetEx : public QDockWidget
{
    Q_OBJECT
public:
    explicit DockWidgetEx(const QString &title, QWidget *parent = 0, Qt::WindowFlags flags = 0);
    explicit DockWidgetEx(QWidget *parent = 0, Qt::WindowFlags flags = 0);

signals:
    
public slots:

protected:
    void showEvent ( QShowEvent * event );
};

#endif // DOCKWIDGETEX_H
