/***************************************************************************
 *                           Shatrabase                                    *
 *   (C) 2012-2014 AFKM <francis@afkm.com>                                 *
 *                   derived from ChessX with thanks                       *
 *   (C) 2007 Michal Rudolf <mrudolf@kdewebdev.org>                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef __COPYDIALOG_H__
#define __COPYDIALOG_H__

#include "ui_copydialog.h"

/*
  The CopyDialog class represents a dialog for choosing a target database
for copied games.  */

class CopyDialog : public QDialog
{
	Q_OBJECT
	public:
        enum SrcMode { SingleGame, Filter, SelectedGames, AllGames };
		/* Create Preferences dialog */
		CopyDialog(QWidget* parent = 0);
		/* Destroy dialog */
		virtual ~CopyDialog();
        /** Turns on or off the 'selected games' radiobutton
            and sets the text. */
        void setSelectionVisible(int num);
		/* Set current board */
		void setDatabases(const QStringList& databases);
		/* Get target database */
		int getDatabase() const;
		/* Get selected mode */
		int getMode() const;
        /* Set the preferred mode */
        void setMode(SrcMode mode);
	private:
		Ui::CopyDialog ui;
};


#endif
