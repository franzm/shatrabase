/***************************************************************************
 *                           Shatrabase                                    *
 *   (C) 2012-2014 AFKM <francis@afkm.com>                                 *
 *                   derived from ChessX with thanks                       *
 *   (C) 2008-2010 Michal Rudolf <mrudolf@kdewebdev.org>                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "settings.h"
#include "analysis.h"
#include "board.h"
#include "analysiswidget.h"
#include "enginelist.h"
#include "enginedebugwidget.h"
#include "messagedialog.h"
#include "metaanalysis.h"

AnalysisWidget::AnalysisWidget(int num, EngineDebugWidget * debug)
        : QWidget(0),
          m_engine(0),
          m_metaengine(0),
          m_ignore(false),
          m_windowNumber(num),
          m_engineDebug(debug)
{
    ui.setupUi(this);
    connect(ui.engineList, SIGNAL(activated(int)), SLOT(toggleAnalysis()));
    connect(ui.analyzeButton, SIGNAL(clicked(bool)), SLOT(toggleAnalysis()));
    connect(ui.variationText, SIGNAL(anchorClicked(QUrl)),
              SLOT(slotLinkClicked(QUrl)));
    connect(ui.vpcount, SIGNAL(valueChanged(int)), SLOT(slotMpvChanged(int)));
    ui.analyzeButton->setFixedHeight(ui.engineList->sizeHint().height());

    // XXX
    ui.cbMeta->setVisible(false);

    //  m_tablebase = new Shredder;
//  connect(m_tablebase, SIGNAL(bestMove(Move, int)), this, SLOT(showTablebaseMove(Move, int)));
}

AnalysisWidget::~AnalysisWidget()
{
    stopEngine();
//    delete m_tablebase;
}

void AnalysisWidget::setMetaMode(bool m)
{
    if ((m && m_engine)
        || (!m && m_metaengine))
        stopEngine();
}

void AnalysisWidget::startEngine()
{       
    int index = ui.engineList->currentIndex();
    stopEngine();
    if (index != -1)
    {
        if (parentWidget() && !parentWidget()->isVisible())
            parentWidget()->show();
        ui.variationText->clear();

        if (!ui.cbMeta->isChecked())
        {
            m_engine = Engine::newEngine(index);
            m_engine->setParent(this);
            ui.vpcount->setEnabled(m_engine->providesMvp());
            ui.label->setEnabled(m_engine->providesMvp());
            if (!m_engine->providesMvp())
            {
                ui.vpcount->setValue(1);
            }

            connect(m_engine, SIGNAL(activated()), SLOT(engineActivated()));
            connect(m_engine, SIGNAL(error(QProcess::ProcessError)), SLOT(engineError(QProcess::ProcessError)));
            connect(m_engine, SIGNAL(deactivated()), SLOT(engineDeactivated()));
            connect(m_engine, SIGNAL(analysisUpdated(const Analysis&)),
                      SLOT(showAnalysis(const Analysis&)));
            if (m_engineDebug)
                connect(m_engine, SIGNAL(engineDebug(Engine*,Engine::DebugType,QString)),
                    m_engineDebug, SLOT(slotEngineDebug(Engine*,Engine::DebugType,QString)));

            m_engine->activate();
        }
        else // meta-analysis
        {
            m_metaengine = new MetaAnalysis(this);
            ui.vpcount->setEnabled(true);
            ui.label->setEnabled(true);

            connect(m_metaengine, SIGNAL(activated()), SLOT(engineActivated()));
            connect(m_metaengine, SIGNAL(deactivated()), SLOT(engineDeactivated()));
            connect(m_metaengine, SIGNAL(analysisUpdated(const Analysis&)),
                                    SLOT(showAnalysis(const Analysis&)));
            if (m_engineDebug)
                connect(m_metaengine, SIGNAL(engineDebug(Engine*,Engine::DebugType,QString)),
                    m_engineDebug, SLOT(slotEngineDebug(Engine*,Engine::DebugType,QString)));

            m_metaengine->activate(index);
        }

        // store current used engine
        QString key = QString("/")+objectName()+"/Engine";
        AppSettings->setValue(key, ui.engineList->itemText(index));
    }
}

void AnalysisWidget::stopEngine()
{
    engineDeactivated();
    if (m_engine)
    {
        m_engine->deactivate();
        m_engine->deleteLater();
        m_engine = 0;
    }
    if (m_metaengine)
    {
        m_metaengine->stopAnalysis();
        m_metaengine->deleteLater();
        m_metaengine = 0;
    }
}

void AnalysisWidget::slotVisibilityChanged(bool visible)
{
    if (isEngineRunning() && !visible && !parentWidget()->isVisible())
        stopEngine();
}

bool AnalysisWidget::isEngineRunning() const
{
    return (m_engine || m_metaengine) && ui.analyzeButton->isChecked();
}

bool AnalysisWidget::isFreeze() const
{
    return ui.freezeButton->isChecked();
}

void AnalysisWidget::engineActivated()
{
    //qDebug() << "AnalysisWidget::engineActivated()";

    ui.analyzeButton->setChecked(true);
    ui.analyzeButton->setText(tr("Stop"));
    m_analyses.clear();
    if (m_engine)
        m_engine->startAnalysis(m_board, ui.vpcount->value());

    if (m_metaengine)
        m_metaengine->startAnalysis(m_board, ui.vpcount->value());
}

void AnalysisWidget::engineError(QProcess::ProcessError e)
{
    MessageDialog::warning(tr("There was an error running engine <b>%2</b>.\n%1")
                              .arg(Engine::processErrorText(e))
                              .arg(ui.engineList->currentText()));
    stopEngine();
}

void AnalysisWidget::engineDeactivated()
{
    ui.analyzeButton->setChecked(false);
    ui.analyzeButton->setText(tr("Analyze"));
}

void AnalysisWidget::toggleAnalysis()
{
    if (!isAnalysisEnabled())
        stopEngine();
    else startEngine();
}

void AnalysisWidget::slotReconfigure()
{
    QString oldEngineName = ui.engineList->currentText();
    if (oldEngineName.isEmpty())
    {
        QString key = QString("/")+objectName()+"/Engine";
        oldEngineName = AppSettings->value(key).toString();
    }

    EngineList enginesList;
    enginesList.restore();
    QStringList names = enginesList.names();
    ui.engineList->clear();
    ui.engineList->insertItems(0,   names);
    int index = names.indexOf(oldEngineName);
    if (index != -1)
        ui.engineList->setCurrentIndex(index);
    else
    {
        ui.engineList->setCurrentIndex(0);
        stopEngine();
    }

    int fontSize = AppSettings->getValue("/General/ListFontSize").toInt();
    QFont f = ui.variationText->font();
    f.setPointSize(fontSize);
    setFont(f);
    ui.variationText->setFont(f);

    int mpv = AppSettings->getValue(
                m_windowNumber == 0 ? "/General/AnalysisLines1" : "/General/AnalysisLines2"
                                 ).toInt();
    ui.vpcount->setValue(mpv);

    ui.cbSendBestMove->setVisible(AppSettings->getValue("/Board/guessMove").toBool());
}

void AnalysisWidget::showAnalysis(const Analysis& analysis)
{
    int mpv = analysis.mpv() - 1;
    if (mpv < 0 || mpv > m_analyses.count() || mpv >= ui.vpcount->value())
        return;
    else
    if (mpv == m_analyses.count())
        m_analyses.append(analysis);
    else
        m_analyses[mpv] = analysis;

    updateAnalysis();
}

void AnalysisWidget::setPosition(const Board& board)
{
    if (ui.freezeButton->isChecked())
        return;

    if (m_board != board)
    {
        m_ignore = false;

        m_board = board;
        m_analyses.clear();
//      m_tablebase->abortLookup();
//      m_tablebaseEvaluation.clear();

        updateAnalysis();

        if (board.isEnd())
        {
            m_ignore = true;
            if (m_engine)
                m_engine->stopAnalysis();
            if (m_metaengine)
                m_metaengine->stopAnalysis();
            return;
        }

        if (m_engine && m_engine->isActive())
            m_engine->startAnalysis(m_board, ui.vpcount->value());

        if (m_metaengine)
            m_metaengine->startAnalysis(m_board, ui.vpcount->value());
    }
}

void AnalysisWidget::slotLinkClicked(const QUrl& url)
{
    int mpv = url.toString().toInt() - 1;
    if (mpv >= 0 && mpv < m_analyses.count())
        emit addVariation(m_analyses[mpv]);
//    else if (mpv == -1)
//        emit addVariation(m_tablebaseEvaluation);
    else
    {
        mpv = (-mpv) - 2;
        if (mpv < m_analyses.count())
        {
            emit addVariation(m_analyses[mpv].variation().at(0).toAlgebraic());
        }
    }
}

void AnalysisWidget::slotMpvChanged(int mpv)
{
    if (isEngineRunning())
    {
        while (m_analyses.count() > mpv)
            m_analyses.removeLast();
        if (!m_ignore)
        {
            if (m_engine)
                m_engine->setMpv(mpv);
        }
    }

    AppSettings->setValue(
                m_windowNumber == 0 ? "/General/AnalysisLines1" : "/General/AnalysisLines2"
                    , mpv);
}



bool AnalysisWidget::isAnalysisEnabled() const
{
    if (!parentWidget())
        return false;
    if (!parentWidget()->isVisible() || !ui.analyzeButton->isChecked())
        return false;
    return true;
}
/*
void AnalysisWidget::showTablebaseMove(Move move, int score)
{
    QString result;
    if (score == 0)
        result = tr("Draw");
    else if ((score < 0) == (m_board.toMove() == Black))
        result = tr("White wins in %n moves", "", qAbs(score));
    else
        result = tr("Black wins in %n moves", "", qAbs(score));

    move = m_board.prepareMove(move.from(),move.to());
    m_tablebaseEvaluation = QString("%1 - %2")
                .arg(m_board.moveToFullLann(move)).arg(result);
    updateAnalysis();
}
*/
void AnalysisWidget::updateAnalysis()
{
    //qDebug() << "analysisupdate";
    if (m_ignore)
        return;

    Move bestMove;
    int  bestScore = -1;
    bool hasminnps=false;
    quint64 minnps = 0, maxnps = 0,
            nodes = 0;
    int time = 1;
    QString text;
    foreach (Analysis a, m_analyses)
    {
        if ((bestScore < 0 || a.score() > bestScore)
                && !a.variation().empty())
        {
            bestMove = a.variation()[0];
            bestScore = a.score();
        }

        nodes = std::max(nodes, a.nodes());
        time = std::max(time, a.time());
        maxnps = std::max(maxnps, a.nodesPerSecond());
        if (!hasminnps || minnps > a.nodesPerSecond())
            minnps = a.nodesPerSecond();

        text.append(a.toString(m_board) + "<br>");
    }
    //int avnps = nodes / time * 1000;
    /*
    if (!m_tablebaseEvaluation.isEmpty())
    {
        text.append(tr("<a href=\"0\">[+]</a> <b>Tablebase:</b> ") + m_tablebaseEvaluation);
    }
    */

    ui.variationText->setText(text);
    ui.labelNodes->setText(tr("nodes") + ": " + QLocale::system().toString(nodes) );
    //: nodes per second:
    ui.labelNps->setText(tr("nps") + ": "
                            + (minnps != maxnps ?
                                    QLocale::system().toString(minnps)
                                    + "-" + QLocale::system().toString(maxnps)
                                :   QLocale::system().toString(minnps))
    //: 'average' nodes per second
    //                        + " (" + tr("avg. ") + QLocale::system().toString(avnps) + ")"
                         );

    if (ui.cbSendBestMove->isVisible()
            && ui.cbSendBestMove->isChecked())
        emit this->bestMove(bestMove);

}

Analysis AnalysisWidget::getMainLine() const
{
    Analysis a;
    if (!m_analyses.isEmpty())
    {
        a = m_analyses.first();
    }
    return a;
}
