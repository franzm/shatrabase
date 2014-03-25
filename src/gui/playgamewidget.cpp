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

    connect(play_, SIGNAL(moveMade1(Move)), SLOT(moveFromEngine(Move)));
    connect(play_, SIGNAL(moveMade2(Move)), SLOT(moveFromEngine(Move)));
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
    ui_->led1->setOnColor(play_->player1IsEngine()? QLed::Blue : QLed::Green);
    ui_->led2->setOnColor(play_->player2IsEngine()? QLed::Blue : QLed::Green);
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
    ui_->led1->setOnColor(play_->player1IsEngine()? QLed::Blue : QLed::Green);
}

void PlayGameWidget::slotEngine2Changed_(const QString& s)
{
    play_->setEngineName2(s);
    ui_->led2->setOnColor(play_->player2IsEngine()? QLed::Blue : QLed::Green);
}

void PlayGameWidget::start_()
{
    // update widget Spaß
    setWidgetsPlayer_(White);
    setWidgetsPlaying_(true);

    playing_ = true;

    // first player is engine? - then go
    sendFreshBoardWhenReady_ = play_->player1IsEngine();

    play_->activate();

    emit startNewGame();
}

void PlayGameWidget::resign_()
{
    playing_ = false;

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

void PlayGameWidget::setWidgetsPlayer_(int stm)
{
    ui_->led1->setValue(stm == White);
    ui_->led2->setValue(stm != White);
}

void PlayGameWidget::setWidgetsPlaying_(bool p)
{
    ui_->b_new->setEnabled(!p);
    ui_->b_resign->setEnabled(p);
    ui_->b_flip->setEnabled(!p);
    ui_->nameEdit1->setEnabled(!p);
    ui_->nameEdit2->setEnabled(!p);
    ui_->engineCombo1->setEnabled(!p);
    ui_->engineCombo2->setEnabled(!p);
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
    // XXX what to do here?
    QMessageBox::warning(
             this,
             tr("Shatra Engine"),
             tr("Sorry, but the Engine did not respond\n"
                "in the specified time... You win!"));

    setWidgetsPlaying_(playing_ = false);
}

void PlayGameWidget::setPosition(const Board& board)
{
    SB_PLAY_DEBUG("PlayGameWidget::setPosition() plyQue_.size()=" << plyQue_.size());

    if (!playing_) return;

    // White is next
    if (board.toMove() == White)
    {
        setWidgetsPlayer_(White);

        if (play_->player1IsEngine())
        {
            play_->setPosition(board);
        }
    }
    // Black is next
    else
    {
        setWidgetsPlayer_(Black);

        if (play_->player2IsEngine())
        {
            play_->setPosition(board);
        }
    }
}


void PlayGameWidget::moveFromEngine(Move m)
{
    SB_PLAY_DEBUG("PlayGameWidget::moveFromEngine() plyQue_.size()=" << plyQue_.size());

    plyQue_.append(m);

    // first ply can be send right away
    if (plyQue_.size() == 1)
    {
        lastMoveSend_ = m;
        emit moveMade(m);
        return;
    }
}

void PlayGameWidget::animationFinished()
{
    SB_PLAY_DEBUG("PlayGameWidget::animationFinished() plyQue_.size()=" << plyQue_.size());

    // more plies in the que?
    if (plyQue_.size() >= 1)
    {
        plyQue_.pop_front();
        if (!plyQue_.empty())
        {
            lastMoveSend_ = plyQue_.first();
            emit moveMade(lastMoveSend_);
            return;
        }
    }

    setWidgetsPlayer_(oppositeColor((Color)lastMoveSend_.sideMoving()));
}
