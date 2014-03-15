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

#include "boardsetup.h"
#include "boardview.h"
#include "messagedialog.h"
#include "settings.h"
#include "boardsetuptoolbutton.h"

#include <QtGui>
#include <QPixmap>

BoardSetupDialog::BoardSetupDialog(QWidget* parent) : QDialog(parent), m_wheelCurrentDelta(0), m_selectedPiece(Empty)
{
	ui.setupUi(this);
	ui.boardView->configure();
    ui.boardView->setFlags(BoardView::F_IgnoreSideToMove | BoardView::F_AllowCopyPiece);
//    ui.boardView->showMoveIndicator(false);

    m_minDeltaWheel = AppSettings->getValue("/Board/minWheelCount").toInt();

	for (int piece = Empty; piece <= BlackShatra; piece++) {
        BoardSetupToolButton* button = new BoardSetupToolButton(this);
        button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        //button->setAlignment(Qt::AlignJustify|Qt::AlignVCenter);
        button->setMinimumSize(QSize(10,10));
        button->m_piece = (Piece)piece;
        if (piece == Empty)
        {
            button->m_pixmap = QPixmap(0,0);
            ui.buttonLayout->addWidget(button, 5, 0);
        }
		else
        {
            button->m_pixmap = ui.boardView->theme().piece(Piece(piece));
            ui.buttonLayout->addWidget(button, (piece - 1) % 5, piece >= BlackBatyr);
        }
        connect(button, SIGNAL(signalDragStarted(QWidget*,QMouseEvent*)), this, SLOT(startDrag(QWidget*,QMouseEvent*)));
        connect(button, SIGNAL(signalClicked(Piece)), this, SLOT(labelClicked(Piece)));
        connect(this, SIGNAL(signalClearBackground(Piece)),button, SLOT(slotClearBackground(Piece)));
	}

    emit signalClearBackground(Empty);

	connect(ui.okButton, SIGNAL(clicked()), SLOT(slotAccept()));
	connect(ui.cancelButton, SIGNAL(clicked()), SLOT(reject()));
	connect(ui.clearButton, SIGNAL(clicked()), SLOT(slotClear()));
	connect(ui.resetButton, SIGNAL(clicked()), SLOT(slotReset()));
    connect(ui.boardView, SIGNAL(clicked(Square, int, QPoint)), SLOT(slotSelected(Square, int)));
    connect(ui.boardView, SIGNAL(moveMade(Square, Square, int)), SLOT(slotMovePiece(Square, Square)));
    connect(ui.boardView, SIGNAL(copyPiece(Square, Square)), SLOT(slotCopyPiece(Square, Square)));
    connect(ui.boardView, SIGNAL(invalidMove(Square)), SLOT(slotInvalidMove(Square)));
	connect(ui.boardView, SIGNAL(wheelScrolled(int)), SLOT(slotChangePiece(int)));
    connect(ui.boardView, SIGNAL(pieceDropped(Square,Piece)), SLOT(slotDroppedPiece(Square, Piece)));
	connect(ui.toMoveButton, SIGNAL(clicked()), SLOT(slotToggleSide()));
	connect(ui.epCombo, SIGNAL(currentIndexChanged(int)), SLOT(slotEnPassantSquare()));
	connect(ui.moveSpin, SIGNAL(valueChanged(int)), SLOT(slotMoveNumber()));
	connect(ui.copyButton, SIGNAL(clicked()), SLOT(slotCopySPN()));
	connect(ui.pasteButton, SIGNAL(clicked()), SLOT(slotPasteSPN()));
}

BoardSetupDialog::~BoardSetupDialog()
{}

Board BoardSetupDialog::board() const
{
	Board b = ui.boardView->board();
	return b;
}

void BoardSetupDialog::setFlipped(bool flipped)
{
	ui.boardView->setFlipped(flipped);
}

void BoardSetupDialog::setBoard(const Board& b)
{
	ui.boardView->setBoard(b);
	ui.moveSpin->setValue(b.moveNumber());
    if (b.enPassantSquare() == NoSquare)
		ui.epCombo->setCurrentIndex(0);
	else if (b.toMove() == White && b.pieceAt(b.enPassantSquare() - 8) == BlackShatra &&
			b.pieceAt(b.enPassantSquare()) == Empty && b.pieceAt(b.enPassantSquare() + 8) == Empty)
		ui.epCombo->setCurrentIndex(b.enPassantSquare() % 8 + 1);
	else if (b.toMove() == Black && b.pieceAt(b.enPassantSquare() + 8) == WhiteShatra &&
			b.pieceAt(b.enPassantSquare()) == Empty && b.pieceAt(b.enPassantSquare() - 8) == Empty)
		ui.epCombo->setCurrentIndex(b.enPassantSquare() % 8 + 1);
	else ui.epCombo->setCurrentIndex(0);
	m_toMove = b.toMove();
	showSideToMove();
	setStatusMessage();
}

int BoardSetupDialog::exec()
{
	ui.boardView->configure();
	return QDialog::exec();
}

void BoardSetupDialog::slotReset()
{
	Board b;
	b.setStandardPosition();
	setBoard(b);
}

void BoardSetupDialog::slotAccept()
{
	// Need to make sure the board is updated with move number set by user
	Board b(ui.boardView->board());
	b.setMoveNumber(ui.moveSpin->value());
	ui.boardView->setBoard(b);

	QString reason = boardStatusMessage();
	if (reason.isEmpty())
		accept();
	else MessageDialog::error(tr("Current position is not valid.\n\n%1.").arg(reason));
}

void BoardSetupDialog::slotClear()
{
	Board b;
    b.setAt(temdekAt[White], WhiteBiy);
    b.setAt(temdekAt[Black], BlackBiy);
	setBoard(b);
}

void BoardSetupDialog::slotSelected(Square square, int button)
{
    Piece piece = (button & Qt::MidButton) ? Empty : m_selectedPiece;
    if (button & Qt::RightButton) {
        if (piece >= BlackBiy)
            piece = (Piece)(piece - (BlackBiy - WhiteBiy));
        else if (piece != Empty)
            piece = (Piece)(piece + (BlackBiy - WhiteBiy));
    }
    Board board = ui.boardView->board();
    if (board.pieceAt(square) == piece)
        piece = Empty;
    board.setAt(square, piece);
    setBoard(board);
}

void BoardSetupDialog::showSideToMove()
{
	QSize size = ui.toMoveButton->iconSize();
	QPixmap pixmap(size);
	QPainter painter(&pixmap);
	painter.fillRect(0, 0, size.width(), size.height(), m_toMove == White ? Qt::black : Qt::white);
	painter.fillRect(1, 1, size.width() - 2, size.height() - 2, m_toMove == White ? Qt::white : Qt::black);
	ui.toMoveButton->setIcon(QIcon(pixmap));
}

void BoardSetupDialog::slotToggleSide()
{
	m_toMove = oppositeColor(m_toMove);
	Board b = ui.boardView->board();
	b.setToMove(m_toMove);
	setBoard(b);
}

void BoardSetupDialog::slotChangePiece(int dir)
{
    int i = m_selectedPiece;
    i += (dir == BoardView::WheelUp) ? -1 : 1;
    if (i < 0) i = (int) BlackShatra;
    else if (i>BlackShatra) i = (int) Empty;
    m_selectedPiece = (Piece) i;
    emit signalClearBackground(m_selectedPiece);
}

void BoardSetupDialog::slotDroppedPiece(Square s, Piece p)
{
    Board b = ui.boardView->board();
    b.setAt(s, p);
    setBoard(b);
}

void BoardSetupDialog::slotMovePiece(Square from, Square to)
{
	Board b = ui.boardView->board();
	Piece p = b.pieceAt(from);
	b.removeFrom(from);
	b.setAt(to, p);
	setBoard(b);
}

void BoardSetupDialog::slotCopyPiece(Square from, Square to)
{
	Board b = ui.boardView->board();
	Piece p = b.pieceAt(from);
	b.setAt(to, p);
	setBoard(b);
}

void BoardSetupDialog::slotInvalidMove(Square from)
{
    Board b = ui.boardView->board();
    Piece p = b.pieceAt(from);
    if (pieceType(p) != Biy)
    {
        b.removeFrom(from);
        setBoard(b);
    }
}

void BoardSetupDialog::wheelEvent(QWheelEvent* e)
{
    m_wheelCurrentDelta += e->delta();
    if (abs(m_wheelCurrentDelta) > m_minDeltaWheel)
    {
        slotChangePiece(m_wheelCurrentDelta < 0 ? BoardView::WheelDown : BoardView::WheelUp);
        m_wheelCurrentDelta = 0;
    }

}

QString BoardSetupDialog::boardStatusMessage() const
{
	switch (ui.boardView->board().validate()) {
	case Valid:
		return QString();
	case NoWhiteBiy:
        return tr("No white biy");
	case NoBlackBiy:
        return tr("No black biy");
    case TooManyBiys:
        return tr("Too many biys");
	case InvalidEnPassant:
		return tr("En passant square is not correct");
	default:
		return tr("Unknown reason");
	}
}

void BoardSetupDialog::setStatusMessage()
{
	QString reason = boardStatusMessage();
	ui.okButton->setEnabled(reason.isEmpty());
    ui.copyButton->setEnabled(reason.isEmpty());
    if (reason.isEmpty())
		ui.spnLabel->setText(tr("SPN: %1").arg(ui.boardView->board().toSPN()));
	else
		ui.spnLabel->setText(tr("Illegal position: %1").arg(reason));
}

void BoardSetupDialog::slotCopySPN()
{
	QApplication::clipboard()->setText(ui.boardView->board().toSPN());
}

void BoardSetupDialog::slotPasteSPN()
{
    // Prepare Spn - clean up code like this:
    // [SPN "***"] to ***
    QString spn = QApplication::clipboard()->text().simplified();
    if (spn.contains("\""))
    {
        int n1 = spn.indexOf('"');
        int n2 = spn.lastIndexOf('"');
        if (n2>n1+1)
        {
            spn = spn.mid(n1+1,n2-n1-1);
        }
    }

    // Another go at Spns copied from Wikis: [SPN]***[/SPN] is reduced to ***
    spn.remove(QRegExp("\\[[^\\]]*\\]"));

    // Now parse the hopefully naked Spn
    Board b;
    if (!b.fromSPN(spn))
    {
        QString msg = spn.length() ?
                    tr("Text in clipboard does not represent valid SPN:<br><i>%1</i>").arg(spn) :
                    tr("There is no text in clipboard.");
        MessageDialog::warning(msg);
    }
    else
    {
        setBoard(b);
    }
}

void BoardSetupDialog::slotEnPassantSquare()
{
	Board b(board());
	if (ui.epCombo->currentIndex() == 0)
		b.clearEnPassantSquare();
	else {
		int shift = b.toMove() == White ? 39 : 15;
		b.setEnPassantSquare(shift + ui.epCombo->currentIndex());
	}
	setBoard(b);
}

void BoardSetupDialog::slotMoveNumber()
{
	Board b(board());
	b.setMoveNumber(ui.moveSpin->value());
	setBoard(b);
}

void BoardSetupDialog::startDrag(QWidget* w, QMouseEvent* event)
{
    BoardSetupToolButton *child = qobject_cast<BoardSetupToolButton*>(w);
    if (!child)
        return;
    Piece p = child->m_piece;

    QPoint hotSpot = event->pos();

    BoardViewMimeData *mimeData = new BoardViewMimeData;
    mimeData->m_piece = p;

    QPixmap pixmap = *child->pixmap();

    QDrag* pDrag = new QDrag(this);
    pDrag->setMimeData(mimeData);
    pDrag->setPixmap(pixmap);
    pDrag->setHotSpot(hotSpot);

    pDrag->exec(Qt::CopyAction | Qt::MoveAction, Qt::CopyAction);
}

void BoardSetupDialog::labelClicked(Piece p)
{
    m_selectedPiece = p;
    emit signalClearBackground(m_selectedPiece);
}
