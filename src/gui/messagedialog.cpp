/***************************************************************************
 *                           Shatrabase                                    *
 *   (C) 2012-2014 AFKM <francis@afkm.com>                                 *
 *                   derived from ChessX with thanks                       *
 *	 (C) 2007 Michal Rudolf <mrudolf@kdewebdev.org>                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "messagedialog.h"
#include <QtGui>
#include <QMessageBox>
#include <QApplication>
#include <QPushButton>

void MessageDialog::error(const QString& text, const QString& title)
{
	QMessageBox::critical(QApplication::activeWindow(), title, text);
}

void MessageDialog::warning(const QString& text, const QString& title)
{
	QMessageBox::warning(QApplication::activeWindow(), title, text);
}

void MessageDialog::information(const QString& text, const QString& title)
{
	QMessageBox::information(QApplication::activeWindow(), title, text);
}

bool MessageDialog::yesNo(const QString& text, const QString& title, const QString& yes,
									 const QString& no)
{
	QMessageBox mb(QApplication::activeWindow());
	mb.setWindowTitle(title);
	mb.setText(text);
	mb.setIcon(QMessageBox::Question);
	QPushButton* y = mb.addButton(yes, QMessageBox::YesRole);
	mb.setDefaultButton(y);
	mb.addButton(no, QMessageBox::NoRole);
	mb.exec();
	return mb.clickedButton() == y;
}

bool MessageDialog::okCancel(const QString& text, const QString& title, const QString& ok,
								  const QString& cancel)
{
	QMessageBox mb(QApplication::activeWindow());
	mb.setWindowTitle(title);
	mb.setText(text);
	mb.setIcon(QMessageBox::Question);
	QPushButton* o = mb.addButton(ok, QMessageBox::ActionRole);
	mb.setDefaultButton(o);
	QPushButton* c = mb.addButton(cancel, QMessageBox::RejectRole);
	mb.setEscapeButton(c);
	mb.exec();
	return mb.clickedButton() == o;
}

int MessageDialog::yesNoCancel(const QString& text, const QString& title, const QString& yes,
								  const QString& no, const QString& cancel)
{
	QMessageBox mb(QApplication::activeWindow());
	mb.setWindowTitle(title);
	mb.setText(text);
	mb.setIcon(QMessageBox::Question);
	QPushButton* y = mb.addButton(yes, QMessageBox::ActionRole);
	mb.setDefaultButton(y);
	QPushButton* n = mb.addButton(no, QMessageBox::NoRole);
	QPushButton* c = mb.addButton(cancel, QMessageBox::RejectRole);
	mb.setEscapeButton(c);
	mb.exec();
	if (mb.clickedButton() == y)
		return Yes;
	else if (mb.clickedButton() == n)
		return No;
	else return Cancel;
}


