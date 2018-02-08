/***************************************************************************
 *                           Shatrabase                                    *
 *   (C) 2012-2014 AFKM <francis@afkm.com>                                 *
 *                   derived from ChessX with thanks                       *
 *   (C) 2009 by Michal Rudolf mrudolf@kdewebdev.org                       *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "analysiswidget.h"
#include "boardsetup.h"
#include "boardview.h"
#include "copydialog.h"
#include "chessbrowser.h"
#include "databaseinfo.h"
#include "databaselist.h"
#include "databasemodel.h"
#include "editaction.h"
#include "eventlistwidget.h"
#include "filtermodel.h"
#include "game.h"
#include "gamelist.h"
#include "mainwindow.h"
#include "messagedialog.h"
#include "memorydatabase.h"
#include "openingtree.h"
#include "output.h"
#include "sgndatabase.h"
#include "playerlistwidget.h"
#include "playgamewidget.h"
#include "positionbase.h"
#include "preferences.h"
#include "savedialog.h"
#include "settings.h"
#include "statistics.h"
#include "tablebase.h"
#include "tableview.h"
#include "histogram.h"
#include "ushiengine.h"
#include "ushienginetester.h"
#include "version.h"

#include <time.h>

#include <QtGui>
#include <QAction>
#include <QFileDialog>
#include <QInputDialog>
#include <QPixmap>
#include <QProgressBar>
#include <QStatusBar>
#include <QMessageBox>
#include <QMenu>

#include <QDebug>

void MainWindow::slotFileNew()
{
    QString file = QFileDialog::getSaveFileName(this, tr("New database"),
        AppSettings->value("/General/databasePath").toString(), 
        tr("SGN database (*.sgn)"));
    if (file.isEmpty())
		return;
	if (!file.endsWith(".sgn"))
		file += ".sgn";
	QFile sgnfile(file);
	if (!sgnfile.open(QIODevice::WriteOnly))
        MessageDialog::warning(tr("Cannot create Shatra database."), tr("New database"));
	else {
		sgnfile.close();
		openDatabase(file);
		AppSettings->setValue("/General/databasePath",
				QFileInfo(file).absolutePath());
	}
}

void MainWindow::slotFileOpen()
{
    QStringList files = QFileDialog::getOpenFileNames(this, tr("Open database"),
            AppSettings->value("/General/databasePath").toString(),
            tr("SGN databases (*.sgn)"));
    foreach (QString file, files)
    {
        if (!file.isEmpty()) {
            AppSettings->setValue("/General/databasePath", QFileInfo(file).absolutePath());
            openDatabaseUrl(file,false);
        }
    }
}

void MainWindow::slotFileOpenUtf8()
{
    QStringList files = QFileDialog::getOpenFileNames(this, tr("Open database"),
            AppSettings->value("/General/databasePath").toString(),
            tr("SGN databases (*.sgn)"));
    foreach (QString file, files)
    {
        if (!file.isEmpty()) {
            AppSettings->setValue("/General/databasePath", QFileInfo(file).absolutePath());
            openDatabaseUrl(file, true);
        }
    }
}

void MainWindow::slotFileOpenRecent()
{
	QAction *action = qobject_cast<QAction *>(sender());
	if (action)
		openDatabase(action->data().toString());
}

void MainWindow::saveDatabase()
{
    if (!database()->isReadOnly() && database()->isModified())
    {
        Output output(Output::Sgn);
        output.output(database()->filename(), *database());
    }
}

bool MainWindow::QuerySaveDatabase()
{
    if (QuerySaveGame())
    {
        if (m_currentDatabase && qobject_cast<MemoryDatabase*>(database()))
        {
            if (database()->isModified())
            {
                int result = MessageDialog::yesNoCancel(tr("The current database is modified!")
                            + '\n' + tr("Save it?"));
                if (MessageDialog::Yes == result)
                {
                     startOperation(tr("Saving %1...").arg(database()->name()));
                     Output output(Output::Sgn);
                     connect(&output, SIGNAL(progress(int)), SLOT(slotOperationProgress(int)));
                     output.output(database()->filename(), *database());
                     finishOperation(tr("%1 saved").arg(database()->name()));
                     return true;
                }
                return result != MessageDialog::Cancel;
            }
        }
        return true;
    }
    return false;
}

void MainWindow::slotFileSave()
{
	if (database()->isReadOnly())
		MessageDialog::warning(tr("<html>The database <i>%1</i> is read-only and cannot be saved.</html>")
				.arg(database()->name()));
    else
    if (m_currentDatabase && qobject_cast<MemoryDatabase*>(database()))
    {
		startOperation(tr("Saving %1...").arg(database()->name()));
		Output output(Output::Sgn);
        connect(&output, SIGNAL(progress(int)), SLOT(slotOperationProgress(int)));
		output.output(database()->filename(), *database());
		finishOperation(tr("%1 saved").arg(database()->name()));
	}
}

void MainWindow::slotFileSaveAs()
{
    if (database()->isReadOnly())
    {
        MessageDialog::warning(tr("<html>The database <i>%1</i> is read-only and cannot be saved.</html>")
                .arg(database()->name()));
        return;
    }
    if (!(m_currentDatabase && qobject_cast<MemoryDatabase*>(database())))
        return;

    // get filename
    QString fn = QFileDialog::getSaveFileName(
            this, tr("Save database as"),
                //AppSettings->value("/General/databasePath").toString() + "/",
                database()->filename(),
            tr("SGN databases (*.sgn)"));

    if (fn.isEmpty())
        return;

    // save
    startOperation(tr("Saving %1...").arg(database()->name()));
    Output output(Output::Sgn);
    connect(&output, SIGNAL(progress(int)), SLOT(slotOperationProgress(int)));
    output.output(fn, *database());
    database()->setFilename(fn);
    finishOperation(tr("%1 saved").arg(database()->name()));
}

void MainWindow::slotFileClose()
{
    if (m_currentDatabase)
    {// Don't remove Clipboard
        if (databaseInfo()->IsLoaded())
        {
            if (QuerySaveDatabase())
            {
                m_openingTree->cancel(false);
                m_databaseList->setFileClose(databaseInfo()->filePath());
                databaseInfo()->close();
                delete databaseInfo();
                m_databases.removeAt(m_currentDatabase);
                m_prevDatabase = 0;
                m_currentDatabase = 0; // Switch to clipboard is always safe
                m_databaseList->setFileCurrent(QString());
                updateMenuDatabases();
                slotDatabaseChanged();
            }
        }
    }
}

void MainWindow::slotFileCloseIndex(int n)
{
    if (m_currentDatabase == n)
    {
        slotFileClose();
    }
    else if (n) // Don't remove Clipboard
    {
        if (m_databases[n]->IsLoaded())
        {
            m_databaseList->setFileClose(m_databases[n]->filePath());
            m_databases[n]->close();
            delete m_databases[n];
            m_databases.removeAt(n);
            if (m_currentDatabase > n)
            {
                // hack as we have just moved the index by one
                m_currentDatabase--;
                m_prevDatabase = 0;
            }
            updateMenuDatabases();
        }
    }
}

void MainWindow::slotFileCloseName(QString fname)
{
    for (int i = 0; i < m_databases.count(); i++)
    {
        if (m_databases[i]->database()->filename() == fname)
        {
            slotFileCloseIndex(i);
            return;
        }
    }
}

void MainWindow::slotFileExportFilter()
{
    /* XXX
	int format;
	QString filename = exportFileName(format);
    if (!filename.isEmpty())
    {
		Output output((Output::OutputType)format);
        output.output(filename, *(databaseInfo()->filter()));
	}
    */
}

void MainWindow::slotFileExportAll()
{
	int format;
    QString filename = exportFileName(format);
    if (!filename.isEmpty())
    {
		Output output((Output::OutputType)format);
		output.output(filename, *database());
	}
}

void MainWindow::slotFileQuit()
{
	qApp->closeAllWindows();
}

void MainWindow::slotPlayerListWidget()
{
    //m_playerList->setDatabase(database());
}

void MainWindow::slotEventListWidget()
{
    //m_eventList->setDatabase(database());
}

void MainWindow::slotConfigure()
{
	PreferencesDialog P(this);
	connect(&P, SIGNAL(reconfigure()), SLOT(slotReconfigure()));
    connect(&P, SIGNAL(reconfigure()), SLOT(slotNotationChanged()));
    P.exec();
}

void MainWindow::slotConfigureGame()
{
    PreferencesDialog P(this);
    P.setPage(1);
    connect(&P, SIGNAL(reconfigure()), SLOT(slotReconfigure()));
    connect(&P, SIGNAL(reconfigure()), SLOT(slotNotationChanged()));
    P.exec();
}

void MainWindow::slotReconfigure()
{
    if (AppSettings->getValue("/MainWindow/VerticalTabs").toBool())
    {
        setDockOptions(QMainWindow::AnimatedDocks | QMainWindow::AllowNestedDocks | QMainWindow::VerticalTabs);
    }
    else
    {
        setDockOptions(QMainWindow::AnimatedDocks | QMainWindow::AllowTabbedDocks | QMainWindow::AllowNestedDocks);
    }
    m_recentFiles.restore();
    updateMenuRecent();
    emit reconfigure(); 	// Re-emit for children
}

void MainWindow::slotConfigureFlip()
{
    m_boardView->setFlipped(!m_boardView->isFlipped());
    AppSettings->setValue("/Board/flipped", QVariant(m_boardView->isFlipped()));
}

void MainWindow::slotEditCopySPN()
{
	QApplication::clipboard()->setText(game().toSPN());
}

void MainWindow::slotEditCopySGN()
{
    Output output(Output::Sgn);
    QString sgn = output.output(&game());
    QApplication::clipboard()->setText(sgn);
}


void MainWindow::slotEditComment()
{
	if (gameEditComment(Output::Comment))
		slotGameChanged();
}

void MainWindow::slotEditCommentBefore()
{
    if (gameEditComment(Output::Precomment))
        slotGameChanged();
}

void MainWindow::slotEditVarPromote()
{
	if (!game().isMainline()) {
		game().promoteVariation(game().currentMove());
		slotGameChanged();
	}
}

void MainWindow::slotEditVarRemove()
{
	if (!game().isMainline()) {
		game().removeVariation(game().currentMove());
		slotGameChanged();
	}
}

void MainWindow::slotEditPasteSPN()
{
	QString spn = QApplication::clipboard()->text().trimmed();
    SHATRA::Board board;
    if (!board.isValidSPN(spn)) {
		QString msg = spn.length() ?
            tr("Text in clipboard does not represent valid SPN:<br><i>%1</i>").arg(spn) :
            tr("There is no text in clipboard.");
		MessageDialog::warning(msg);
		return;
	}
	board.fromSPN(spn);
    if (board.validate() != SHATRA::Valid) {
        MessageDialog::warning(tr("The clipboard contains SPN, but an illegal position. "
            "You can only paste such positions in <b>Setup position</b> dialog."));
		return;
	}
	game().setStartingBoard(board);
//    game().setResult(ResultUnknown);
    game().removeTag(SHATRA::TagNameResult);
	slotGameChanged();
}

void MainWindow::slotEditPasteSGN()
{
    QString sgn = QApplication::clipboard()->text().trimmed();
    qDebug() << sgn;
    if (!sgn.isEmpty())
    {
        MemoryDatabase sgnDatabase;
        sgnDatabase.openString(sgn);
        Game g;
        if (sgnDatabase.loadGame(0,g))
        {
            slotGameNew();
            game() = g;
            slotGameChanged();
        }
        if (g.board().moveNumber() == 1)
        {
            MessageDialog::warning(tr("Text in clipboard does not represent valid SGN"));
            g.clear();
//            m_output->reset();
//            g.setModified(false); // must find out why this doesn't work
        }
    }
}

void MainWindow::slotEditTruncateEnd()
{
	game().truncateVariation(Game::AfterMove);
	slotGameChanged();
}

void MainWindow::slotEditRemoveResult()
{
    game().removeTag(SHATRA::TagNameResult);
    slotGameChanged();
}

void MainWindow::slotEditTruncateStart()
{
	game().truncateVariation(Game::BeforeMove);
	slotGameChanged();
}

void MainWindow::slotEditBoard()
{
	BoardSetupDialog B(this);
	B.setBoard(game().board());
	B.setFlipped(m_boardView->isFlipped());
	if (B.exec() == QDialog::Accepted) {
		game().setStartingBoard(B.board());
		slotGameChanged();
	}
}

void MainWindow::slotEditCopyImage()
{
    QPixmap pixmap(m_boardView->size());
    m_boardView->render(&pixmap);
    QImage image = pixmap.toImage();
    QApplication::clipboard()->setImage(image);
}

void MainWindow::slotHelpBug()
{
//	QDesktopServices::openUrl(QUrl
//    ("http://sourceforge.net/tracker/?group_id=163833&atid=829300"));
}

void MainWindow::slotBoardMove(SHATRA::Square from, SHATRA::Square to, int button)
{
    const SHATRA::Board& board = game().board();
    SHATRA::Move m(board.prepareMove(from, to));
    if (!m.isLegal()) return;

    SHATRA::PieceType promotionPiece = SHATRA::NoPiece;

    if (m.pieceTypeMoved() == SHATRA::Biy
            || SHATRA::g_version == SHATRA::Original)
    {
        if (m.isCapture() && board.moveIsDual(from, to))
        {
            QMessageBox mb;
            mb.setText(tr("You can either capture or drop"));
            mb.setInformativeText(tr("   Yes to capture, No to drop"));
            // NB can add a Cancel button for non-HumanTournament mode
            mb.setStandardButtons(QMessageBox::No | QMessageBox::Yes);
//            if (mb.exec() == QMessageBox::Cancel) return;

            if (mb.exec() == QMessageBox::No)
            {
                m = board.prepareMove2(from, to);
                if (!m.isLegal()) return;
            }
        }
        else if (m.isPassMove())
        {
            QMessageBox mb;
            mb.setText(tr("You have the option not to move"));
            mb.setInformativeText(tr("Do you really want to pass?"));
            mb.setStandardButtons(QMessageBox::No | QMessageBox::Yes);
            if (mb.exec() == QMessageBox::No) return;
        }
    }
    if (m.isPromotion())
    {
        if (SHATRA::g_version == SHATRA::Original)
            m.setPromotionPiece(SHATRA::Batyr);
        else
        {
            bool ok; int i = 0;
            SHATRA::PieceType ix[3];
            QStringList moves;

            if (board.canPromoteTo(SHATRA::Batyr))
                moves << tr("Batyr"), ix[i++] = SHATRA::Batyr;
            if (board.canPromoteTo(SHATRA::Tura))
                moves << tr("Tura"), ix[i++] = SHATRA::Tura;
            if (board.canPromoteTo(SHATRA::Yalkyn))
                moves << tr("Yalkyn"), ix[i++] = SHATRA::Yalkyn;
            if (i)
            {
                int index = moves.indexOf(QInputDialog::getItem(0,
                tr("Promotion"), tr("Promote to:"), moves, 0, false, &ok));
                if (!ok)
                    return;
                promotionPiece = ix[index];
            }
            m.setPromotionPiece(promotionPiece); // set None if none available
        }
    }
    // Use an existing move with the correct promotion type if possible
    if (game().findNextMove(from, to, promotionPiece))
    {
        if (button & Qt::AltModifier)
            game().backward();
        else
        {
            slotGameChanged(); return;
        }
    }

    if (game().atLineEnd())
        game().addMove(m);
    else
    {
        if (button & Qt::ControlModifier)
            game().replaceMove(m);
        else game().addVariation(m);

        game().forward();
    }

    if (m_playGame->isPlaying())
    {
        m_playGame->setPosition(board);
    }

    slotGameChanged();
}

void MainWindow::slotBoardClick(SHATRA::Square s, int button, QPoint pos)
{
    if (button & Qt::RightButton)
    {
        if (button & Qt::ShiftModifier)
        {
            QMenu* menu = new QMenu(this);
            m_annotationSquare = s;
            menu->addAction(QIcon(":/images/square_red.png"),
                  tr("Red Square"), this, SLOT(slotRedSquare()));
            menu->addAction(QIcon(":/images/square_yellow.png"),
                  tr("Yellow Square"), this, SLOT(slotYellowSquare()));
            menu->addAction(QIcon(":/images/square_green.png"),
                  tr("Green Square"),  this, SLOT(slotGreenSquare()));
            menu->addAction(QIcon(":/images/square_none.png"),
                  tr("Remove Color"),  this, SLOT(slotNoColorSquare()));
            menu->addSeparator();
            menu->addAction(tr("Red Arrow to here"));
            menu->addAction(tr("Yellow Arrow to here"));
            menu->addAction(tr("Green Arrow to here"));
            menu->addAction(tr("Remove Arrow to here"));
            menu->exec(pos);
        }
    }
}

void MainWindow::slotMoveChanged()
{
    const Game& g = game();
    bool a = m_mainAnalysis->isEngineRunning()
            && !m_mainAnalysis->isFreeze();
    if (a) m_mainAnalysis->stopEngine();

    // Set board first
    if (m_lastSendBoard != g.board())
        m_boardView->setBoard(g.board(), g.move(g.currentMove()));
    else
        m_boardView->setBoard(g.board());
    m_lastSendBoard = g.board();

    emit displayTime(g.timeAnnotation(), g.board().toMove());

	// Highlight current move
    m_gameView->showMove(g.currentMove());

	slotSearchTree();
	emit boardChange(g.board());
    if (a) m_mainAnalysis->startEngine();

	// Clear  entries
	m_nagText.clear();
}

void MainWindow::slotNotationChanged()
{
    if (SHATRA::g_nChanged)
    {
        slotGameChanged();
        SHATRA::g_nChanged = false;
    }
}

void MainWindow::slotBoardMoveWheel(int wheel)
{
	if (wheel & Qt::AltModifier)
		if (wheel & BoardView::WheelDown) slotGameMoveLast();
		else slotGameMoveFirst();
	else if (wheel & Qt::ControlModifier)
		if (wheel & BoardView::WheelDown) slotGameMoveNextN();
		else slotGameMovePreviousN();
	else
		if (wheel & BoardView::WheelDown) slotGameMoveNext();
		else slotGameMovePrevious();
}

void MainWindow::slotBoardFlipExternal()
{
    m_boardView->setExternal(!m_boardView->isExternal());
    AppSettings->setValue("/Board/external", QVariant(m_boardView->isExternal()));
}

void MainWindow::slotBoardExternalClosed()
{
    m_ExternalBoardAction->setChecked(false);
    AppSettings->setValue("/Board/external", false);
}

void MainWindow::slotGameVarEnter()
{
    if (game().variationCount(game().currentMove()))
    {
		game().moveToId(game().variations().first());
		slotMoveChanged();
	}
}

void MainWindow::slotGameVarExit()
{
    if (!game().isMainline())
    {
		while (!game().atLineStart())
        {
			game().backward();
        }
		game().backward();
		slotMoveChanged();
	}
}

void MainWindow::slotGameAddComment(const QString& s)
{
    //qDebug() << "add game comment" << s;
    game().setAnnotation(s);
    slotGameChanged(false);
}

void MainWindow::slotGameLoadFirst()
{
    gameLoad(0);
    m_gameList->setFocus();
}

void MainWindow::slotGameLoadLast()
{
    gameLoad(databaseInfo()->database()->count() - 1);
	m_gameList->setFocus();
}

void MainWindow::slotGameLoadPrevious()
{
    if (QuerySaveGame())
    {
        int game = m_gameList->currentIndex().row() - 1;
        if (game != -1)
        {
            gameLoad(game);
            m_gameList->setFocus();
        }
    }
}

void MainWindow::loadNextGame()
{
    int game = m_gameList->currentIndex().row() + 1;
    if (game < databaseInfo()->database()->count())
    {
        gameLoad(game);
        m_gameList->setFocus();
    }
}

void MainWindow::slotGameLoadNext()
{
    if (QuerySaveGame()) loadNextGame();
}

void MainWindow::slotGameLoadRandom()
{
    if (databaseInfo()->database()->count())
    {
        int r = rand() % databaseInfo()->database()->count();
        gameLoad(r);
		m_gameList->setFocus();
	}
}

void MainWindow::slotGameLoadChosen()
{
    int index = QInputDialog::getInt(this, tr("Load Game"), tr("Game number:"), gameIndex() + 1,
						  1, database()->count());
	gameLoad(index - 1);
	m_gameList->setFocus();
}

bool MainWindow::slotGameNew()
{
	if (database()->isReadOnly())
    {
		MessageDialog::error(tr("This database is read only."));
    }
    else
    {
        if (QuerySaveGame())
        {
            databaseInfo()->newGame();
            slotGameChanged();
            return true;
        }
	}
    return false;
}

void MainWindow::slotPlayEnableWidgets(bool e, bool strong)
{
    // e1 is strong/tournament
//    const bool e1 = e || !strong;
    e |= !strong;

    m_gameList->setEnabled(e);
    m_databaseList->setEnabled(e);
    m_openingTreeView->setEnabled(e);
    m_autoPlay->setEnabled(e);
    m_menuDatabase->setEnabled(e);
    m_menuEdit->setEnabled(e);
    m_menuFile->setEnabled(e);
    m_menuGame->setEnabled(e);
    m_mainAnalysis->setEnabled(e);
    m_analysis2->setEnabled(e);
    m_gameView->setEnabled(e);
}

void MainWindow::slotPlayGameNew(const QMap<QString, QString>& tags)
{
    if (slotGameNew())
    {
        // disable all non-related functions
        slotPlayEnableWidgets(false, m_playGame->isTournament() && m_playGame->isHumanInvolved());

        // set boardview flags
        m_boardView->setFlags(
                    (BoardView::F_DisableWhite * (!m_playGame->whiteCanMove()))
                  | (BoardView::F_DisableBlack * (!m_playGame->blackCanMove()))
                );

        // transfer tags
        Game &g = databaseInfo()->currentGame();
        for (QMap<QString, QString>::const_iterator i=tags.begin(); i!=tags.end(); ++i)
        {
            g.setTag(i.key(), i.value());
        }
        g.setModified(true);
        slotGameChanged();

        m_playGame->startNewGameOk();
    }
}

void MainWindow::slotPlayGameContinue()
{
    Game &g = databaseInfo()->currentGame();

    m_playGame->continuePosition(g.board());
}

void MainWindow::slotPlayGamePaused()
{
    // restore functionallity
    slotPlayEnableWidgets(true);

    // restore boardview flags
    m_boardView->setFlags(0);
}

void MainWindow::slotPlayGameEnd()
{
    game().setTag("Result", m_playGame->resultString());
    slotGameChanged();

    // restore functionallity
    slotPlayEnableWidgets(true);

    // restore boardview flags
    m_boardView->setFlags(0);

    if (m_playGame->doAutoSaveAndContinue())
    {
        m_nextGameSaveQuick = true; /* avoid save dialog */
        // XXX wait a while before restart
        QTimer * t = new QTimer(this);
        t->setInterval(250);
        connect(t,SIGNAL(timeout()),m_playGame,SLOT(startNewGame()));
        connect(t,SIGNAL(timeout()),t,SLOT(deleteLater()));
        t->start();
    }
}

void MainWindow::slotPlayGameMove(SHATRA::Move m)
{
    if (game().atLineEnd())
        game().addMove(m);
    else
    {
        // only add variation when different
        // XXX won't work, won't trigger animationFinished() for play game
        //if (!(m == game().move(game().currentMove())))

        game().addVariation(m);

        game().forward();
    }

    if (m_playGame->isBoardUpdate())
        slotGameChanged(true);
    else
    {
        //slotGameChanged(false);
        m_playGame->animationFinished(game().board());
    }
}

void MainWindow::slotPlayPlayerWins()
{
    slotPlayGameEnd();
    QMessageBox::information(this,
                             tr("!!!"),
                             tr("You win!\n(XXX This should trigger an animation)")
                             );
}

void MainWindow::slotPlayOtherWins()
{
    slotPlayGameEnd();
    QMessageBox::information(this,
                             tr("..."),
                             tr("You did not win!\n(XXX This should trigger an animation)")
                             );
}

void MainWindow::slotBoardAnimationFinished(const SHATRA::Board& b)
{
    m_playGame->animationFinished(b);
}

void MainWindow::saveGame()
{
    if (!database()->isReadOnly())
    {
        databaseInfo()->saveGame();
        //database()->index()->setTag("Length", QString::number(game().moveCount()), gameIndex() );
        database()->addGameInfo(gameIndex(), &game());
        //m_gameList->updateFilter();
        slotFilterChanged();
        slotGameChanged();
        slotDatabaseChanged();
        game().setModified(false);

        if (AppSettings->getValue("/General/autoCommitDB").toBool())
        {
            saveDatabase();
        }
    }
}

bool MainWindow::slotGameSave()
{
    if (m_nextGameSaveQuick)
    {
        m_nextGameSaveQuick = false;
        saveGame();
        return true;
    }

    if (database()->isReadOnly())
    {
        MessageDialog::error(tr("This database is read only."));
        game().setModified(false); // Do not notify more than once
        return true;
    }

    int n = saveDialog()->exec(database(), game());
    if (n==QDialog::Accepted)
    {
        saveGame();
    }
    return (n!=QDialog::Rejected);
}

void MainWindow::slotGameModify(const EditAction& action)
{
	game().moveToId(action.move());
	slotMoveChanged();
	switch (action.type()) {
	case EditAction::RemoveNextMoves:
		game().truncateVariation();
		break;
	case EditAction::RemovePreviousMoves:
		game().truncateVariation(Game::BeforeMove);
		break;
	case EditAction::RemoveVariation: {
		game().removeVariation(game().variationNumber());
		break;
	}
	case EditAction::PromoteVariation: {
		game().promoteVariation(action.move());
		break;
	}
    case EditAction::VariationUp: {
        game().moveVariationUp(action.move());
        break;
    }
    case EditAction::VariationDown: {
        game().moveVariationDown(action.move());
        break;
    }
	case EditAction::EditPrecomment:
		if (!gameEditComment(Output::Precomment))
			return;
		break;
	case EditAction::EditComment:
		if (!gameEditComment(Output::Comment))
			return;
		break;
	case EditAction::AddNag:
		game().addNag(Nag(action.data().toInt()), action.move());
		break;
	case EditAction::ClearNags:
        game().clearNags(action.move());
		break;
	default:
		break;
	}
	slotGameChanged();
}


void MainWindow::slotGameChanged(bool updateMove)
{
	if (m_showSgnSource)
		m_gameView->setPlainText(m_output->output(&game()));
	else
		m_gameView->setText(m_output->output(&game()));

	// Finally update game information
	QString white = game().tag("White");
	QString black = game().tag("Black");
    QString eco; // = game().tag("ECO").left(3);
	QString whiteElo = game().tag("WhiteElo");
	QString blackElo = game().tag("BlackElo");
	if (whiteElo == "?")
		whiteElo = QString();
	if (blackElo == "?")
		blackElo = QString();
	QString players = QString("<b><a href=\"tag:white\">%1</a></b> %2 - <b><a href=\"tag:black\">%3</a></b> %4")
			  .arg(white).arg(whiteElo).arg(black).arg(blackElo);
	QString result = QString("<b>%1</b> &nbsp; %2").arg(game().tag("Result")).arg(eco);
	QString site = game().tag("Site").left(30);
	QString event = game().tag("Event").left(30);
	QString header = "<i>";
	if (!event.isEmpty()) {
		header.append(site);
        if (game().result() != SHATRA::ResultUnknown)
			header.append(QString(" (%1)").arg(game().tag("Round")));
		if (!site.isEmpty())
			header.append(", ");
	}
	header.append(site);
	if (!game().tag("Date").startsWith("?")) {
		if (header.length() > 4)
			header.append(", ");
		header.append(game().tag("Date"));
	}
	header.append("</i>");

	QString title;
	if (!white.isEmpty() || !black.isEmpty())
		title.append(players);
	else title.append(tr("<b>New game</b>"));
    if (game().result() != SHATRA::ResultUnknown || !eco.isEmpty())
		title.append(QString(", ") + result);
	if (header.length() > 8)
		title.append(QString("<br>") + header);
    m_gameTitle->setText(QString("<qt>%1</qt>").arg(title));

    if (updateMove)
        slotMoveChanged();
}

void MainWindow::slotGameViewLink(const QUrl& url)
{
	if (url.scheme() == "move") {
		if (url.path() == "prev") game().backward();
		else if (url.path() == "next") game().forward();
		else if (url.path() == "exit") game().moveToId(game().parentMove());
		else
			game().moveToId(url.path().toInt());
		slotMoveChanged();
	} else if (url.scheme() == "precmt" || url.scheme() == "cmt") {
		game().moveToId(url.path().toInt());
		slotMoveChanged();
		Output::CommentType type = url.scheme() == "cmt" ? Output::Comment : Output::Precomment;
		if (gameEditComment(type))
			slotGameChanged();
	} else if (url.scheme() == "egtb") {
		if (!game().atGameEnd())
			game().addVariation(url.path());
		else
            game().addMove(url.path());
		game().forward();
		slotGameChanged();
    } else if (url.scheme() == "tag")
    {
        /*
        m_playerList->setDatabase(database());
		if (url.path() == "white")
        {
            m_playerList->slotSelectPlayer(game().tag("White"));
        }
		else if (url.path() == "black")
        {
            m_playerList->slotSelectPlayer(game().tag("Black"));
        }
        */
	}
}

void MainWindow::slotGameViewLink(const QString& url)
{
	slotGameViewLink(QUrl(url));
}

void MainWindow::slotGameViewToggle(bool toggled)
{
	m_showSgnSource = toggled;
	slotGameChanged();
}

void MainWindow::slotGameDumpMoveNodes()
{
    game().dumpAllMoveNodes();
}

void MainWindow::slotGameAddVariation(const Analysis& analysis)
{
	game().addVariation(analysis.variation(),
								QString::number(analysis.score() / 100.0, 'f', 2));
	slotGameChanged();
}

void MainWindow::slotGameAddVariation(const QString& san)
{
    QString s = san;
    s = s.remove(QRegExp("-.*"));
    s = s.remove(QRegExp("[0-9]*\\."));
    if (game().atLineEnd())
        game().addMove(s);
    else
        game().addVariation(s);
    slotGameChanged();
}

void MainWindow::slotGameUncomment()
{
    game().removeComments();
    slotGameChanged();
}

void MainWindow::slotGameRemoveVariations()
{
    game().removeVariations();
    slotGameChanged();
}

void MainWindow::slotToggleAutoAnalysis()
{
    slotToggleAutoPlayer();
    if (m_autoAnalysis->isChecked() && !m_mainAnalysis->isEngineRunning())
    {
        MessageDialog::information(
            tr("Analysis Pane 1 is not running an engine for automatic analysis."),
            tr("Auto Analysis"));
    }
}

void MainWindow::slotToggleAutoPlayer()
{
    QAction* autoPlayAction = (QAction*) sender();
    if (autoPlayAction)
    {
        if (autoPlayAction->isChecked())
        {
            QAction* otherAction = (autoPlayAction == m_autoPlay) ?
                        m_autoAnalysis : m_autoPlay;
            otherAction->setChecked(false);
            int interval = 2000;//XXX AppSettings->getValue("/Board/AutoPlayerInterval").toInt();
            if (m_autoPlayTimer->interval() != interval)
            {
                m_autoPlayTimer->setInterval(interval);
            }
            m_autoPlayTimer->start();
        }
        else
        {
            m_autoPlayTimer->stop();
        }
    }
}

void MainWindow::slotAutoPlayTimeout()
{
    if (m_autoAnalysis->isChecked()
    && m_mainAnalysis->isEngineRunning()
    && !m_mainAnalysis->isFreeze()
    && (m_AutoInsertLastBoard != m_boardView->board()))
    {
        Analysis a = m_mainAnalysis->getMainLine();
        if (!a.variation().isEmpty())
        {
            SHATRA::Move m = a.variation().first();
            if (!game().currentNodeHasMove(m.from(), m.to()))
            {
                slotGameAddVariation(a);
            }
        }
        m_AutoInsertLastBoard = m_boardView->board();
    }
    if (game().atGameEnd() && AppSettings->getValue("/Board/AutoSaveAndContinue").toBool())
    {
        saveGame();
        loadNextGame();
    }
    else
    {
        slotGameMoveNext();
    }
    m_autoPlayTimer->start();
}

void MainWindow::slotFilterChanged()
{
	if (gameIndex() >= 0)
		m_gameList->selectGame(gameIndex());
    //int count = databaseInfo()->database()->count();
    //QString f = count == database()->count() ? "all" : QString::number(count);
    m_statusFilter->setText(QString(" %1: %2 ")
                .arg(databaseName())
                .arg(database()->count()));
}

void MainWindow::slotFilterLoad(int index)
{
	gameLoad(index);
	activateWindow();
}

void MainWindow::slotStatusMessage(const QString& msg)
{
	statusBar()->showMessage(msg);
}

void MainWindow::slotOperationProgress(int progress)
{
    m_progressBar->setValue(progress);
}

void MainWindow::slotDatabaseChange()
{
	QAction* action = qobject_cast<QAction*>(sender());
	if (action && m_currentDatabase != action->data().toInt()) {
        m_prevDatabase = m_currentDatabase;
        database()->index()->clearCache();
		m_currentDatabase = action->data().toInt();
        m_databaseList->setFileCurrent(m_databases[m_currentDatabase]->filePath());
		slotDatabaseChanged();
	}
}

void MainWindow::slotDatabaseFindDuplicates()
{
    bool do_sym = QMessageBox::question(this, database()->name(),
                                tr("Also look for symmetric/mirrored duplicates?"))
                    == QMessageBox::Yes;

    QVector<int> v;
    database()->findDuplicates(v, do_sym);

    if (v.empty())
    {
        QMessageBox::information(this, database()->name(),
                                 tr("No duplicate games found in database."));
    }
    else
    {
        if (QMessageBox::question(this, database()->name(),
                              tr("There are %1 duplicate games in this Database, "
                                 "do you want to flag them as deleted?").arg(v.count()))
            == QMessageBox::Yes)
        {
            for (int i=0; i<v.count(); ++i)
                database()->remove(v[i]);
        }
    }
}


void MainWindow::copyGame(int target, const Game& game)
{
    if (m_databases[target]->isValid())
    {
        // The database is open and accessible
        m_databases[target]->database()->appendGame(game);
    }
}

void MainWindow::copyGame(QString fileName, const Game& game)
{
    for (int i=0; i < m_databases.count(); ++i)
    {
        if (m_databases[i]->filePath() == fileName)
        {
            if (m_databases[i]->isValid())
            {
                copyGame(i,game);
                //XXX m_databases[i]->filter()->resize(m_databases[i]->database()->count(), true);
            }
            return;
        }
    }

    // The database is closed
    Output writer(Output::Sgn);
    Game g = game;
    writer.append(fileName, g);
    m_databaseList->update(fileName);
}

void MainWindow::slotDatabaseCopy(int preselect)
{
	if (m_databases.count() < 2) {
		MessageDialog::error(tr("You need at least two open databases to copy games"));
		return;
	}
	CopyDialog dlg(this);
    dlg.setMode((CopyDialog::SrcMode)preselect);
    dlg.setSelectionVisible(m_gameList->numSelected());

	QStringList db;
    for (int i = 0; i < m_databases.count(); ++i)
		if (i != m_currentDatabase)
            db.append(tr("%1. %2 (%3 games)")
                      .arg(i)
                      .arg(databaseName(i))
                      .arg(m_databases[i]->database()->count()));
	dlg.setDatabases(db);
	if (dlg.exec() != QDialog::Accepted)
		return;

	int target = dlg.getDatabase();
	if (target >= m_currentDatabase)
		target++;

    Game g;
    switch (dlg.getMode())
    {
	case CopyDialog::SingleGame:
		m_databases[target]->database()->appendGame(game());
		break;
	case CopyDialog::Filter:
        /* XXX
        for (int i = 0; i < database()->count(); ++i)
			if (databaseInfo()->filter()->contains(i) && database()->loadGame(i, g))
                m_databases[target]->database()->appendGame(g);
        */
		break;
    case CopyDialog::SelectedGames:
        for (int i = 0; i < m_gameList->numSelected(); ++i)
        {
            qDebug() << "--" << m_gameList->selectedGame(i);
            if (database()->loadGame(m_gameList->selectedGame(i), g))
                m_databases[target]->database()->appendGame(g);
        }
        break;
    case CopyDialog::AllGames:
        for (int i = 0; i < database()->count(); ++i)
            if (database()->loadGame(i, g))
				m_databases[target]->database()->appendGame(g);
		break;
	default:
        return;
	}
    slotDatabaseChanged();
    // XXX m_databases[target]->filter()->resize(m_databases[target]->database()->count(), true);
}

void MainWindow::slotDatabaseCopySingle()
{
    slotDatabaseCopy(0);
}

void MainWindow::slotDatabaseChanged()
{
    database()->index()->calculateCache();
    setWindowTitle(tr("%1 - Shatrabase").arg(databaseName()));
    m_gameList->setDatabaseInfo(databaseInfo());
	slotFilterChanged();
    gameLoad(gameIndex()>=0 ? gameIndex() : 0, true, true);
    //m_playerList->setDatabase(database());
    //m_eventList->setDatabase(database());
    if (m_stats)
        m_stats->setDatabaseModel(*static_cast<DatabaseModel*>(m_gameList->model()));
	emit databaseChanged(databaseInfo());
}

void MainWindow::slotSearchTag()
{
	m_gameList->simpleSearch(1);
}

void MainWindow::slotSearchBoard()
{
    PositionSearch ps(databaseInfo()->database(), m_boardView->board());
    m_openingTree->cancel(false);
    slotBoardSearchStarted();
    // XXX databaseInfo()->filter()->executeSearch(ps);
    slotBoardSearchUpdate();
}

void MainWindow::slotBoardSearchUpdate()
{
    m_gameList->updateFilter();
    slotFilterChanged();
    finishOperation(tr("Search ended"));
}

void MainWindow::slotBoardSearchStarted()
{
    startOperation(tr("Updating tree..."));
}

void MainWindow::slotSearchReverse()
{
    /* XXX databaseInfo()->filter()->reverse();
	m_gameList->updateFilter();
    slotFilterChanged();
    */
}

void MainWindow::slotSearchReset()
{
    /* XXX databaseInfo()->filter()->setAll(1);
	m_gameList->updateFilter();
    slotFilterChanged(); */
}

void MainWindow::slotToggleFilter()
{
    /* XXX m_gameList->m_FilterActive = m_toggleFilter->isChecked();
    m_gameList->updateFilter(); */
}

void MainWindow::slotTreeUpdate()
{
    /* XXX
    if (m_gameList->m_FilterActive)
    {
        m_gameList->updateFilter();
        slotFilterChanged();
    } */
    finishOperation(tr("Tree updated."));
    if (m_bGameChange)
    {
        slotGameLoadFirst();
        m_bGameChange = false;
    }
}

void MainWindow::slotTreeUpdateStarted()
{
    startOperation(tr("Updating tree..."));
}

void MainWindow::slotSearchTree()
{
    if (m_openingTreeView->isVisible() )
    {
        // XXX
        m_openingTree->update(*databaseInfo()->database(),
                              m_boardView->board(), false);//m_gameList->m_FilterActive);
	}
}

void MainWindow::slotSearchTreeMove(const QModelIndex& index)
{
    m_bGameChange = false;
    QString move = qobject_cast<OpeningTree*>(m_openingTreeView->model())->move(index);
    SHATRA::Move m = m_boardView->board().parseMove(move);
	if (!m.isLegal())
		return;
	else if (m == game().move(game().nextMove()))
		slotGameMoveNext();
    else
        slotBoardMove(m.from(), m.to(), 0);
}

void MainWindow::slotDatabaseDeleteGames(const QVector<int>& idxs)
{
    for (int i=0; i<idxs.size(); ++i)
    {
        const int n = idxs[i];

        if (database()->deleted(n))
        {
            database()->undelete(n);
        }
        else
        {
            database()->remove(n);
        }
    }
    m_gameList->updateFilter();
}

void MainWindow::slotDatabaseDeleteFilter()
{
    /* XXX database()->remove(*databaseInfo()->filter());
    m_gameList->updateFilter(); */
}

void MainWindow::slotDatabaseCompact()
{
    /* XXX
	database()->compact();
	databaseInfo()->resetFilter();
	slotDatabaseChanged();
	m_gameList->updateFilter();
    */
}

void MainWindow::slotGetGameData(Game& g)
{
    g = game();
}

bool MainWindow::slotGameMoveNext()
{
  /*Move m = game().move(game().nextMove());
    m_currentFrom = BN[m.from()];
    m_currentTo = BN[m.to()];
    */
    return gameMoveBy(1);
}

void MainWindow::slotNoColorSquare()
{
    game().appendSquareAnnotation(m_annotationSquare, 0);
    slotGameChanged();
}

void MainWindow::slotGreenSquare()
{
    game().appendSquareAnnotation(m_annotationSquare, 'G');
    slotGameChanged();
}

void MainWindow::slotYellowSquare()
{
    game().appendSquareAnnotation(m_annotationSquare, 'Y');
    slotGameChanged();
}

void MainWindow::slotRedSquare()
{
    game().appendSquareAnnotation(m_annotationSquare, 'R');
    slotGameChanged();
}

void MainWindow::slotTestEngineEngine()
{
    USHIEngineTester * test = new USHIEngineTester(this);
    connect(test, SIGNAL(stopped()), test, SLOT(deleteLater()));

    EngineList elist;
    elist.restore();
    QString bin = elist[0].command;

    test->setBinary(0,bin);
    test->setBinary(1,bin);
    test->startTests();
}

void MainWindow::slotTestPositionBase()
{
    QString fn = QFileDialog::getSaveFileName(this, tr("Save game positions"),
                                              "./");
    if (fn.isEmpty())
        return;

    PositionBase pb;

    pb.addDatabase(*database());
    pb.saveFile(fn.toLatin1().constData());
}
