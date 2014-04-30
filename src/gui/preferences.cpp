/***************************************************************************
 *                           Shatrabase                                    *
 *   (C) 2012-2014 AFKM <francis@afkm.com>                                 *
 *                   derived from ChessX with thanks                       *
 *   (C) 2005 Michal Rudolf <mrudolf@kdewebdev.org>                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "common.h"
#include "colorlist.h"
#include "preferences.h"
#include "settings.h"
#include "timecontrol.h"
#include "messagedialog.h"
#include "engineoptiondialog.h"

#include <QCheckBox>
#include <QColorDialog>
#include <QComboBox>
#include <QListWidget>
#include <QPainter>
#include <QDir>
#include <QPushButton>
#include <QSpinBox>
#include <QRadioButton>
#include <QFileDialog>
#include <QDesktopServices>
#include <QMessageBox>

int PreferencesDialog::s_lastIndex = 0;

// helper for timeedit <-> int

void setTime(QTimeEdit& te, int msec)
{
    QTime t(msec/1000/60/60, (msec/1000/60)%60, (msec/1000)%60, msec%1000);
    te.setTime(t);
}

int getMSecs(const QTimeEdit& te)
{
    QTime t = te.time();
    return (((t.hour() * 60 + t.minute()) * 60 + t.second()) * 1000) + t.msec();
}



PreferencesDialog::PreferencesDialog(QWidget* parent) : QDialog(parent)
{
	ui.setupUi(this);
/*
#ifndef Q_WS_WIN
	ui.engineProtocolWinBoard->setText(tr("XBoard"));
#endif
*/
    ui.cbMoveOnLoad->addItem(tr("go to beginning of game"));
    ui.cbMoveOnLoad->addItem(tr("go to first move of game"));
    ui.cbMoveOnLoad->addItem(tr("go to last move of game"));

	connect(ui.okButton, SIGNAL(clicked()), SLOT(accept()));
    connect(ui.resetButton, SIGNAL(clicked()), SLOT(slotReset()));
	connect(ui.cancelButton, SIGNAL(clicked()), SLOT(reject()));
	connect(ui.applyButton, SIGNAL(clicked()), SLOT(slotApply()));
	connect(ui.engineList, SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)),
		SLOT(slotSelectEngine(QListWidgetItem*, QListWidgetItem*)));
	connect(ui.engineName, SIGNAL(textChanged(const QString&)), SLOT(slotEngineNameChange(const QString&)));
	connect(ui.addEngineButton, SIGNAL(clicked(bool)), SLOT(slotAddEngine()));
	connect(ui.deleteEngineButton, SIGNAL(clicked(bool)), SLOT(slotDeleteEngine()));
	connect(ui.engineUpButton, SIGNAL(clicked(bool)), SLOT(slotEngineUp()));
	connect(ui.engineDownButton, SIGNAL(clicked(bool)), SLOT(slotEngineDown()));
	connect(ui.directoryButton, SIGNAL(clicked(bool)), SLOT(slotSelectEngineDirectory()));
	connect(ui.commandButton, SIGNAL(clicked(bool)), SLOT(slotSelectEngineCommand()));
    connect(ui.browsePathButton, SIGNAL(clicked(bool)), SLOT(slotSelectDataBasePath()));
    connect(ui.engineOptionMore, SIGNAL(clicked(bool)), SLOT(slotShowOptionDialog()));
    connect(ui.notationNumeric, SIGNAL(clicked()), SLOT(slotNumericNotation()));
    connect(ui.notationAlgebraic, SIGNAL(clicked()), SLOT(slotAlgebraicNotation()));

    connect(ui.cbMatchTime, SIGNAL(clicked()), SLOT(slotTCEnable()));
    connect(ui.cbLimit, SIGNAL(clicked()), SLOT(slotTCEnable()));
    connect(ui.cbTournament, SIGNAL(clicked()), SLOT(slotTCEnable()));
    connect(ui.cbLimitDepth, SIGNAL(clicked()), SLOT(slotTCEnable()));
    connect(ui.cbLimitTime, SIGNAL(clicked()), SLOT(slotTCEnable()));
    connect(ui.cbLimitNodes, SIGNAL(clicked()), SLOT(slotTCEnable()));
    connect(ui.cbTimeInc1, SIGNAL(clicked()), SLOT(slotTCEnable()));
    connect(ui.cbTime2, SIGNAL(clicked()), SLOT(slotTCEnable()));
    connect(ui.cbTimeInc2, SIGNAL(clicked()), SLOT(slotTCEnable()));
    connect(ui.cbTimeInc3, SIGNAL(clicked()), SLOT(slotTCEnable()));
    connect(ui.cbAllMoves1, SIGNAL(clicked()), SLOT(slotTCEnable()));
    connect(ui.cbAllMoves2, SIGNAL(clicked()), SLOT(slotTCEnable()));

    connect(ui.comboTimeFormat, SIGNAL(currentIndexChanged(int)), SLOT(slotTCUpdate()));
    connect(ui.time1, SIGNAL(timeChanged(QTime)), SLOT(slotTCUpdate()));
    connect(ui.time2, SIGNAL(timeChanged(QTime)), SLOT(slotTCUpdate()));
    connect(ui.time3, SIGNAL(timeChanged(QTime)), SLOT(slotTCUpdate()));
    connect(ui.timeInc1, SIGNAL(timeChanged(QTime)), SLOT(slotTCUpdate()));
    connect(ui.timeInc2, SIGNAL(timeChanged(QTime)), SLOT(slotTCUpdate()));
    connect(ui.timeInc3, SIGNAL(timeChanged(QTime)), SLOT(slotTCUpdate()));
    connect(ui.moves1, SIGNAL(valueChanged(int)), SLOT(slotTCUpdate()));
    connect(ui.moves2, SIGNAL(valueChanged(int)), SLOT(slotTCUpdate()));
    connect(ui.limitTime, SIGNAL(timeChanged(QTime)), SLOT(slotTCUpdate()));
    connect(ui.limitDepth, SIGNAL(valueChanged(int)), SLOT(slotTCUpdate()));
    connect(ui.limitNodes, SIGNAL(valueChanged(int)), SLOT(slotTCUpdate()));

    // language combo
    const QMap<QString,QString> lang = AppSettings->languages();
    for (QMap<QString,QString>::const_iterator i=lang.begin(); i!=lang.end(); ++i)
    {
        ui.languageCombo->addItem(i.value(), QVariant(i.key()));
    }
    connect(ui.languageCombo, SIGNAL(activated(int)), SLOT(slotShowLanguageMessage()));

    // timeformat combo
    for (int i=0; i<TimeControl::MaxFormat; ++i)
        ui.comboTimeFormat->addItem(TimeControl::formatNameTr((TimeControl::Format)i));

	restoreSettings();

	// Start off with no Engine selected
	ui.engineEditWidget->setEnabled(false);
    ui.tabWidget->setCurrentIndex(s_lastIndex);

#ifdef SB_NO_CLOCKS
    ui.tabTimeControl->setEnabled(false);
#endif
}

void PreferencesDialog::slotTCEnable()
{
    // keep one of limits on
    if (!(ui.cbLimitTime->isChecked() || ui.cbLimitDepth->isChecked() ||
          ui.cbLimitNodes->isChecked()))
        ui.cbLimitTime->setChecked(true);

    // limit
    bool on = ui.cbLimit->isChecked();
    ui.cbLimitTime->setEnabled(on);
    ui.cbLimitDepth->setEnabled(on);
    ui.cbLimitNodes->setEnabled(on);
    ui.limitTime->setEnabled(on && ui.cbLimitTime->isChecked());
    ui.limitDepth->setEnabled(on && ui.cbLimitDepth->isChecked());
    ui.limitNodes->setEnabled(on && ui.cbLimitNodes->isChecked());

    // tournament
    bool all1 = ui.cbAllMoves1->isChecked();
    bool all2 = ui.cbAllMoves2->isChecked();
    // sanity
    bool tc3 = (!all1 && (!ui.cbTime2->isChecked() || !all2));
    if (all1)
        ui.cbTime2->setChecked(false);
    // 1
    on = ui.cbTournament->isChecked();
    ui.cbAllMoves1->setEnabled(on);
    ui.moves1->setEnabled(on && !all1);
    ui.time1->setEnabled(on);
    ui.cbTimeInc1->setEnabled(on);
    ui.timeInc1->setEnabled(on && ui.cbTimeInc1->isChecked());
    // 2
    ui.cbTime2->setEnabled(on & !all1);
    bool on1 = on & ui.cbTime2->isChecked() & !all1;
    ui.cbAllMoves2->setEnabled(on1);
    ui.moves2->setEnabled(on1 && !all2);
    ui.time2->setEnabled(on1);
    ui.cbTimeInc2->setEnabled(on1);
    ui.timeInc2->setEnabled(on1 && ui.cbTimeInc2->isChecked());
    // 3
    on1 = on && tc3;
    ui.time3->setEnabled(on1);
    ui.cbTimeInc3->setEnabled(on1);
    ui.timeInc3->setEnabled(on1 && ui.cbTimeInc3->isChecked());

    slotTCUpdate();
}

void PreferencesDialog::slotTCUpdate()
{
    TimeControl tc;

    if (ui.cbMatchTime->isChecked())
        tc.setType(TimeControl::T_Match);
    else
    if (ui.cbTournament->isChecked())
        tc.setType(TimeControl::T_Tournament);
    else
//    if (ui.cbLimit->isChecked())
        tc.setType(TimeControl::T_Limit);

    if (ui.comboTimeFormat->currentIndex()>=0)
        tc.setFormat((TimeControl::Format)ui.comboTimeFormat->currentIndex());
    else
        tc.setFormat(TimeControl::F_Long);

    tc.setDepthLimit(ui.cbLimitDepth->isChecked()? ui.limitDepth->value() : TimeControl::Unlimited);
    tc.setNodeLimit(ui.cbLimitNodes->isChecked()? ui.limitNodes->value() : TimeControl::Unlimited);
    tc.setTimeLimit(ui.cbLimitTime->isChecked()? getMSecs(*ui.limitTime) : TimeControl::Unlimited);

    bool tc2 = ui.cbTime2->isChecked(),
         all1 = ui.cbAllMoves1->isChecked(),
         all2 = ui.cbAllMoves2->isChecked(),
         tc3 = !all1 && (!all2 || !tc2);
    tc.setNumMoves1(all1? TimeControl::Unlimited : ui.moves1->value());
    tc.setNumMoves2(tc2? (all2? TimeControl::Unlimited : ui.moves2->value()) : 0);
    tc.setTimeForMoves1(getMSecs(*ui.time1));
    tc.setTimeForMoves2(getMSecs(*ui.time2));
    tc.setTimeAdd(tc3? getMSecs(*ui.time3) : 0);
    tc.setTimeInc1(ui.cbTimeInc1->isChecked()? getMSecs(*ui.timeInc1) : 0);
    tc.setTimeInc2(ui.cbTimeInc2->isChecked() && tc2? getMSecs(*ui.timeInc2) : 0);
    tc.setTimeInc3(ui.cbTimeInc3->isChecked() && tc3? getMSecs(*ui.timeInc3) : 0);

    ui.labelTC->setText(tc.humanReadable());
}


PreferencesDialog::~PreferencesDialog()
{
}

void PreferencesDialog::done( int r ) {
    QDialog::done( r );
    close();
}
void PreferencesDialog::closeEvent(QCloseEvent*)
{
	AppSettings->setLayout(this);
}

void PreferencesDialog::slotSelectEngineDirectory()
{
	QString dir = QFileDialog::getExistingDirectory(this,
					tr("Select engine directory"), ui.engineDirectory->text(),
					QFileDialog::ShowDirsOnly);
    if (QDir(dir).exists())
		ui.engineDirectory->setText(dir);
}

void PreferencesDialog::slotSelectDataBasePath()
{
    QString dir = QFileDialog::getExistingDirectory(this,
                    tr("Select databases folder"), ui.defaultDataBasePath->text(),
                    QFileDialog::ShowDirsOnly);
    if (QDir(dir).exists())
        ui.defaultDataBasePath->setText(dir);
}

void PreferencesDialog::slotAddEngine()
{
	QString command = selectEngineFile();
	if (command.isEmpty())
		return;
	QString name = EngineData::commandToName(command);
    if (name.isEmpty()) name = tr("New Engine");
	EngineData data(name);
	data.command = command;
	engineList.append(data);
	ui.engineList->addItem(name);
	ui.engineList->setCurrentRow(engineList.count() - 1);
}

void PreferencesDialog::slotSelectEngineCommand()
{
	QString command = selectEngineFile(ui.engineCommand->text());
	if (!command.isEmpty()) {
		ui.engineCommand->setText(command);
		ui.engineName->setText(EngineData::commandToName(command));
	}
}

void PreferencesDialog::slotDeleteEngine()
{
	int row = ui.engineList->currentRow();
	if (row >= 0) {
		// Looks like it should crash, but it first removes
		// the item, then it updates it by slotSelectEngine (which is obsolete)
		// and only then it deletes it and removes the data
		QListWidgetItem *del = ui.engineList->takeItem(row);
		delete del;
		engineList.removeAt(row);
	}
}

void PreferencesDialog::slotEngineNameChange(const QString& name)
{
	if (ui.engineList->currentItem()) {
		ui.engineList->currentItem()->setText(name);
		engineList[ui.engineList->currentIndex().row()].name = name;
	}
}

void PreferencesDialog::slotEngineUp()
{
	int index = ui.engineList->currentIndex().row();
	if (index > 0) {
		engineList.swap(index, index - 1);
		QListWidgetItem* item = ui.engineList->takeItem(index - 1);
		ui.engineList->insertItem(index, item);
	}
}

void PreferencesDialog::slotEngineDown()
{
	int index = ui.engineList->currentIndex().row();
	if (index < ui.engineList->count() - 1) {
		engineList.swap(index, index + 1);
		QListWidgetItem* item = ui.engineList->takeItem(index + 1);
		ui.engineList->insertItem(index, item);
	}
}

void PreferencesDialog::updateEngineData(int index)
{
	if (index < 0 || index >= engineList.count())
		return;
	engineList[index].name = ui.engineName->text();
	engineList[index].command = ui.engineCommand->text();
	engineList[index].options = ui.engineOptions->text();
	engineList[index].directory = ui.engineDirectory->text();
	engineList[index].protocol = ui.engineProtocolUnknown->isChecked() ?
					  EngineData::Unknown : EngineData::USHI;
}

void PreferencesDialog::slotSelectEngine(QListWidgetItem* currentItem, QListWidgetItem* previoushitem)
{
	int previous = ui.engineList->row(previoushitem);
	int current = ui.engineList->row(currentItem);

	if (previous != -1)
		updateEngineData(previous);

	if (current != -1) {
		ui.engineEditWidget->setEnabled(true);
		// Fill edit fields with data for selected engine
		ui.engineName->setText(engineList[current].name);
		ui.engineCommand->setText(engineList[current].command);
		ui.engineOptions->setText(engineList[current].options);
		ui.engineDirectory->setText(engineList[current].directory);
        ui.engineOptionMore->setEnabled(true);
		if (engineList[current].protocol == EngineData::Unknown)
			ui.engineProtocolUnknown->setChecked(true);
		else ui.engineProtocolUSHI->setChecked(true);
	} else {
		ui.engineName->clear();
		ui.engineCommand->clear();
		ui.engineOptions->clear();
		ui.engineDirectory->clear();
		ui.engineProtocolUSHI->setChecked(true);
		ui.engineEditWidget->setEnabled(false);
        ui.engineOptionMore->setEnabled(false);
	}
}

QString PreferencesDialog::selectEngineFile(const QString& oldpath)
{
	return QFileDialog::getOpenFileName(this, tr("Select engine executable"),
					oldpath);
}

void PreferencesDialog::slotShowOptionDialog()
{
    int index = ui.engineList->currentIndex().row();
    updateEngineData(index);
    EngineOptionDialog dlg(0, engineList, index);
    if (dlg.exec() == QDialog::Accepted)
    {
        engineList[index].m_optionValues = dlg.GetResults();
    }
}

void PreferencesDialog::slotNumericNotation()
{
    g_notation = false; g_nChanged = true;
    ui.notationNumeric->setChecked(true);
}

void PreferencesDialog::slotAlgebraicNotation()
{
    g_notation = true; g_nChanged = true;
    ui.notationAlgebraic->setChecked(true);
}

int PreferencesDialog::exec()
{
	int result = QDialog::exec();
    s_lastIndex = ui.tabWidget->currentIndex();
	if (result == QDialog::Accepted) {
		saveSettings();
		emit reconfigure();
	}
	return result;
}

void PreferencesDialog::slotReset()
{
    if (MessageDialog::yesNo(tr("Clear all application settings?"),tr("Warning")))
    {
        AppSettings->clear();
        restoreSettings();
        emit reconfigure();
    }
}

void PreferencesDialog::slotApply()
{
	saveSettings();
	emit reconfigure();
}


void PreferencesDialog::restoreSettings()
{
    // Restore size
	AppSettings->layout(this);

    // Read general settings
    AppSettings->beginGroup("/General/");
    QString locale = AppSettings->getValue("Language").toString();
    for (int i=0; i<ui.languageCombo->count(); ++i)
        if (ui.languageCombo->itemData(i).toString() == locale)
            ui.languageCombo->setCurrentIndex(i);
    ui.notationNumeric->setChecked(!AppSettings->getValue("Notation").toBool());
    ui.notationAlgebraic->setChecked(AppSettings->getValue("Notation").toBool());
    g_notation = AppSettings->getValue("Notation").toBool();
    ui.useIndexFile->setChecked(AppSettings->getValue("useIndexFile").toBool());
    ui.cbAutoCommitDB->setChecked(AppSettings->getValue("autoCommitDB").toBool());
    ui.engineRestartCheck->setChecked(AppSettings->getValue("restartAnalysisOnMpv").toBool());
    AppSettings->endGroup();

    // Read Board settings
    AppSettings->beginGroup("/Board/");
    ui.guessMoveCheck->setChecked(AppSettings->getValue("guessMove").toBool());
    ui.guessNextMoveCheck->setChecked(AppSettings->getValue("guessNextMove").toBool());
    ui.boardMoatCheck->setChecked(AppSettings->getValue("showMoat").toBool());
    ui.boardTowerCheck->setChecked(AppSettings->getValue("showTower").toBool());
    ui.boardFrameCheck->setChecked(AppSettings->getValue("showFrame").toBool());
    ui.boardSNumbersCheck->setChecked(AppSettings->getValue("showSquareNumbers").toBool());
    ui.boardFrameSize->setValue(AppSettings->getValue("frameWidth").toInt());
    ui.hilightCurrentMove->setChecked(AppSettings->getValue("showCurrentMove").toBool());
    ui.hilightAllMoves->setChecked(AppSettings->getValue("showAllMoves").toBool());
    ui.animateMovesCheck->setChecked(AppSettings->getValue("animateMoves").toBool());
    ui.animateMovesSpeed->setValue(AppSettings->getValue("animateMovesSpeed").toDouble());
    ui.animateMovesLength->setValue(AppSettings->getValue("animateMovesLength").toDouble());
    ui.animateMovesSpeedVsLength->setValue(
        AppSettings->getValue("animateMovesSpeedVsLength").toDouble()
        * ui.animateMovesSpeedVsLength->maximum());
    ui.minWheelCount->setValue(AppSettings->getValue("minWheelCount").toInt());
    //ui.autoPlayInterval->setValue(AppSettings->getValue("AutoPlayerInterval").toInt());
    //ui.cbSaveAndContinue->setChecked(AppSettings->getValue("AutoSaveAndContinue").toBool());
    QString pieceTheme = AppSettings->getValue("pieceTheme").toString();
    ui.pieceEffect->setCurrentIndex(AppSettings->getValue("pieceEffect").toInt());
    QString boardTheme = AppSettings->getValue("boardTheme").toString();
    ui.boardColorsList->clear();
    restoreColorItem(ui.boardColorsList, tr("Light squares"), "lightColor");
    restoreColorItem(ui.boardColorsList, tr("Dark squares"), "darkColor");
    restoreColorItem(ui.boardColorsList, tr("Highlighted squares"), "highlightColor");
    restoreColorItem(ui.boardColorsList, tr("Frame"), "frameColor");
    restoreColorItem(ui.boardColorsList, tr("Current move"), "currentMoveColor");
    restoreColorItem(ui.boardColorsList, tr("Background1"), "backgroundColor");
    restoreColorItem(ui.boardColorsList, tr("Background2"), "backgroundColor2");
    AppSettings->endGroup();

	QString themeDir(AppSettings->dataPath() + "/themes");

    if (!QFile::exists(themeDir))
        themeDir = QString(":/themes");

	QStringList themes = QDir(themeDir).entryList(QStringList("*.png"));
	for (QStringList::Iterator it = themes.begin(); it != themes.end(); ++it) {
		(*it).truncate((*it).length() - 4);
		ui.pieceThemeCombo->addItem(*it);
	}

    QString boardDir(AppSettings->dataPath() + "/themes/boards");

    if (!QFile::exists(boardDir))
        boardDir = QString(":/themes/boards");

    themes = QDir(boardDir).entryList(QStringList("*.png"));
	QStringListIterator it(themes);
	while (it.hasNext()) {
		QString trim(it.next());
		ui.boardThemeCombo->addItem(trim.left(trim.length() - 4));
	}
	ui.boardThemeCombo->addItem(tr("[plain colors]"));

	selectInCombo(ui.pieceThemeCombo, pieceTheme);
	selectInCombo(ui.boardThemeCombo, boardTheme);

	// Read Engine settings
	engineList.restore();
	ui.engineList->clear();
	ui.engineList->insertItems(0, engineList.names());

	// Read Advanced settings
	ui.limitSpin->setValue(AppSettings->value("/General/EditLimit", 10).toInt());
    ui.spinBoxRecentFiles->setValue(AppSettings->value("/History/MaxEntries", 4).toInt());

#if QT_VERSION < 0x050000
    QString dataPath = QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation) + "/shatradata";
#else
    QString dataPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/shatradata";
#endif
    ui.defaultDataBasePath->setText(AppSettings->value("/General/DefaultDataPath", dataPath).toString());

    int k = AppSettings->getValue("/General/gameStartPosition").toInt();
    ui.cbMoveOnLoad->setCurrentIndex(k==1? 1 : (k==-1? 2 : 0));

    // Read Game List settings
    ui.gameTextFontSizeSpin->setValue(AppSettings->value("/GameText/FontSize", DEFAULT_FONTSIZE).toInt());
    ui.spinBoxListFontSize->setValue(AppSettings->value("/General/ListFontSize", DEFAULT_LISTFONTSIZE).toInt());
    ui.verticalTabs->setChecked(AppSettings->getValue("/MainWindow/VerticalTabs").toBool());

    // Play Game
    ui.cbSaveMoveTime->setChecked( AppSettings->getValue("/PlayGame/saveMoveTime").toBool() );

    // Time Control
    AppSettings->beginGroup("/TimeControl/");
    QString mode = AppSettings->getValue("mode").toString();
    if (mode == timeControlTypeName[TimeControl::T_Match])
        ui.cbMatchTime->setChecked(true);
    else
    if (mode == timeControlTypeName[TimeControl::T_Tournament])
        ui.cbTournament->setChecked(true);
    else
//    if (mode == timeControlTypeName[TimeControl::T_Limit])
        ui.cbLimit->setChecked(true);

    ui.comboTimeFormat->setCurrentIndex(TimeControl::formatFromName(AppSettings->getValue("format").toString()));
    ui.cbAllMoves1->setChecked(AppSettings->getValue("allMoves1").toBool());
    ui.cbAllMoves2->setChecked(AppSettings->getValue("allMoves2").toBool());
    ui.moves1->setValue(AppSettings->getValue("numMoves1").toInt());
    ui.moves2->setValue(AppSettings->getValue("numMoves2").toInt());
    setTime(*ui.time1, AppSettings->getValue("timeForMoves1").toInt());
    setTime(*ui.time2, AppSettings->getValue("timeForMoves2").toInt());
    setTime(*ui.time3, AppSettings->getValue("timeAdd").toInt());
    setTime(*ui.timeInc1, AppSettings->getValue("timeInc1").toInt());
    setTime(*ui.timeInc2, AppSettings->getValue("timeInc2").toInt());
    setTime(*ui.timeInc3, AppSettings->getValue("timeInc3").toInt());
    ui.cbTimeInc1->setChecked(AppSettings->getValue("doTimeInc1").toBool());
    ui.cbTimeInc2->setChecked(AppSettings->getValue("doTimeInc2").toBool());
    ui.cbTimeInc3->setChecked(AppSettings->getValue("doTimeInc3").toBool());
    ui.cbTime2->setChecked(AppSettings->getValue("doTime2").toBool());
    setTime(*ui.limitTime, AppSettings->getValue("timeLimit").toInt());
    ui.limitNodes->setValue(AppSettings->getValue("nodeLimit").toInt());
    ui.limitDepth->setValue(AppSettings->getValue("depthLimit").toInt());
    ui.cbLimitTime->setChecked(AppSettings->getValue("doTimeLimit").toBool());
    ui.cbLimitNodes->setChecked(AppSettings->getValue("doNodeLimit").toBool());
    ui.cbLimitDepth->setChecked(AppSettings->getValue("doDepthLimit").toBool());
    AppSettings->endGroup();

    slotTCEnable();
}

void PreferencesDialog::saveSettings()
{
    AppSettings->beginGroup("/General/");
    QString locale;
    int i = ui.languageCombo->currentIndex();
    if (i<ui.languageCombo->count())
        locale = ui.languageCombo->itemData(i).toString();
    else
        locale = AppSettings->defaultLocale();
    AppSettings->setValue("Language",QVariant(locale));
    AppSettings->setValue("Notation",QVariant(ui.notationAlgebraic->isChecked()));
    AppSettings->setValue("useIndexFile",QVariant(ui.useIndexFile->isChecked()));
    AppSettings->setValue("autoCommitDB",QVariant(ui.cbAutoCommitDB->isChecked()));
    AppSettings->setValue("restartAnalysisOnMpv",QVariant(ui.engineRestartCheck->isChecked()));
    AppSettings->endGroup();

    AppSettings->beginGroup("/Board/");
    AppSettings->setValue("guessMove", QVariant(ui.guessMoveCheck->isChecked()));
    AppSettings->setValue("guessNextMove", QVariant(ui.guessNextMoveCheck->isChecked()));
    AppSettings->setValue("showMoat", QVariant(ui.boardMoatCheck->isChecked()));
    AppSettings->setValue("showTower", QVariant(ui.boardTowerCheck->isChecked()));
    AppSettings->setValue("showFrame", QVariant(ui.boardFrameCheck->isChecked()));
    AppSettings->setValue("showSquareNumbers", QVariant(ui.boardSNumbersCheck->isChecked()));
    AppSettings->setValue("frameWidth", QVariant(ui.boardFrameSize->value()));
    AppSettings->setValue("showCurrentMove", QVariant(ui.hilightCurrentMove->isChecked()));
    AppSettings->setValue("showAllMoves", QVariant(ui.hilightAllMoves->isChecked()));
    AppSettings->setValue("animateMoves", QVariant(ui.animateMovesCheck->isChecked()));
    AppSettings->setValue("animateMovesSpeed", QVariant(ui.animateMovesSpeed->value()));
    AppSettings->setValue("animateMovesLength", QVariant(ui.animateMovesLength->value()));
    AppSettings->setValue("animateMovesSpeedVsLength", QVariant(
        (double)ui.animateMovesSpeedVsLength->value() / ui.animateMovesSpeedVsLength->maximum()));
    AppSettings->setValue("minWheelCount", ui.minWheelCount->value());
    //AppSettings->setValue("AutoPlayerInterval", ui.autoPlayInterval->value());
	AppSettings->setValue("pieceTheme", ui.pieceThemeCombo->currentText());
	AppSettings->setValue("pieceEffect", ui.pieceEffect->currentIndex());
    //AppSettings->setValue("AutoSaveAndContinue",QVariant(ui.cbSaveAndContinue->isChecked()));
    if (ui.boardThemeCombo->currentIndex() != ui.boardThemeCombo->count() - 1)
		AppSettings->setValue("boardTheme", ui.boardThemeCombo->currentText());
	else	AppSettings->setValue("boardTheme", QString());
	QStringList colorNames;
    colorNames << "lightColor" << "darkColor" << "highlightColor" << "frameColor" << "currentMoveColor"
               << "backgroundColor" << "backgroundColor2";
	saveColorList(ui.boardColorsList, colorNames);
    AppSettings->endGroup();

    // play game
    AppSettings->setValue("/PlayGame/saveMoveTime", ui.cbSaveMoveTime->isChecked());

    // time control
    AppSettings->beginGroup("/TimeControl/");

    if (ui.cbMatchTime->isChecked())
        AppSettings->setValue("mode", QString(timeControlTypeName[TimeControl::T_Match]));
    else
    if (ui.cbTournament->isChecked())
        AppSettings->setValue("mode", QString(timeControlTypeName[TimeControl::T_Tournament]));
    else
//    if (ui.cbLimit->isChecked())
        AppSettings->setValue("mode", QString(timeControlTypeName[TimeControl::T_Limit]));

    AppSettings->setValue("format", TimeControl::formatName((TimeControl::Format)ui.comboTimeFormat->currentIndex()));
    AppSettings->setValue("allMoves1", ui.cbAllMoves1->isChecked());
    AppSettings->setValue("allMoves2", ui.cbAllMoves2->isChecked());
    AppSettings->setValue("numMoves1", ui.moves1->value());
    AppSettings->setValue("numMoves2", ui.moves2->value());
    AppSettings->setValue("timeForMoves1", getMSecs(*ui.time1));
    AppSettings->setValue("timeForMoves2", getMSecs(*ui.time2));
    AppSettings->setValue("timeAdd", getMSecs(*ui.time3));
    AppSettings->setValue("timeInc1", getMSecs(*ui.timeInc1));
    AppSettings->setValue("timeInc2", getMSecs(*ui.timeInc2));
    AppSettings->setValue("timeInc3", getMSecs(*ui.timeInc3));
    AppSettings->setValue("doTimeInc1", ui.cbTimeInc1->isChecked());
    AppSettings->setValue("doTimeInc2", ui.cbTimeInc2->isChecked());
    AppSettings->setValue("doTimeInc3", ui.cbTimeInc3->isChecked());
    AppSettings->setValue("doTime2", ui.cbTime2->isChecked());
    AppSettings->setValue("timeLimit", getMSecs(*ui.limitTime));
    AppSettings->setValue("nodeLimit", ui.limitNodes->value());
    AppSettings->setValue("depthLimit", ui.limitDepth->value());
    AppSettings->setValue("doTimeLimit", ui.cbLimitTime->isChecked());
    AppSettings->setValue("doDepthLimit", ui.cbLimitDepth->isChecked());
    AppSettings->setValue("doNodeLimit", ui.cbLimitNodes->isChecked());
    AppSettings->endGroup();

	// Save engine settings
	updateEngineData(ui.engineList->currentIndex().row());  // Make sure current edits are saved
	engineList.save();

    AppSettings->setValue("/General/EditLimit", ui.limitSpin->value());
    AppSettings->setValue("/History/MaxEntries", ui.spinBoxRecentFiles->value());
    AppSettings->setValue("/General/DefaultDataPath", ui.defaultDataBasePath->text());
    AppSettings->setValue("/GameText/FontSize", ui.gameTextFontSizeSpin->value());
    AppSettings->setValue("/General/ListFontSize", ui.spinBoxListFontSize->value());
    AppSettings->setValue("/MainWindow/VerticalTabs", ui.verticalTabs->isChecked());
    AppSettings->setValue("/General/gameStartPosition",
                          ui.cbMoveOnLoad->currentIndex() == 1 ?
                              1 : (ui.cbMoveOnLoad->currentIndex() == 2 ? -1 : 0));
    QDir().mkpath(ui.defaultDataBasePath->text());
}

bool PreferencesDialog::selectInCombo(QComboBox* combo, const QString& text)
{
    for (int i = 0; i < combo->count(); ++i)
		if (combo->itemText(i) == text) {
			combo->setCurrentIndex(i);
			return true;
		}
	combo->setCurrentIndex(combo->count() - 1);
	return false;
}

void PreferencesDialog::restoreColorItem(ColorList* list, const QString& text, const QString& cfgname)
{
    QColor color = AppSettings->getValue(cfgname).value<QColor>();
	list->addItem(text, color);
}

void PreferencesDialog::saveColorList(ColorList* list, const QStringList& cfgnames)
{
    for (int i = 0; i < list->count(); ++i)
		AppSettings->setValue(cfgnames[i], list->color(i));
}

void PreferencesDialog::slotShowLanguageMessage()
{
    // XXX This text should be in the correct language ...
    QMessageBox::information(this, tr("Language change"),
                             tr("You need to restart Shatrabase, "
                                "to apply the language change."));
}
