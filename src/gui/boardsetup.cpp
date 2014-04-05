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
#include <QMenu>

BoardSetupDialog::BoardSetupDialog(QWidget* parent)
    : QDialog(parent), m_wheelCurrentDelta(0), m_selectedPiece(Empty)
{
	ui.setupUi(this);
    m_boardView = new BoardView(ui.boardTab);//boardFrame);
    m_boardView->configure(); // load piece data
    ui.boardLayout->addWidget(m_boardView);
    m_boardView->setFlags(BoardView::F_AllowAllMoves | BoardView::F_AllowCopyPiece |
                          BoardView::F_HideMoveHelp | BoardView::F_NoExecuteMoves);

    m_minDeltaWheel = AppSettings->getValue("/Board/minWheelCount").toInt();

    for (int piece = WhiteBatyr; piece <= BlackShatra; piece++)
    {
        BoardSetupToolButton* button = new BoardSetupToolButton(this);
        button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        //button->setAlignment(Qt::AlignJustify|Qt::AlignVCenter);
        button->setMinimumSize(QSize(50,50));
        button->m_piece = (Piece)piece;

        button->m_pixmap = m_boardView->theme().piece(Piece(piece));
        ui.buttonLayout->addWidget(button, (piece - 1) % 5, piece >= BlackBatyr);

        connect(button, SIGNAL(signalDragStarted(QWidget*,QMouseEvent*)),
                                    SLOT(startDrag(QWidget*,QMouseEvent*)));
        connect(button, SIGNAL(signalClicked(Piece)), this, SLOT(labelClicked(Piece)));
        connect(this, SIGNAL(signalClearBackground(Piece)),button, SLOT(slotClearBackground(Piece)));
	}

    // popup menu
    m_popmenu = new QMenu(this);

    pa_temdek = new QAction(/* name will be set later */ m_popmenu);
    connect(pa_temdek, SIGNAL(triggered()), SLOT(slotSquareTemdek()));
    m_popmenu->addAction(pa_temdek);
    pa_defunkt = new QAction(tr("Set defunkt"), m_popmenu);
    connect(pa_defunkt, SIGNAL(triggered()), SLOT(slotSquareDefunkt()));
    m_popmenu->addAction(pa_defunkt);
    pa_enpassant = new QAction(/* name will be set later */ m_popmenu);
    connect(pa_enpassant, SIGNAL(triggered()), SLOT(slotSquareDefunkt()));
    m_popmenu->addAction(pa_enpassant);
    pa_urgent = new QAction(/* name will be set later */ m_popmenu);
    connect(pa_urgent, SIGNAL(triggered()), SLOT(slotSquareUrgent()));
    m_popmenu->addAction(pa_urgent);

    emit signalClearBackground(Empty);

	connect(ui.okButton, SIGNAL(clicked()), SLOT(slotAccept()));
	connect(ui.cancelButton, SIGNAL(clicked()), SLOT(reject()));
	connect(ui.clearButton, SIGNAL(clicked()), SLOT(slotClear()));
	connect(ui.resetButton, SIGNAL(clicked()), SLOT(slotReset()));
    connect(m_boardView, SIGNAL(clicked(Square, int, QPoint)), SLOT(slotSelected(Square, int)));
    connect(m_boardView, SIGNAL(moveMade(Square, Square, int)), SLOT(slotMovePiece(Square, Square)));
    connect(m_boardView, SIGNAL(copyPiece(Square, Square)), SLOT(slotCopyPiece(Square, Square)));
    connect(m_boardView, SIGNAL(invalidMove(Square)), SLOT(slotInvalidMove(Square)));
    connect(m_boardView, SIGNAL(wheelScrolled(int)), SLOT(slotChangePiece(int)));
    connect(m_boardView, SIGNAL(pieceDropped(Square,Piece)), SLOT(slotDroppedPiece(Square, Piece)));
	connect(ui.toMoveButton, SIGNAL(clicked()), SLOT(slotToggleSide()));
//	connect(ui.epCombo, SIGNAL(currentIndexChanged(int)), SLOT(slotEnPassantSquare()));
	connect(ui.moveSpin, SIGNAL(valueChanged(int)), SLOT(slotMoveNumber()));
	connect(ui.copyButton, SIGNAL(clicked()), SLOT(slotCopySPN()));
	connect(ui.pasteButton, SIGNAL(clicked()), SLOT(slotPasteSPN()));
}

BoardSetupDialog::~BoardSetupDialog()
{}

Board BoardSetupDialog::board() const
{
    return m_board;
}

void BoardSetupDialog::setFlipped(bool flipped)
{
    m_boardView->setFlipped(flipped);
}

void BoardSetupDialog::setBoard(const Board& b)
{
    m_board = b;
    m_boardView->setBoard(b);
	ui.moveSpin->setValue(b.moveNumber());
    /*
    if (b.enPassantSquare() == NoSquare)
		ui.epCombo->setCurrentIndex(0);
    else
        ui.epCombo->setCurrentIndex();
       if (b.toMove() == White && b.pieceAt(b.enPassantSquare() - 8) == BlackShatra &&
			b.pieceAt(b.enPassantSquare()) == Empty && b.pieceAt(b.enPassantSquare() + 8) == Empty)
		ui.epCombo->setCurrentIndex(b.enPassantSquare() % 8 + 1);
	else if (b.toMove() == Black && b.pieceAt(b.enPassantSquare() + 8) == WhiteShatra &&
			b.pieceAt(b.enPassantSquare()) == Empty && b.pieceAt(b.enPassantSquare() - 8) == Empty)
		ui.epCombo->setCurrentIndex(b.enPassantSquare() % 8 + 1);
	else ui.epCombo->setCurrentIndex(0);
    */
	m_toMove = b.toMove();
	showSideToMove();
	setStatusMessage();
}

int BoardSetupDialog::exec()
{
    m_boardView->configure();
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
    m_board.setMoveNumber(ui.moveSpin->value());
    m_boardView->setBoard(m_board);

	QString reason = boardStatusMessage();
	if (reason.isEmpty())
		accept();
	else MessageDialog::error(tr("Current position is not valid.\n\n%1.").arg(reason));
}

void BoardSetupDialog::slotClear()
{
	Board b;
//    b.setAt(gateAt[White], WhiteBiy);
//    b.setAt(gateAt[Black], BlackBiy);
    b.fillOffboard();
	setBoard(b);
}

void BoardSetupDialog::slotSelected(Square square, int button)
{
    // popup
    if (button & Qt::LeftButton)
    {
        openSquarePopup(square);
    }
    // flip color
    if (button & Qt::RightButton)
    {
        Piece piece = m_board.pieceAt(square);
        if (piece == Empty || piece == InvalidPiece)
            return;

        if (piece >= BlackBatyr && piece <= BlackShatra)
            piece = (Piece)(piece - (BlackBatyr - WhiteBatyr));
        else if (piece >= WhiteBatyr && piece <= WhiteShatra)
            piece = (Piece)(piece + (BlackBatyr - WhiteBatyr));
        else
            return;

        Board b(m_board);
        b.removeFrom(square);
        b.setAt(square, piece);
        if (b.pieceAt(square) != piece)
            return;
        setBoard(b);
    }
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
    m_board.setToMove(m_toMove);
    setBoard(m_board);
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
    m_board.setAt(s, p);
    setBoard(m_board);
}

void BoardSetupDialog::slotMovePiece(Square from, Square to)
{
    Piece p = m_board.pieceAt(from);
    m_board.removeFrom(from);
    if (m_board.pieceAt(to) != Empty)
        m_board.removeFrom(to);
    m_board.setAt(to, p);
    setBoard(m_board);
}

void BoardSetupDialog::slotCopyPiece(Square from, Square to)
{
    Piece p = m_board.pieceAt(from);
    if (m_board.pieceAt(to) != Empty)
        m_board.removeFrom(to);
    m_board.setAt(to, p);
    setBoard(m_board);
}

void BoardSetupDialog::slotInvalidMove(Square from)
{
    Piece p = m_board.pieceAt(from);
    if (pieceType(p) != Biy)
    {
        m_board.removeFrom(from);
        setBoard(m_board);
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
    switch (m_board.validate())
    {
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
        return tr("Incomplete");
	}
}

void BoardSetupDialog::setStatusMessage()
{
	QString reason = boardStatusMessage();
	ui.okButton->setEnabled(reason.isEmpty());
    ui.copyButton->setEnabled(reason.isEmpty());
    if (reason.isEmpty())
        ui.spnLabel->setText(tr("SPN: %1").arg(m_board.toSPN()));
	else
		ui.spnLabel->setText(tr("Illegal position: %1").arg(reason));
}

void BoardSetupDialog::slotCopySPN()
{
    QApplication::clipboard()->setText(m_board.toSPN());
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
/*
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
*/
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

// -------------- popup --------------

void BoardSetupDialog::openSquarePopup(Square s)
{
    if (s == InvalidSquare)
        return;

    m_popsquare = s;
    Piece piece = m_board.pieceAt(s);

    bool ispiece = !(piece == Empty || piece == InvalidPiece)
                     && piece < WasBatyr;

    pa_temdek->setEnabled(s<=10 || s>=53);
    pa_temdek->setText( m_board.temdekOn(s>31)?
                            tr("Open temdek") : tr("Close temdek") );
    pa_enpassant->setEnabled((s>=18 && s<=24) || (s>=39 && s<=45));
    pa_enpassant->setText(m_board.enPassantSquare() == s?
                    tr("Clear en passant square") : tr("Set en passant square"));
    pa_urgent->setEnabled(ispiece && isInHomeFort(s, s<=31? White : Black));
    pa_urgent->setText(m_board.isUrgent(s)? tr("Clear urgent") : tr("Set urgent"));

    pa_defunkt->setEnabled(ispiece && piece != WhiteBiy && piece != BlackBiy);

    m_popmenu->exec(QCursor::pos());
}

void BoardSetupDialog::slotSquareDefunkt()
{
    int p = m_board.pieceAt(m_popsquare);
    if (p >= WhiteBatyr && p<=WhiteShatra)
        p += (WasBatyr - WhiteBatyr);
    else
    if (p >= BlackBatyr && p<=BlackShatra)
        p += (WasBatyr - BlackBatyr);
    else return;

    m_board.setAt(m_popsquare, (Piece)p);
    setBoard(m_board);
}

void BoardSetupDialog::slotSquareTemdek()
{
    QString spn = m_board.toSPN();

    int stm = m_popsquare > 31;

    if (m_board.temdekOn(stm))
    {
        if (m_board.temdekOn(!stm))
        {
            // clear one of T or t
            spn.replace(stm? "t" : "T", "");
        }
        else
        {
            // remove temdek completely
            spn.replace(stm? "t" : "T", "-");
        }
    }
    else
    {
        // insert
        if (stm == White)
        {
            // T before t
            if (m_board.temdekOn(Black))
            {
                const int i = spn.indexOf("t");
                if (i<0) return;
                spn.insert(i, "T");
            }
            else
            // T after w/b
            {
                const int i = spn.indexOf('-');
                if (i<0) return;
                spn.replace(i, 1, "T");
            }
        }
        else
        {
            // t after T
            if (m_board.temdekOn(White))
            {
                const int i = spn.indexOf("T");
                if (i<0) return;
                spn.insert(i+1, "t");
            }
            else
            // t replaces - after w/b
            {
                const int i = spn.indexOf('-');
                if (i<0) return;
                spn.replace(i, 1, "t");
            }
        }
    }
    //qDebug() << spn;
    Board b;
    if (b.fromSPN(spn))
        setBoard(b);
    else
        qDebug() << "temdek failed";
    //SQSSRSBRB/K/SSSSSSS/7/7/7/7/sssssss/k/brbsrssqs w Tt - - - 1
    //SQSSRSBRB/K/SSSSSSS/7/7/7/7/sssssss/k/brbsrssqs w - - - 1
}

void BoardSetupDialog::slotSquareEnPassant()
{
    Board b(m_board);
    if (b.enPassantSquare() == m_popsquare)
        b.setEnPassantSquare(NoSquare);
    else
        b.setEnPassantSquare(m_popsquare);

    if (b.fromSPN(b.toSPN()))
        setBoard(b);
    else
        qDebug() << "en passant failed";
}


void BoardSetupDialog::slotSquareUrgent()
{
    Board b(m_board);
    if (b.isUrgent(m_popsquare))
        b.clearUrgentAt(NB[m_popsquare]);
    else
        b.setUrgentAt(NB[m_popsquare]);

    if (b.fromSPN(b.toSPN()))
        setBoard(b);
    else
        qDebug() << "urgent failed";
}
