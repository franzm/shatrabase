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

#include <QMessageBox>
#include <QDate>

#include "playgamewidget.h"
#include "ui_playgame.h"
#include "settings.h"
#include "enginelist.h"
#include "playgame.h"
#include "playgameenginedialog.h"

PlayGameWidget::PlayGameWidget(EngineDebugWidget * debug, QWidget *parent) :
    QWidget         (parent),
    colorPlayer_    (QLed::Green),
    colorEngine0_   (QLed::Green),
    colorEngine1_   (QLed::Blue),
    blinkInterval_  (500),
    ui_             (new Ui::PlayGame),
    engineDebug_    (debug),
    activeLed_      (0),
    play_           (new PlayGame(engineDebug_, this)),
    playing_        (false),
    ignoreAnswer_   (false)

{
    setObjectName("PlayGameWidget");
    setWindowTitle(tr("Player selection"));

    // blink timer
    blinkTimer_.setSingleShot(false);
    blinkTimer_.setInterval(blinkInterval_);
    connect(&blinkTimer_, SIGNAL(timeout()), SLOT(slotBlinkTimer_()));

    // Time Control (TM)
    connect(&tc_, SIGNAL(timeUpdated()), SLOT(slotUpdateClocks_()));
    connect(&tc_, SIGNAL(timeOut(int)), SLOT(slotTimeout_(int)));

    // ------- setup ui ------

    ui_->setupUi(this);

    ui_->clock2->setColor(false);

    connect(ui_->nameEdit1, SIGNAL(textEdited(QString)), SLOT(slotName1Changed_(QString)));
    connect(ui_->nameEdit2, SIGNAL(textEdited(QString)), SLOT(slotName2Changed_(QString)));

    connect(ui_->engineCombo1, SIGNAL(activated(QString)), SLOT(slotEngine1Changed_(QString)));
    connect(ui_->engineCombo2, SIGNAL(activated(QString)), SLOT(slotEngine2Changed_(QString)));

    connect(ui_->b_settings1, SIGNAL(clicked()), SLOT(slotConfig1Clicked_()));
    connect(ui_->b_settings2, SIGNAL(clicked()), SLOT(slotConfig2Clicked_()));

    connect(ui_->b_new, SIGNAL(clicked()), SLOT(start_()));
    connect(ui_->b_continue, SIGNAL(clicked()), SLOT(continue_()));
    connect(ui_->b_pause, SIGNAL(clicked()), SLOT(pause_()));
    connect(ui_->b_resign, SIGNAL(clicked()), SLOT(resign_()));
    connect(ui_->b_flip, SIGNAL(clicked()), SLOT(flipPlayers_()));

    // ----- setup PlayGame -----

    connect(play_, SIGNAL(moveMade1(Move)), SLOT(moveFromEngine(Move)));
    connect(play_, SIGNAL(moveMade2(Move)), SLOT(moveFromEngine(Move)));
    connect(play_, SIGNAL(ready()), SLOT(enginesReady()));
    connect(play_, SIGNAL(engineClueless()), SLOT(engineClueless()));

    setWidgetsPlaying_(false);

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
    if (playing_)
        stop();

    // time control settings
    tc_.configure();

    // all player settings are stored in PlayGame
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

    initTiming_();
    setWidgetsPlaying_(playing_);
}

void PlayGameWidget::updateEngineWidgets_()
{
    bool e1 = play_->player1IsEngine(),
         e2 = play_->player2IsEngine();

    // update human/engine led colors
    ui_->led1->setOnColor(e1 ? colorEngine0_ : colorPlayer_);
    ui_->led2->setOnColor(e2 ? colorEngine0_ : colorPlayer_);

    ui_->b_settings1->setEnabled(e1 && !playing_);
    ui_->b_settings2->setEnabled(e2 && !playing_);
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
    updateEngineWidgets_();
}

void PlayGameWidget::slotEngine2Changed_(const QString& s)
{
    play_->setEngineName2(s);
    updateEngineWidgets_();
}

void PlayGameWidget::slotConfig1Clicked_()
{
}

void PlayGameWidget::slotConfig2Clicked_()
{
    PlayGameEngineDialog * d = new PlayGameEngineDialog(play_, 1, this);
    connect(d,SIGNAL(accepted()), SLOT(slotReconfigure()));

    d->exec();
}


void PlayGameWidget::start_()
{
    // update widget Spaß
    setWidgetsPlayer_(White);
    setWidgetsPlaying_(true);

    lastStm_ = White;
    playing_ = true;
    ignoreAnswer_ = false;
    //playerMultiPly_ = false;

    // first player is engine? - then go
    // XXX not really working right now
    sendFreshBoardWhenReady_ = play_->player1IsEngine();

    // predefine the known game tags
    QMap<QString, QString> tags;
    tags.insert("White", play_->playerName1());
    tags.insert("Black", play_->playerName2());
    tags.insert("Date", QDate::currentDate().toString(Qt::ISODate));

    emit startNewGame(tags);

    play_->activate();

    initTiming_();
    tc_.startMove();
}

void PlayGameWidget::continue_()
{
    // update widget Spaß
    setWidgetsPlaying_(true);

    playing_ = true;
    ignoreAnswer_ = false;

    play_->activate();
    //XXX tc_.startMove();

    emit continueGame();

    // ... wait for setPosition() from MainWindow
}

void PlayGameWidget::stop()
{
    if (tc_.isMoving())
        tc_.endMove();
    setWidgetsPlaying_(playing_ = false);
    play_->deactivate();
}

void PlayGameWidget::stopThinking()
{
    tc_.endMove();
    ignoreAnswer_ = true;
    blinkTimer_.stop();
    play_->deactivate();
}

void PlayGameWidget::pause_()
{
    tc_.endMove();
    setWidgetsPlaying_(playing_ = false);
    play_->deactivate();

    emit pauseGame();
}

void PlayGameWidget::resign_()
{
    if (QMessageBox::question(this, tr("Resigning"), tr("Are you sure you want to resign?"))
        == QMessageBox::Yes)
    {
        stop();
        emit playerLoses();
    }
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
    activeLed_ = stm;
    ui_->led1->setValue(stm == White);
    ui_->led2->setValue(stm != White);
}

void PlayGameWidget::setWidgetsPlaying_(bool p)
{    
    const bool
        tourn = tc_.type() == TimeControl::T_Tournament,
        timeplay = tourn || tc_.type() == TimeControl::T_Match;

    ui_->b_new->setEnabled(!p);
    ui_->b_continue->setEnabled(!p && !timeplay);
    ui_->b_pause->setEnabled(p && !timeplay);
    ui_->b_flip->setEnabled(/*!p*/false); // XXX Don't allow as long as first Player can't be Engine
    ui_->b_resign->setEnabled(p);
    ui_->nameEdit1->setEnabled(!p);
    ui_->nameEdit2->setEnabled(!p);
    ui_->engineCombo1->setEnabled(/*!p*/false); // XXX Don't allow as long as first Player can't be Engine
    ui_->engineCombo2->setEnabled(!p);

    if (!p)
    {
        blinkTimer_.stop();
        ui_->led1->setValue(false);
        ui_->led2->setValue(false);
    }

    // bit hacky here
    updateEngineWidgets_();
}


void PlayGameWidget::enginesReady()
{
    SB_PLAY_DEBUG("PlayGameWidget::enginesReady()");

    if (!playing_)
        return;

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
    if (!playing_)
        return;

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

    //qDebug() << "PlayGameWidget::setPosition() stm="<<board.toMove()<<"plyQue_.size()=" << plyQue_.size();

    if (!playing_) return;


    if (lastStm_ != board.toMove())
    {
        // player's move ended
        tc_.endMove();
    }

    lastStm_ = board.toMove();

    // check if last player move ended game
    // (Win/Lose will be checked after move animation ended)
    if (checkGameResult_(board, false, false))
        return;

    // White is next
    if (board.toMove() == White)
    {
        setWidgetsPlayer_(White);

        // start engine
        if (play_->player1IsEngine())
        {
            blinkTimer_.start();
            play_->setPosition(board);
            tc_.startMove();
        }

    }
    // Black is next
    else
    {
        setWidgetsPlayer_(Black);

        // start engine
        if (play_->player2IsEngine())
        {
            blinkTimer_.start();
            play_->setPosition(board);
            tc_.startMove();
        }

    }

}


void PlayGameWidget::moveFromEngine(Move m)
{
    SB_PLAY_DEBUG("PlayGameWidget::moveFromEngine() plyQue_.size()=" << plyQue_.size());

    //qDebug() << "PlayGameWidget::moveFromEngine() plyQue_.size()=" << plyQue_.size();

    if (plyQue_.empty())
    {
        tc_.endMove();
    }

    blinkTimer_.stop();

    // Stopped playing while Engine was thinking?
    if (!playing_ || ignoreAnswer_)
    {
        ignoreAnswer_ = false;
        plyQue_.clear();
        return;
    }

    plyQue_.append(m);

    // first ply of a sequence can be send right away
    if (plyQue_.size() == 1)
    {
        lastStm_ = (Color)m.sideMoving();
        emit moveMade(m);
        return;
    }
}

void PlayGameWidget::animationFinished(const Board& board)
{
    SB_PLAY_DEBUG("PlayGameWidget::animationFinished() plyQue_.size()=" << plyQue_.size());

    //qDebug() << "PlayGameWidget::animationFinished() plyQue_.size()=" << plyQue_.size();

    if (!playing_)
        return;

    // more plies in the que? (means engine move last)
    if (!plyQue_.empty())
    {
        // we sent that one before
        plyQue_.pop_front();
        // next ply?
        if (!plyQue_.empty())
        {
            emit moveMade(plyQue_.first());
            return;
        }

        tc_.endMove();

        // check if last engine move ended game
        if (!checkGameResult_(board, true, true))
        {
            // switch to other player
            setWidgetsPlayer_(oppositeColor(lastStm_));

            // start move thinking
            tc_.startMove();
        }
    }

}


bool PlayGameWidget::checkGameResult_(const Board & board, bool trigger, bool dostop)
{
    bool end = false;

    const bool
        wwin = board.gameResult() == WhiteWin,
        bwin = board.gameResult() == BlackWin,
        e1 = play_->player1IsEngine(),
        e2 = play_->player2IsEngine();

    // stop playing
    if ( wwin || bwin || board.hasNoMoves() )
    {
        if (dostop)
            stop();
        end = true;
    }

    if (trigger)
    {
        // emit appropriate signals
        if ((wwin && !e1) || (bwin && !e2))
            emit playerWins();
        else
        if ((wwin && e1) || (bwin && e2))
            emit playerLoses();
    }

    return end;
}



void PlayGameWidget::slotBlinkTimer_()
{
    QLed * led = (activeLed_ == 0)? ui_->led1 : ui_->led2;

    led->setOnColor(led->onColor() == colorEngine0_ ?
                          colorEngine1_ : colorEngine0_);
}

void PlayGameWidget::slotTimeout_(int stm)
{
    // XXX This is all crap (alles Quark)

    if (stm == 0 && !play_->player1IsEngine())
    {
        stop();
        emit playerLoses();
    }
    else
    if (stm == 1 && play_->player2IsEngine())
    {
        stop();
        emit playerWins();
    }
    else
        Q_ASSERT(!"XXX engine/engine not supported");
}

void PlayGameWidget::slotUpdateClocks_()
{
    ui_->clock1->setTime(tc_.getTotalTime(White), tc_.getMoveTime(White));
    ui_->clock2->setTime(tc_.getTotalTime(Black), tc_.getMoveTime(Black));
}

void PlayGameWidget::initTiming_()
{
    // clock visibility
    bool
        showTotal = tc_.totalTimeAtStart() != TimeControl::Unlimited,
        showMove = tc_.type() == TimeControl::T_Match
                || tc_.type() == TimeControl::T_Tournament;

    ui_->clock1->setVisible(showTotal, showMove);
    ui_->clock2->setVisible(showTotal, showMove);

    tc_.start();
}

