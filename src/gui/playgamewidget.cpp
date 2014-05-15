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

// no multicaptures are given by PlayGameEngine
// each capture of a sequence is it's own move (in terms of move-time)
// currently a workaround
#define SB_SINGLE_CAPTURE

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
    winStm_         (-1),
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

    // XXX not needed anymore
    ui_->b_settings1->setVisible(false);
    ui_->b_settings2->setVisible(false);

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
    connect(ui_->b_config, SIGNAL(clicked()), SIGNAL(configureGameRequest()));

    // ----- setup PlayGame -----

    connect(play_, SIGNAL(moveMade1(Move)), SLOT(moveFromEngine(Move)));
    connect(play_, SIGNAL(moveMade2(Move)), SLOT(moveFromEngine(Move)));
    connect(play_, SIGNAL(moveInfo1(Move,int)), SLOT(infoFromEngine(Move,int)));
    connect(play_, SIGNAL(moveInfo2(Move,int)), SLOT(infoFromEngine(Move,int)));
    connect(play_, SIGNAL(ready()), SLOT(enginesReady()));
    connect(play_, SIGNAL(engineClueless()), SLOT(engineClueless()));
    connect(play_, SIGNAL(engineError(QString)), SLOT(engineError(const QString&)));

    setWidgetsPlaying_(false);

    slotReconfigure();
}

PlayGameWidget::~PlayGameWidget()
{
    delete ui_;
}

bool PlayGameWidget::isHuman(Color stm) const
{
    return (stm == White && !play_->player1IsEngine())
            ||
           (stm == Black && !play_->player2IsEngine());
}

bool PlayGameWidget::whiteCanMove() const
{
    return !play_->player1IsEngine();
}

bool PlayGameWidget::blackCanMove() const
{
    return !play_->player2IsEngine();
}

bool PlayGameWidget::isTournament() const
{
    return tc_.type() == TimeControl::T_Tournament;
}

bool PlayGameWidget::isHumanInvolved() const
{
    return !(play_->player1IsEngine() && play_->player2IsEngine());
}

bool PlayGameWidget::doInfoLines() const
{
    return !(isTournament() && isHumanInvolved())
            && !(!play_->player1IsEngine() && !play_->player2IsEngine())
            ;
}

bool PlayGameWidget::doAutoSaveAndContinue() const
{
    return (!isHumanInvolved() && ui_->cbSaveAndContinue->isChecked());
}

QString PlayGameWidget::resultString() const
{
    if (winStm_ < 0)
        return "";
    if (winStm_ == White)
        return "1-0";
    if (winStm_ == Black)
        return "0-1";
    return "1/2-1/2";
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

    // set player 2 to the first engine when
    // Engines have been auto-added to list
    if (enginesList.wasEmpty() && !names.empty())
        play_->setEngineName2(names[0]);

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
    // also calls updateEngineWidgets_();
    setWidgetsPlaying_(playing_);
}

void PlayGameWidget::slotEngine2Changed_(const QString& s)
{
    play_->setEngineName2(s);
    setWidgetsPlaying_(playing_);
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
    // predefine the known game tags
    QMap<QString, QString> tags;
    tags.insert(TagNameWhite, play_->playerName1());
    tags.insert(TagNameBlack, play_->playerName2());
    tags.insert(TagNameDate, QDate::currentDate().toString(Qt::ISODate).replace('-',"."));
    tags.insert(TagNameTimeControl, tc_.humanReadable());

    emit startNewGameRequest(tags);
}

void PlayGameWidget::startNewGame()
{
    start_();
}

void PlayGameWidget::startNewGameOk()
{
    if (isPlaying())
        stop();

    // update widget Spaß
    setWidgetsPlayer_(White);
    setWidgetsPlaying_(true);

    winStm_ = -1;
    curStm_ = White;
    playing_ = true;
    userMoved_ = false;
    ignoreAnswer_ = false;
    score_[0] = score_[1] = 0;
    lastMoveNumber_ = 0;
    hashHistory_.clear();
    plyQue_.clear();

    // first player is engine? - then go
    sendBoardWhenReady_ = play_->player1IsEngine();
    sendBoard_.setStandardPosition();

    // run engines
    play_->activate();

    // start counting player 1
    initTiming_();
    if (isHuman(White))
        tc_.startMove();
}

void PlayGameWidget::continue_()
{
    emit continueGameRequest();
}

void PlayGameWidget::continuePosition(const Board &board)
{
    SB_PLAY_DEBUG("PlayGameWidget::continuePosition(..)");

    winStm_ = -1;
    // curStm_ is actually last stm,
    // will be flipped at end of func.
    curStm_ = board.toMove();
    playing_ = true;
    userMoved_ = false;
    ignoreAnswer_ = false;
    score_[0] = score_[1] = 0;
    hashHistory_.clear();
    lastMoveNumber_ = 0;
    plyQue_.clear();

    // update widget Spaß
    setWidgetsPlaying_(true);
    setWidgetsPlayer_(curStm_);

    sendBoardWhenReady_ = !isHuman(curStm_);
    sendBoard_ = board;

    play_->activate();

    initTiming_(curStm_);

    if (isHuman(curStm_))
        tc_.startMove();

    curStm_ = (Color)(!curStm_);

}

void PlayGameWidget::stop()
{
    if (tc_.isMoving())
        tc_.endMove();
    setWidgetsPlaying_(playing_ = false);
    play_->deactivate();
    plyQue_.clear();
}

void PlayGameWidget::stopThinking()
{
    tc_.endMove();
    ignoreAnswer_ = true;
    blinkTimer_.stop();
    play_->deactivate();
    plyQue_.clear();
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
        winStm_ = play_->player2IsEngine()? 1 : 0;
        gameComment(winStm_==White? tr("Black resigned") : tr("White resigned"));
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
        tourn = isTournament(),
        timeplay = tourn || tc_.type() == TimeControl::T_Match,
        doinf = doInfoLines();

    ui_->b_new->setEnabled(!p);
    ui_->b_continue->setEnabled(!p && (!timeplay || !isHumanInvolved()));
    ui_->b_pause->setEnabled(p && (!timeplay || !isHumanInvolved()));
    ui_->b_flip->setEnabled(!p);
    ui_->b_resign->setEnabled(p && isHumanInvolved());
    ui_->b_config->setEnabled(!p);
    ui_->nameEdit1->setEnabled(!p);
    ui_->nameEdit2->setEnabled(!p);
    ui_->engineCombo1->setEnabled(!p);
    ui_->engineCombo2->setEnabled(!p);
    ui_->labelInfo1->setVisible(doinf);
    ui_->labelInfo2->setVisible(doinf);
    ui_->labelInfo1->setText("");
    ui_->labelInfo2->setText("");
    ui_->cbSaveAndContinue->setVisible(!isHumanInvolved());

    if (!p)
    {
        blinkTimer_.stop();
        ui_->led1->setValue(false);
        ui_->led2->setValue(false);
    }

    // XXX bit hacky here
    updateEngineWidgets_();
}

void PlayGameWidget::setMoveComment_(int mt, int score)
{
    QString s;
    if (AppSettings->getValue("/PlayGame/saveMoveTime").toBool())
    {
        s += tc_.msecToString(mt);
    }
    if (AppSettings->getValue("/PlayGame/saveScore").toBool()
            && score)
    {
        if (!s.isEmpty())
            s += " ";
        if (score>0)
            s += "+";
        s += QString::number((qreal)score / 100);
    }

    if (!s.isEmpty())
        emit gameComment(s);
}

void PlayGameWidget::enginesReady()
{
    SB_PLAY_DEBUG("PlayGameWidget::enginesReady()");

    if (!playing_)
        return;

    if (sendBoardWhenReady_)
    {
        sendBoardWhenReady_ = false;

        //setPosition(b);
        play_->setPosition(sendBoard_,settings_(sendBoard_.toMove()));
        tc_.startMove();
    }
}

void PlayGameWidget::engineClueless()
{
    if (!playing_)
        return;

    if (tc_.isMoving())
        tc_.endMove();


    // XXX what to do here?
    if (isHumanInvolved())
        QMessageBox::warning(
                 this,
                 tr("Shatra Engine"),
                 tr("Sorry, but the Engine did not respond\n"
                    "in the specified time... You win!"));

    setWidgetsPlaying_(playing_ = false);
    stop();
    emit gameComment(tr("Engine didn't respond"));
    emit gameEnded();
}

void PlayGameWidget::engineError(const QString & str)
{
    emit gameComment("*ERROR* " + str);
}

void PlayGameWidget::setPosition(const Board& board)
{
    SB_PLAY_DEBUG("PlayGameWidget::setPosition() stm="<<board.toMove()<<"plyQue_.size()=" << plyQue_.size());

    //qDebug() << "PlayGameWidget::setPosition() stm="<<board.toMove()<<"plyQue_.size()=" << plyQue_.size();

    if (!playing_) return;

    // XXX reset engine que
    plyQue_.clear();

    // player's move ended (or at least one ply)
    tc_.stopMove();

    // when move ended, !stm == side that made last move
    curStm_ = board.transitAt() == 0?
                (Color)(!board.toMove())
                       : board.toMove();

    userMoved_ = isHuman((Color)curStm_);
}

void PlayGameWidget::infoFromEngine(Move m, int s)
{
    QLabel * l = ui_->labelInfo1;
    if (m.sideMoving() == 1)
        l = ui_->labelInfo2;

    //if (m.sideMoving() == 1)
    //    s = -s;

    score_[m.sideMoving()] = s;

    bool isNum = AppSettings->getValue("/General/Notation").toBool() == NUM;

    l->setText(QString("<html><b>%1</b> (<font color=\"#%4\">%2%3</font>)</html>")
               .arg(isNum? m.toNumeric() : m.toAlgebraic())
               .arg(s>0? "+" : "")
               .arg((qreal)s/100)
               .arg(s>0? "080" : s<0? "800" : "000"));
}

void PlayGameWidget::moveFromEngine(Move m)
{
    SB_PLAY_DEBUG("PlayGameWidget::moveFromEngine() plyQue_.size()=" << plyQue_.size());
    //qDebug() << "PlayGameWidget::moveFromEngine() plyQue_.size()=" << plyQue_.size();

#ifndef SB_SINGLE_CAPTURE
    // stop counter on first engine move
    // (potential multi-capture is already generated so don't count further)
    if (plyQue_.empty())
    {
        tc_.endMove();
    }
#else
    if (tc_.isMoving())
    {
        if (m.willContinue())
            tc_.stopMove();
        else
            tc_.endMove();
    }
#endif

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
//        qDebug() << "ENGINE MOVED";
        curStm_ = (Color)m.sideMoving();
        emit moveMade(m);

        // clear 'what-was-i-thinking-field'
        if (curStm_)
            ui_->labelInfo2->clear();
        else
            ui_->labelInfo1->clear();

        return;
    }
}

void PlayGameWidget::animationFinished(const Board& board)
{
    SB_PLAY_DEBUG("PlayGameWidget::animationFinished() stm="<<board.toMove()
                  << " plyQue_.size()=" << plyQue_.size());

    if (!playing_)
        return;

    if (board.moveNumber() > lastMoveNumber_)
    {
        hashHistory_.push_back(board.getHashValue());
        if (Board::isRepeating(hashHistory_))
        {
            // XXX may work :|
            winStm_ = Draw;
            emit gameComment("Draw by repetition");
            stop();
            // XXX need a signal/slot for player notice
            emit gameEnded();
            return;
        }
        lastMoveNumber_ = board.moveNumber();
    }

    // don't restart an ended game
    //if (checkGameResult_(board, false, false))
    //    return;

    // get last moving side on board
    const bool transit = board.transitAt() != 0;
    const Color stm = transit? board.toMove() : (Color)(!board.toMove());

    // user move
    if (isHuman(stm))
    {
        // make sure no duplicate animation has occured
        if (!userMoved_) return;
        userMoved_ = false;

        // this is a multiply move?
        if (transit != 0)
        {
            tc_.continueMove();
            return;
        }

        // only switches sides, tc_.stopMove() was called in setPosition()
        int mt = tc_.endMove();
        setMoveComment_(mt, 0);

        // switch sides if game not ended
        if (!checkGameResult_(board, true, true))
            startNewMove_(board);

        return;
    }

    // engine moved last
    else if (!plyQue_.empty())
    {
        // we sent that one before
        plyQue_.pop_front();

        // more plies in the que?
        if (!plyQue_.empty())
        {
            emit moveMade(plyQue_.first());
            return;
        }

#ifdef SB_SINGLE_CAPTURE
        if (!transit)
#endif
            setMoveComment_( tc_.getMoveTime(curStm_), score_[curStm_] );

        // check if last engine move ended game
        if (!checkGameResult_(board, true, true))
        {
#ifdef SB_SINGLE_CAPTURE
            if (transit)
            {
                // get next move from engine
                tc_.continueMove();
                play_->setPosition(board, settings_(stm));
                blinkTimer_.start();
                return;
            }
#endif
            // switch to other player and start move counter
            // tc_.endMove() was called before
            startNewMove_(board);
        }
    }
}

void PlayGameWidget::startNewMove_(const Board& board)
{
    SB_PLAY_DEBUG("PlayGameWidget::startNewMove_(..)");

    const Color stm = board.toMove();

    // update widets
    setWidgetsPlayer_(stm);

    // start engine
    if (!isHuman(stm))
    {
        blinkTimer_.start();
        play_->setPosition(board, settings_(stm));
    }

    // start timing
    tc_.startMove();
}


bool PlayGameWidget::checkGameResult_(const Board & board, bool trigger, bool dostop)
{
    bool end = false;

    const bool
        wwin = board.gameResult() == WhiteWin,
        bwin = board.gameResult() == BlackWin,
        draw = false, //XXX need to fix this// board.gameResult() == Draw,
        e1 = play_->player1IsEngine(),
        e2 = play_->player2IsEngine();

    // stop playing
    if ( wwin || bwin || draw || board.hasNoMoves() )
    {
        if (dostop)
            stop();
        end = true;

        // determine winning side
        winStm_ = draw? 2 : curStm_;//board.toMove();
    }

    if (trigger)
    {
        // emit appropriate signals
        if ((wwin && !e1) || (bwin && !e2))
        {
            if (isHumanInvolved())
                emit playerWins();
            else
                emit gameEnded();
        }
        else
        if ((wwin && e1) || (bwin && e2))
        {
            if (isHumanInvolved())
                emit playerLoses();
            else
                emit gameEnded();
        }
    }

    return end;
}

Engine::SearchSettings PlayGameWidget::settings_(int stm) const
{
    Engine::SearchSettings s;

    if (tc_.type() == TimeControl::T_Limit)
    {
        s.maxDepth = tc_.depthLimit();
        s.maxTime = tc_.timeLimit();
        s.maxNodes = tc_.nodeLimit();
        return s;
    }

    if (tc_.type() == TimeControl::T_Tournament)
    {
        s.wtime = tc_.getTotalTime(White);
        s.btime = tc_.getTotalTime(Black);
        s.movestogo = tc_.movesToGo();
        return s;
    }

    if (tc_.type() == TimeControl::T_Match)
    {
        s.maxTime = tc_.getMoveTime(!stm);
        return s;
    }

    return s;
}


void PlayGameWidget::slotBlinkTimer_()
{
    QLed * led = (activeLed_ == 0)? ui_->led1 : ui_->led2;

    led->setOnColor(led->onColor() == colorEngine0_ ?
                          colorEngine1_ : colorEngine0_);
}

void PlayGameWidget::slotTimeout_(int stm)
{
    emit gameComment(stm == 0? tr("White lost in time") : tr("Black lost in time"));

    winStm_ = !stm;

    stop();

    // engine vs. engine
    if (!isHumanInvolved())

        emit gameEnded();

    // player vs engine
    else
    {
        if (isHuman((Color)stm))
            emit playerLoses();
        else
            emit playerWins();
    }
}

void PlayGameWidget::slotUpdateClocks_()
{
    ui_->clock1->setTime(tc_.getTotalTime(White), tc_.getMoveTime(White));
    ui_->clock2->setTime(tc_.getTotalTime(Black), tc_.getMoveTime(Black));
}

void PlayGameWidget::initTiming_(int stm)
{
    // clock visibility
    bool
        showTotal = tc_.totalTimeAtStart() != TimeControl::Unlimited,
        showMove = tc_.type() == TimeControl::T_Match
                || tc_.type() == TimeControl::T_Tournament;

    ui_->clock1->setVisible(showTotal, showMove);
    ui_->clock2->setVisible(showTotal, showMove);

    tc_.start(stm);
}

