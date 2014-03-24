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

#include "playerselect.h"
#include "ui_playerselect.h"
#include "settings.h"
#include "enginelist.h"
#include "playersetup.h"
#include "board.h"

#include <QDebug>

PlayerSelect::PlayerSelect(QWidget *parent) :
    QWidget     (parent),
    ui_         (new Ui::PlayerSelect),
    player_     (new PlayerSetup(this))
{
    setObjectName("PlayerSelectWidget");
    setWindowTitle(tr("Player selection"));

    ui_->setupUi(this);

    ui_->b_resign->setEnabled(false);

    connect(ui_->nameEdit1, SIGNAL(textEdited(QString)), SLOT(slotName1Changed_(QString)));
    connect(ui_->nameEdit2, SIGNAL(textEdited(QString)), SLOT(slotName2Changed_(QString)));

    connect(ui_->engineCombo1, SIGNAL(activated(QString)), SLOT(slotEngine1Changed_(QString)));
    connect(ui_->engineCombo2, SIGNAL(activated(QString)), SLOT(slotEngine2Changed_(QString)));

    connect(ui_->b_new, SIGNAL(clicked()), SLOT(start_()));
    connect(ui_->b_flip, SIGNAL(clicked()), SLOT(flipPlayers_()));
    connect(ui_->b_resign, SIGNAL(clicked()), SLOT(resign_()));
}

PlayerSelect::~PlayerSelect()
{
    delete ui_;
}


void PlayerSelect::slotReconfigure()
{
    // all settings are stored in PlayerSetup
    player_->slotReconfigure();

    ui_->nameEdit1->setText(player_->playerName1());
    ui_->nameEdit2->setText(player_->playerName2());

    // get the engine list to populate combobox
    EngineList enginesList;
    enginesList.restore();
    QStringList names = enginesList.names();
    names.insert(0, tr("human"));

    // setup combo boxes
    ui_->engineCombo1->clear();
    ui_->engineCombo1->insertItems(0, names);
    int index = names.indexOf(player_->engineName1());
    if (index != -1)
        ui_->engineCombo1->setCurrentIndex(index);
    else
        ui_->engineCombo1->setCurrentIndex(0);

    ui_->engineCombo2->clear();
    ui_->engineCombo2->insertItems(0, names);
    index = names.indexOf(player_->engineName2());
    if (index != -1)
        ui_->engineCombo2->setCurrentIndex(index);
    else
        ui_->engineCombo2->setCurrentIndex(0);

    // update human-player led colors
    if (!player_->player1IsEngine())
        ui_->led1->setOnColor(QLed::Green);
    if (!player_->player2IsEngine())
        ui_->led2->setOnColor(QLed::Green);
}


void PlayerSelect::slotName1Changed_(const QString& s)
{
    player_->setPlayerName1(s);
}

void PlayerSelect::slotName2Changed_(const QString& s)
{
    player_->setPlayerName2(s);
}

void PlayerSelect::slotEngine1Changed_(const QString& s)
{
    player_->setEngineName1(s);
}

void PlayerSelect::slotEngine2Changed_(const QString& s)
{
    qDebug() << s;
    player_->setEngineName2(s);
}

void PlayerSelect::start_()
{
    // update widget Spaß
    ui_->led1->setValue(true);
    ui_->led2->setValue(false);
    ui_->b_new->setEnabled(false);
    ui_->b_resign->setEnabled(true);
    ui_->b_flip->setEnabled(false);
    ui_->nameEdit1->setEnabled(false);
    ui_->nameEdit2->setEnabled(false);
    ui_->engineCombo1->setEnabled(false);
    ui_->engineCombo2->setEnabled(false);
}

void PlayerSelect::resign_()
{
    ui_->b_new->setEnabled(true);
    ui_->b_resign->setEnabled(false);
    ui_->b_flip->setEnabled(true);
    ui_->nameEdit1->setEnabled(true);
    ui_->nameEdit2->setEnabled(true);
    ui_->engineCombo1->setEnabled(true);
    ui_->engineCombo2->setEnabled(true);
}

void PlayerSelect::flipPlayers_()
{
    player_->setPlayerName1(ui_->nameEdit2->text());
    player_->setPlayerName2(ui_->nameEdit1->text());
    player_->setEngineName1(ui_->engineCombo2->currentText());
    player_->setEngineName2(ui_->engineCombo1->currentText());

    slotReconfigure();
}

void PlayerSelect::setPosition(const Board& board)
{
    if (board.toMove() == White)
    {
        ui_->led1->setValue(false);
        ui_->led2->setValue(true);
    }
    else
    {
        ui_->led1->setValue(true);
        ui_->led2->setValue(false);
    }
}

