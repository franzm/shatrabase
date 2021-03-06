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

#include "copydialog.h"


CopyDialog::CopyDialog(QWidget * parent) : QDialog(parent)
{
	ui.setupUi(this);
	connect(ui.databaseList, SIGNAL(itemDoubleClicked(QListWidgetItem *)), SLOT(accept()));

    // XXX quickly removed
    ui.filterButton->setVisible(false);
}

CopyDialog::~CopyDialog()
{
}

void CopyDialog::setSelectionVisible(int num)
{
    ui.selectedButton->setVisible(num>0);
    ui.selectedButton->setText(ui.selectedButton->text()
                               + " (" + QString::number(num) + ")");
}

void CopyDialog::setDatabases(const QStringList& databases)
{
	ui.databaseList->clear();
	ui.databaseList->addItems(databases);
	ui.databaseList->setCurrentRow(0);
}

int CopyDialog::getDatabase() const
{
	return ui.databaseList->currentRow();
}

void CopyDialog::setMode(SrcMode mode)
{
    switch (mode)
    {
    case SingleGame:
        ui.singleButton->setChecked(true);
        break;
    case Filter:
        ui.filterButton->setChecked(true);
        break;
    case AllGames:
        ui.allButton->setChecked(true);
        break;
    case SelectedGames:
        ui.selectedButton->setChecked(true);
    }
}

int CopyDialog::getMode() const
{
	if (ui.singleButton->isChecked())
		return SingleGame;
	else if (ui.filterButton->isChecked())
		return Filter;
    else if (ui.selectedButton->isChecked())
        return SelectedGames;
    else return AllGames;
}

