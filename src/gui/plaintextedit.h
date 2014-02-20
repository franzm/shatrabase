/***************************************************************************
 *                           Shatrabase                                    *
 *   (C) 2012-2014 AFKM <francis@afkm.com>                                 *
 *                   derived from ChessX with thanks                       *
 *   Copyright (C) 2010 by Michal Rudolf <mrudolf@kdewebdev.org>           *
 ***************************************************************************/

#ifndef PLAINTEXTEDIT_H
#define PLAINTEXTEDIT_H

#include <QtGui>
#include <QPlainTextEdit>

class PlainTextEdit : public QPlainTextEdit
{
public:
	 PlainTextEdit(QWidget* parent = 0);
protected:
	 virtual void keyPressEvent(QKeyEvent* event);
};

#endif // PLAINTEXTEDIT_H
