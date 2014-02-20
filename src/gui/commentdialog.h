/***************************************************************************
 *                           Shatrabase                                    *
 *   (C) 2012-2014 AFKM <francis@afkm.com>                                 *
 *                   derived from ChessX with thanks                       *
 *   (C) 2010 Michal Rudolf <mrudolf@kdewebdev.org>                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef COMMENTDIALOG_H
#define COMMENTDIALOG_H

#include "ui_commentdialog.h"

class CommentDialog : public QDialog {
	 Q_OBJECT
public:
	 CommentDialog(QWidget *parent = 0);
     void setText(QString text);
	 QString text() const;

protected slots:
     void clearTimeSlot();
     void clearTextSlot();

private:
	 Ui::CommentDialog ui;
     static bool lastTimeWasEgt;
};

#endif // COMMENTDIALOG_H
