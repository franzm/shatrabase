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

#include "playgamewidget.h"
#include "ui_playgame.h"
#include "settings.h"
#include "enginelist.h"
#include "playgame.h"

#include <QMessageBox>

PlayGameWidget::PlayGameWidget(QWidget *parent) :
    QWidget     (parent),
    ui_         (new Ui::PlayGame),
    play_       (new PlayGame(this)),
    playing_    (false)
{
    setObjectName("PlayGameWidget");
    setWindowTitle(tr("Player selection"));

    // ----- setup PlayGame -----

    connect(play_, SIGNAL(moveMade1(Move)), SLOT(moveFromEngine1(Move)));
    connect(play_, SIGNAL(moveMade2(Move)), SLOT(moveFromEngine2(Move)));
    connect(play_, SIGNAL(ready()), SLOT(enginesReady()));
    connect(play_, SIGNAL(engineClueless()), SLOT(engineClueless()));

    // ------- setup ui ------

    ui_->setupUi(this);

    ui_->b_resign->setEnabled(false);

    connect(ui_->nameEdit1, SIGNAL(textEdited(QString)), SLOT(slotName1Changed_(QString)));
    connect(ui_->nameEdit2, SIGNAL(textEdited(QString)), SLOT(slotName2Changed_(QString)));

    connect(ui_->engineCombo1, SIGNAL(activated(QString)), SLOT(slotEngine1Changed_(QString)));
    connect(ui_->engineCombo2, SIGNAL(activated(QString)), SLOT(slotEngine2Changed_(QString)));

    connect(ui_->b_new, SIGNAL(clicked()), SLOT(start_()));
    connect(ui_->b_flip, SIGNAL(clicked()), SLOT(flipPlayers_()));
    connect(ui_->b_resign, SIGNAL(clicked()), SLOT(resign_()));

    slotReconfigure();
}

PlayGameWidget::~PlayGameWidget()
{
    delete ui_;
}

bool PlayGameWidget::whiteCanMove() const
{
    return !play_->player1IsEngine();
}

bool PlayGameWidget::blackCanMove() const
{
    return !play_->player2IsEngine();
}

void PlayGameWidget::slotReconfigure()
{
    // all settings are stored in PlayGame
    play_->slotReconfigure();

    ui_->nameEdit1->setText(play_->playerName1());
    ui_->nameEdit2->setText(play_->playerName2());

    // get the engine list to populate combobox
    EngineList enginesList;
    enginesList.restore();
    QStringList names = enginesList.names();
    names.insert(0, tr("human"));

    // setup combo boxes
    ui_->engineCombo1->clear();
    ui_->engineCombo1->insertItems(0, names);
    int index = names.indexOf(play_->engineName1());
    if (index != -1)
        ui_->engineCombo1->setCurrentIndex(index);
    else
        ui_->engineCombo1->setCurrentIndex(0);

    ui_->engineCombo2->clear();
    ui_->engineCombo2->insertItems(0, names);
    index = names.indexOf(play_->engineName2());
    if (index != -1)
        ui_->engineCombo2->setCurrentIndex(index);
    else
        ui_->engineCombo2->setCurrentIndex(0);

    // update human/engine led colors
    ui_->led1->setOnColor(play_->player1IsEngine()? QLed::Red : QLed::Green);
    ui_->led2->setOnColor(play_->player2IsEngine()? QLed::Red : QLed::Green);
}


void PlayGameWidget::slotName1Changed_(const QString& s)
{
    play_->setPlayerName1(s);
}

void PlayGameWidget::slotName2Changed_(const QString& s)
{
    play_->setPlayerName2(s);
}

void PlayGameWidget::slotEngine1Changed_(const QString& s)
{
    play_->setEngineName1(s);
    ui_->led1->setOnColor(play_->player1IsEngine()? QLed::Red : QLed::Green);
}

void PlayGameWidget::slotEngine2Changed_(const QString& s)
{
    play_->setEngineName2(s);
    ui_->led2->setOnColor(play_->player2IsEngine()? QLed::Red : QLed::Green);
}

void PlayGameWidget::start_()
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

    playing_ = true;

    // first player is engine? - then go
    sendFreshBoardWhenReady_ = play_->player1IsEngine();

    play_->activate();

    emit startNewGame();
}

void PlayGameWidget::resign_()
{
    playing_ = false;

    ui_->b_new->setEnabled(true);
    ui_->b_resign->setEnabled(false);
    ui_->b_flip->setEnabled(true);
    ui_->nameEdit1->setEnabled(true);
    ui_->nameEdit2->setEnabled(true);
    ui_->engineCombo1->setEnabled(true);
    ui_->engineCombo2->setEnabled(true);

    emit resignGame();
}

void PlayGameWidget::flipPlayers_()
{
    play_->setPlayerName1(ui_->nameEdit2->text());
    play_->setPlayerName2(ui_->nameEdit1->text());
    play_->setEngineName1(ui_->engineCombo2->currentText());
    play_->setEngineName2(ui_->engineCombo1->currentText());

    slotReconfigure();
}

void PlayGameWidget::setPosition(const Board& board)
{
    SB_PLAY_DEBUG("PlayGameWidget::setPosition()");

    if (!playing_) return;

    // White is next
    if (board.toMove() == White)
    {
        setPlayer_(White);

        if (play_->player1IsEngine())
        {
            play_->setPosition(board);
        }
    }
    // Black is next
    else
    {
        setPlayer_(Black);

        if (play_->player2IsEngine())
        {
            play_->setPosition(board);
        }
    }
}

void PlayGameWidget::enginesReady()
{
    SB_PLAY_DEBUG("PlayGameWidget::enginesReady()");

    if (sendFreshBoardWhenReady_)
    {
        sendFreshBoardWhenReady_ = false;

        Board b;
        b.setStandardPosition();
        setPosition(b);
    }
}

void PlayGameWidget::engineClueless()
{
    QMessageBox::warning(
             this,
             tr("Shatra Engine"),
             tr("Sorry, but the Engine did not respond\n"
                "in the specified time... You win!"));
}

void PlayGameWidget::moveFromEngine1(Move m)
{
    setPlayer_(m.sideMoving());
    emit moveMade(m);
}

void PlayGameWidget::moveFromEngine2(Move m)
{
    setPlayer_(m.sideMoving());
    emit moveMade(m);
}

void PlayGameWidget::setPlayer_(int stm)
{
    ui_->led1->setValue(stm == White);
    ui_->led2->setValue(stm != White);
}
