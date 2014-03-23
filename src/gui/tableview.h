/***************************************************************************
 *                           Shatrabase                                    *
 *   (C) 2012-2014 AFKM <francis@afkm.com>                                 *
 *                   derived from ChessX with thanks                       *
 *     QTableView with some adjustments like storing header layout         *
 *        (C) 2007 Michal Rudolf <mrudolf@kdewebdev.org>                   *
 *   Copyright (C) 2012 by Jens Nissen jens-chessx@gmx.net                 *
 ***************************************************************************/

#ifndef TABLEVIEW_H
#define TABLEVIEW_H

#include <QtCore>
#include <QTableView>
#include <QTreeView>

/**
	The TableView class is a specialized version of QTableView with automatic
	header layout saving.

    It also let's the user move and hide the header sections.
*/

class TableView : public QTableView
{
Q_OBJECT
public:
   TableView(QWidget *parent = 0);
	~TableView();
public slots:
	/* Store current configuration. */
	void saveConfig();
	/* Restore current configuration. */
    virtual void slotReconfigure();
    /* Show context menu on header view */
    void ShowContextMenu(const QPoint&);
};

#endif
