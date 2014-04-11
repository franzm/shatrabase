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

#include "boardview.h"
#include "settings.h"
#include "movelist.h"
#include "boardpainter.h"

#include <QApplication>
#include <QSizePolicy>
#include <QLayout>


BoardView::BoardView(QWidget* parent, int flags)
  : QWidget(parent),
    m_parent(parent),
    m_view(0),
    m_flags(flags),
    m_isExternal(false),
    m_showCurrentMove(true),
    m_showAllMoves(true),
    m_guessMove (true),
    m_guessNextMove (true),
    m_selectedSquare (InvalidSquare),
    m_hoverSquare (InvalidSquare),
    m_bestMoveFrom  (InvalidSquare),
    m_bestMoveTo    (InvalidSquare),
    m_goal_index    (0),
    //m_own_from      (0),
    //m_own_to        (0),
    m_dragged       (InvalidPiece),
    m_dragStartSquare(InvalidSquare),
    m_lastDropped   (0),
    //m_clickUsed(false),
    // m_wheelCurrentDelta(0),
    //m_minDeltaWheel(0),
    //m_moveListCurrent(0)
    m_messageSend   (false)
{
    // that's how it's meant
    Q_ASSERT(m_parent);

    setMouseTracking(true);
    setAcceptDrops(true);

    //installEventFilter(this); XXX currently not used
	m_board.setStandardPosition();

    // to place the BoardPainter in
    m_layout = new QVBoxLayout(this);
    m_layout->setContentsMargins(0,0,0,0);
}

BoardView::~BoardView()
{

}

void BoardView::configure()
{
    AppSettings->beginGroup("/Board/");
    m_showCurrentMove = AppSettings->getValue("showCurrentMove").toBool();
    m_showAllMoves = AppSettings->getValue("showAllMoves").toBool();
    m_guessMove = AppSettings->getValue("guessMove").toBool();
    m_guessNextMove = AppSettings->getValue("guessNextMove").toBool();
    //m_minDeltaWheel = AppSettings->getValue("minWheelCount").toInt();
    bool flipped = AppSettings->getValue("flipped").toBool();
    AppSettings->endGroup();

    m_theme.configure();
    m_theme.setSize(QSize(256,256));

    //selectSquare();

    // recreate BoardPainter
    if (m_view)
    {
        m_layout->removeWidget(m_view);
        m_view->deleteLater();
    }

    m_view = new BoardPainter(&m_theme, this);
    m_view->setBoard(m_board);
    connect(m_view, SIGNAL(displayMessage(QString)), SLOT(slotDisplayMessage(QString)));
    connect(m_view, SIGNAL(animationFinished()), SLOT(slotAnimationFinished()));

    setFlipped(flipped);

    m_layout->addWidget(m_view);

    update();
}

void BoardView::displayMessage(const QString& msg)
{
    m_messageSend = true;
    signalDisplayMessage(msg);
}

QString BoardView::squareToString_(Square s) const
{
    QString r = QString("square %1 ").arg(s);
    switch (m_board.pieceAt(s))
    {
        case WhiteBatyr:  r += tr("white batyr"); break;
        case WhiteTura:   r += tr("white tura"); break;
        case WhiteYalkyn: r += tr("white yalkyn"); break;
        case WhiteBiy:    r += tr("white biy"); break;
        case WhiteShatra: r += tr("white shatra"); break;

        case BlackBatyr:  r += tr("black batyr"); break;
        case BlackTura:   r += tr("black tura"); break;
        case BlackYalkyn: r += tr("black yalkyn"); break;
        case BlackBiy:    r += tr("black biy"); break;
        case BlackShatra: r += tr("black shatra"); break;

        case WasBatyr:    r += tr("captured batyr"); break;
        case WasTura:     r += tr("captured tura"); break;
        case WasYalkyn:   r += tr("captured yalkyn"); break;
        case WasShatra:   r += tr("captured shatra"); break;

        default: break;
    }

    return r;
}

void BoardView::closeEvent(QCloseEvent * e)
{
    // a close on external board will
    // just get it back into parent
    if (m_isExternal)
    {
        e->ignore();
        setExternal(false);
        emit externalClosed();
    }
    else
        QWidget::closeEvent(e);
}

void BoardView::setExternal(bool e)
{
    if (m_isExternal)
    {
        setParent(m_parent);
    }
    else
    {
        setParent(0);
        show();
    }
    m_isExternal = e;
}

/*bool BoardView::eventFilter(QObject *obj, QEvent *ev)
{
	return QWidget::eventFilter(obj, ev);
}*/

void BoardView::setFlags(int flags)
{
    m_flags = flags;
}

/*
void BoardView::setBoard(const Board& value,int from, int to)
{
    //qDebug() << "setBoard(from=" << from << ", to=" << to << ")"
    //            << "\n" << value.toSPN();

    // reset gui flags
    m_selectedSquare
        = m_dragStartSquare
        = m_bestMoveFrom
        = m_bestMoveTo
            = InvalidSquare;
    m_dragged = InvalidPiece;

    //m_bestMoveFrom = 14; // debug test
    //m_bestMoveTo = 28;

    // copy position
	m_board = value;

    // m_board.debugDump();

    // get all possible moves
    m_moves.clear();
    m_board.getMoveSquares(m_moves);

    // trigger own animation from last user action
    if (from == InvalidSquare &&
        m_own_from != InvalidSquare && m_own_to != InvalidSquare)
    {
        from = m_own_from;
        to = m_own_to;
        m_own_from = m_own_to = InvalidSquare;
    }

    // update painter
    if (m_view)
        m_view->setBoard(value,NULL);

    // XXX need more than that to show all current moves
    // e.g. on mouse-click into game-list and such
    selectSquare_(from);

	update();
}
*/

void BoardView::setBoard(const Board& value, const Move& move)
{
    // reset gui flags
    m_selectedSquare
        = m_dragStartSquare
        = m_bestMoveFrom
        = m_bestMoveTo
            = InvalidSquare;
    m_dragged = InvalidPiece;

    // copy position
    m_board = value;
    m_move = move;

    // get all possible moves
    m_moves.clear();
    m_board.getMoveSquares(m_moves);

    // update painter
    if (m_view)
    {
        if (m_move.isLegal())
        {
            m_view->setBoard(value,&m_move, m_lastDropped);
            m_lastDropped = 0;
        }
        else
            m_view->setBoard(value,NULL);
    }
    selectSquare_(m_move.from());

    update();
}


Board BoardView::board() const
{
    return m_board;
}

const BoardTheme& BoardView::theme() const
{
    return m_theme;
}


void BoardView::setBestMove(int from, int to)
{
    if (from == InvalidSquare || to == InvalidSquare)
        return;

    m_bestMoveFrom = from;
    m_bestMoveTo = to;
    /*
    // find index of the supplied move in own movelist
    for (size_t i=0; i<m_moves.size(); ++i)
        if (m_moves[i].from == from &&
                m_moves[i].to == to)
    {
        // set m_goal_index (little hacky XXX)
        m_goal_index = 0;
        showGoals_(from, i);
        break;
    }*/
}



#if (0)
void BoardView::drawSquares(QPaintEvent* event)
{
    QPainter p(this); int x, y;

    for (Square sq = fsq; sq <= lsq; sq++)
    {
        QRect r = squareRect(sq);
        if (!event->region().intersects(r)) continue;

        x = gBoard[sq][0]; y = gBoard[sq][1];
        QPoint pos(r.x(), r.y());
        p.drawPixmap(pos, m_theme.square((x + y) % 2));

        if ((sq == gateAt[Black] && m_board.temdekOn(Black)) ||
            (sq == gateAt[White] && m_board.temdekOn(White)))
        {
            QLine cross[2]; // draw temdeks here

            int x1 = r.x() + 2, x2 = x1 + r.width() - 4;
            int y1 = r.y() + 2, y2 = y1 + r.height() - 4;
            cross[0] = QLine(x1, y1, x2, y2);
            cross[1] = QLine(x2, y1, x1, y2);

            QPen pen;
            pen.setWidth(m_scale);
            pen.setCapStyle(Qt::RoundCap);
            pen.setColor(QColor::fromRgb(255, 0, 0));
            p.setPen(pen);
            p.drawLines(cross, 2);
        }
    }
}

void BoardView::drawDraggedPieces(QPaintEvent* /*event*/)
{
    // Draw dragged piece
    if (m_dragged != Empty)
    {
        QPainter p(this);
        p.drawPixmap(m_dragPoint, m_theme.piece(m_dragged));
    }
}

void BoardView::drawMoveIndicator(QPaintEvent* /*event*/)
{
    if (m_showMoveIndicator & !m_board.hasNoMoves())
    {
        QPainter p(this);
        // Draw side to move indicator
        bool white = m_board.toMove() == White;
        int square = width() - 8 * m_theme.size().width() - 4;
        int maxsquare = m_theme.size().width() / 2;
        if (square > maxsquare)
        {
            square = maxsquare;
        }
        if (square > 8)
        {
            QColor color = white ? Qt::white : Qt::black;
            QColor border = white ? Qt::black : Qt::white;
            int posy = (white == m_flipped) ? 1 :
                14 * m_theme.size().width() - square;
            p.setPen(border);
            p.setBrush(QColor(color));
            p.drawRect(8 * m_theme.size().width() + 8,
                posy, square, square);
        }
    }
}

void BoardView::drawPieces(QPaintEvent* event)
{
    QPainter p(this);

    for (Square square = fsq; square <= lsq; square++)
    {
        QRect rect = squareRect(square);
        if (!event->region().intersects(rect)) continue;

        int x = isFlipped() ? 8 - gBoard[square][0] : gBoard[square][0];
        int y = isFlipped() ? 13 - gBoard[square][1] : gBoard[square][1];

        QPoint pos(x * m_theme.size().width(), y * m_theme.size().height());

        p.drawPixmap(pos, m_theme.piece(m_board.pieceAt(square)));
        if (m_board.isUrgent(square))
            p.drawPixmap(pos, m_theme.urgent());

        if (m_showCurrentMove)
        {
            if (square == m_currentFrom || square == m_currentTo)
            {
                QPen pen;
                pen.setColor(m_theme.color(BoardTheme::CurrentMove));
                pen.setWidth(2);
                pen.setJoinStyle(Qt::MiterJoin);
                p.setPen(pen);
                p.drawRect(pos.x() + 1 + m_showFrame,
                    pos.y() + 1 + m_showFrame,
                        m_theme.size().width() - 2 - m_showFrame,
                            m_theme.size().height() - 2 - m_showFrame);
            }
        }
        if (square == m_selectedSquare ||
                square == m_hiFrom || square == m_hiTo)
        {
            QPen pen;
            pen.setColor(m_theme.color(BoardTheme::Highlight));
            pen.setWidth(2);
            pen.setJoinStyle(Qt::MiterJoin);
            p.setPen(pen);
            p.drawRect(pos.x() + 1 + m_showFrame,
                pos.y() + 1 + m_showFrame,
                    m_theme.size().width() - 2 - m_showFrame,
                        m_theme.size().height() - 2 - m_showFrame);
        }
        if (m_showFrame)
        {
            p.setPen(m_theme.color(BoardTheme::Frame));
            p.drawRect(QRect(pos, m_theme.size()));
        }
    }
}
void BoardView::paintEvent(QPaintEvent* event)
{
    QWidget::paintEvent(event);
    /*
    drawSquares(event);
    drawSquareAnnotations(event);
    drawPieces(event);
    drawMoveIndicator(event);
    drawArrowAnnotations(event);
    drawDraggedPieces(event);
    */
}
#endif



Square BoardView::squareAt(const QPoint& p) const
{
    return m_view? m_view->squareAt(p) : InvalidSquare;
}

void BoardView::leaveEvent(QEvent *)
{
    if (m_messageSend)
    {
        m_messageSend = false;
        signalDisplayMessage("");
    }
}

void BoardView::mousePressEvent(QMouseEvent* event)
{
    // leftclick
    if (event->buttons() & Qt::LeftButton)
    {
        // select / potentially start dragging
        if (m_hoverSquare != InvalidSquare)
        {
            // init for drag start
            m_dragStart = event->pos();
            m_dragStartSquare = m_hoverSquare;

            // generally select
            selectSquare_(m_dragStartSquare);

            event->accept();
            return;
        }
    }
    // right-click
    else if ((event->button() & Qt::RightButton)
             && (m_hoverSquare != InvalidSquare)
             && !(m_flags & F_NoExecuteMoves))
    {
        if (m_guessNextMove && m_board.isMovable(m_hoverSquare))
        {
            showGoals_(m_hoverSquare, 1);
        }
    }
}

void BoardView::mouseMoveEvent(QMouseEvent *event)
{
    //m_button = event->button() + event->modifiers();

    // signal copying
    if (m_flags & F_AllowCopyPiece)
    {
        setCursor((event->modifiers() & Qt::ShiftModifier)?
            QCursor(Qt::DragCopyCursor) : QCursor(Qt::DragMoveCursor));
    }

    // -- on hover --

    if (!event->buttons())
	{
        Square s = squareAt(event->pos());

        // status message on square
        if (s != InvalidSquare)
            displayMessage(squareToString_(s));
        else
        if (m_messageSend)
        {
            displayMessage("");
        }

        // draggable piece?
        if (canDrag(s))
        {
            setCursor(QCursor(Qt::OpenHandCursor));
            setHoverSquare_(s);
            // highlight goal squares
            showGoals_(s);
        }
        // clear highlights
        else
        {
            setCursor(QCursor(Qt::ArrowCursor));
            if (m_flags & F_AllowAllMoves)
                setHoverSquare_(s); // always highlight square
            else
                setHoverSquare_();
            showGoals_();
        }
        event->accept();
		return;
	}

    // -- change drag endpoint --

    if (m_dragged != InvalidPiece)
    {
        m_dragPoint = event->pos();

        Square s = squareAt(m_dragPoint);

        if ((m_flags & F_AllowAllMoves)
            || m_board.canMoveTo(m_dragStartSquare, s))
            setHoverSquare_(s);
        else
            setHoverSquare_();

        // update painter
        if (m_view)
            m_view->setDragPiece(m_dragStartSquare, m_dragged, m_dragPoint,
                                 (m_flags & F_AllowAllMoves) && (event->modifiers() & Qt::ShiftModifier)
                                 );

        event->accept();
        return;
    }

    // -- start dragging --

    if (m_dragStartSquare != InvalidSquare)
    {
        // drag enabeling distance
        if ((event->pos() - m_dragStart).manhattanLength()
                < QApplication::startDragDistance())
            return;

        // can piece be moved (also according to boardview flags)?
        if (!canDrag(m_dragStartSquare))
            return;

        // doit
        m_dragged = m_board.pieceAt(m_dragStartSquare);
        m_dragPoint = event->pos() - m_theme.pieceCenter();
    }

    // XXX why should this be needed? special flags?
    //m_board.removeFrom(s);
}

void BoardView::mouseReleaseEvent(QMouseEvent* event)
{
    Square s = squareAt(event->pos());

    // ---- piece drop ----

    if (m_dragged != InvalidPiece)
    {
        Square from = m_dragStartSquare;
        // XXX probably irrelevant
        //m_board.setAt(from, m_dragged);

        // reset drag data
        m_dragged = InvalidPiece;
        m_dragStartSquare = InvalidSquare;
        // reset painter
        if (m_view) m_view->setDragPiece();
        selectSquare_();
        setCursor(QCursor(Qt::ArrowCursor));

        // dropped on a new square on board?
        if (s != InvalidSquare && s != m_dragStartSquare)
        {
            // copy piece
            if ((m_flags & F_AllowCopyPiece) && (event->modifiers() & Qt::ShiftModifier))
            {
                emit copyPiece(from, s);
            }
            // or move piece
            else
            {
                m_lastDropped = s;
                emit moveMade(from, s, event->button() + event->modifiers());
            }
        }
        else emit invalidMove(from);

        event->accept();
        return;
    }

    // any valid square action?
    else if (m_selectedSquare != InvalidSquare)
    {
        Square from = m_selectedSquare;

        // left-click/release on square
        if (event->button() & Qt::LeftButton)
        {
            std::vector<Square> v;
            m_board.getReachableSquares(from,v);

            // auto execute move?
            if (m_guessMove && v.size() && canDrag(m_selectedSquare)
                && !(m_flags & F_NoExecuteMoves))
            {
                //m_own_from = from;
                //m_own_to = v[m_goal_index%v.size()];
                emit moveMade(from, v[m_goal_index%v.size()],
                                event->button() + event->modifiers());
                event->accept();
                return;
            }
        }
    }

    // any click/release on any square?
    if (s != InvalidSquare && F_AllowAllMoves)
    {
        emit clicked(s, event->button(), event->pos());
        event->accept();
        return;
    }

}

/*
void BoardView::wheelEvent(QWheelEvent* e)
{
    m_wheelCurrentDelta += e->delta();
    if (abs(m_wheelCurrentDelta) > m_minDeltaWheel)
    {
        int change = m_wheelCurrentDelta < 0 ? WheelDown : WheelUp;
        emit wheelScrolled(change + e->modifiers());
        m_wheelCurrentDelta = 0;
    }
}
*/
bool BoardView::execBestMove()
{
    if (m_bestMoveFrom && m_bestMoveTo)
    {
        emit moveMade(m_bestMoveFrom, m_bestMoveTo, Qt::LeftButton);
        return true;
    }
    return false;
}

void BoardView::keyPressEvent(QKeyEvent * e)
{
    // F2 for closing external window
    if (e->key()==Qt::Key_F2 && m_isExternal)
    {
        e->accept();
        close(); // close event will signal mainwindow and update menu-checkbox
        return;
    }
    // execute best move
    else if (e->key() == Qt::Key_Space)
    {
        if (execBestMove())
        {
            e->accept();
            return;
        }
    }
    QWidget::keyPressEvent(e);
}


void BoardView::setFlipped(bool flipped)
{
    if (m_view) m_view->setFlipped(flipped);
}

bool BoardView::isFlipped() const
{
    return (m_view)? m_view->isFlipped() : false;
}


void BoardView::selectSquare_(Square s)
{
    if (s == m_selectedSquare) return;

    // clear previous hightlights
    if (m_view) m_view->clearHighlights(BoardPainter::H_SELECT);

    // assign
    m_selectedSquare = s;
    if (s == InvalidSquare) return;

    // set highlight
    if (m_view && m_showCurrentMove && !(m_flags & F_HideMoveHelp))
        m_view->addHighlight(m_selectedSquare, BoardPainter::H_SELECT);
}

void BoardView::setHoverSquare_(Square s)
{
    if (s == m_hoverSquare) return;

    // reset previous hover highlight
    if (m_view) m_view->clearHighlights(BoardPainter::H_HOVER);

    // assign
    m_hoverSquare = s;
    if (s == InvalidSquare /*|| (m_flags & F_HideMoveHelp)*/) return;

    // set highlight
    if (m_view) m_view->addHighlight(s, BoardPainter::H_HOVER);
}

void BoardView::showGoals_(Square s, int gidx)
{
    // clear all previous
    if (m_view)
        m_view->clearHighlights(BoardPainter::H_GOAL | BoardPainter::H_TARGET);

    if (s == InvalidSquare || (m_flags & F_HideMoveHelp))
        return;

    // get all move goals
    std::vector<Square> squares;
    m_board.getReachableSquares(s, squares);

    // set goal-index
    if (gidx < 0)
    {
        m_goal_index = 0;
        // set to best-move?
        if (m_bestMoveFrom == s)
        {
            for (size_t i=0; i<squares.size(); ++i)
                if (squares[i] == m_bestMoveTo)
                    { m_goal_index = i; break; }
        }
    }
    else if (gidx > 0 && squares.size())
    {
        m_goal_index = (m_goal_index + gidx) % squares.size();
    }

    // highlight goal squares
    if (m_view && m_showAllMoves)
    for (size_t i=0; i<squares.size(); ++i)
    {
        m_view->addHighlight(squares[i], BoardPainter::H_GOAL
                             | (BoardPainter::H_TARGET
                                * (i == m_goal_index))
                                * m_guessMove);
    }
}

/*
QRect BoardView::squareRect(Square square)
{
    int x = isFlipped() ? 8 - gBoard[square][0] : gBoard[square][0];
    int y = isFlipped() ? 13 - gBoard[square][1] : gBoard[square][1];
    return QRect(QPoint(x * m_theme.size().width(),
                        y * m_theme.size().height()), m_theme.size());
}
*/
bool BoardView::canDrag(Square s)
{
    if (m_view->isAnimating())
        return false;
    //if (m_dragged != InvalidPiece) // already dragging
    //    return false;
    if ((m_flags & F_DisableWhite) && m_board.toMove() == White)
        return false;
    if ((m_flags & F_DisableBlack) && m_board.toMove() == Black)
        return false;

    if (s == InvalidSquare)
        return false;
    if (m_flags & F_AllowAllMoves)
        return m_board.pieceAt(s) != Empty;
    if (m_board.isMovable(s))
    {
        //setCursor(QCursor(Qt::OpenHandCursor));
        return true;
    }
    return false;
}

int BoardView::heightForWidth(int width) const
{
    return width + (width>>1);
}

void BoardView::dragEnterEvent(QDragEnterEvent *event)
{
    qDebug() << event;
    const BoardViewMimeData *mimeData = qobject_cast<const BoardViewMimeData *>(event->mimeData());
    if (mimeData)
    {
        event->acceptProposedAction();
    }
}

void BoardView::dragMoveEvent(QDragMoveEvent *event)
{
    event->acceptProposedAction();
}

void BoardView::dragLeaveEvent(QDragLeaveEvent *event)
{
    event->accept();
}

void BoardView::dropEvent(QDropEvent *event)
{
    const BoardViewMimeData *mimeData = qobject_cast<const BoardViewMimeData *>(event->mimeData());
    if (mimeData)
    {
        Square s = squareAt(event->pos());
        emit pieceDropped(s, mimeData->m_piece);
        event->acceptProposedAction();
    }
}

#if (0)
void BoardView::drawSquareAnnotations(QPaintEvent* event)
{
    QString annotation = m_board.squareAnnotation();

    if( !annotation.isEmpty() && !annotation.isNull())
    {
        QStringList list = annotation.split(",");

        for (QStringList::Iterator it = list.begin(); it != list.end(); it++) {
            if (*it != "") {
                drawSquareAnnotation(event,*it);
            }
        }
    }
}

void BoardView::drawArrowAnnotations(QPaintEvent* event)
{
    QString annotation = m_board.arrowAnnotation();

    if( !annotation.isEmpty() && !annotation.isNull())
    {
        QStringList list = annotation.split(",");

        for (QStringList::Iterator it = list.begin(); it != list.end(); it++) {
            if (*it != "") {
                drawArrowAnnotation(event,*it);
            }
        }
    }
}

void BoardView::drawSquareAnnotation(QPaintEvent* event, QString annotation)
{
    QPainter p(this);
    QString trimmed = annotation.simplified();
    QChar colorChar = trimmed[0];
    QChar fileChar = trimmed[1];
    QChar rankChar = trimmed[2];
    QString files = "abcdefgh";
    QString ranks = "12345678";
    int file = files.indexOf(fileChar);
    int rank = ranks.indexOf(rankChar);
    int square = rank * 8 + file;

    QRect rect = squareRect(square);
    if (!event->region().intersects(rect))
        return;
    int x = isFlipped() ? 7 - square % 8 : square % 8;
    int y = isFlipped() ? square / 8 : 7 - square / 8;
    QPoint pos(x * m_theme.size().width(), y * m_theme.size().height());
    //p.drawPixmap(pos, m_theme.square((x + y) % 2));

    QColor color = Qt::red;
    if(colorChar == 'Y') {
        color = Qt::yellow;
    } else if(colorChar == 'G') {
        color = Qt::green;
    } else if(colorChar == 'B') {
        color = Qt::blue;
    }

    p.save();
    QPen pen(color);
    QBrush brush(color);
    p.setPen(pen);
    p.setBrush(brush);

    p.setOpacity(.20);

    p.drawRect(pos.x(),pos.y(),m_theme.size().width(),m_theme.size().height());

    p.restore();

}

void BoardView::drawArrowAnnotation(QPaintEvent* event, QString annotation)
{
    QPainter p(this);

    static QString letters = "abcdefgh";
    static QString numbers = "12345678";


    QString trimmed = annotation.simplified();

    QChar colorChar = trimmed[0];
    QChar fileChar1 = trimmed[1];
    QChar rankChar1 = trimmed[2];
    QChar fileChar2 = trimmed[3];
    QChar rankChar2 = trimmed[4];
    int file1 = letters.indexOf(fileChar1);
    int rank1 = numbers.indexOf(rankChar1);
    int file2 = letters.indexOf(fileChar2);
    int rank2 = numbers.indexOf(rankChar2);

    if( file1 < 0 || file2 < 0 || rank1 < 0 || rank2 < 0 )
    {
        return;
    }
    int square1 = rank1 * 8 + file1;
    int square2 = rank2 * 8 + file2;

    QRect rect1 = squareRect(square1);
    QRect rect2 = squareRect(square2);
    QRect u = rect1.united(rect2);
    if (!event->region().intersects(u))
        return;
    int x1 = isFlipped() ? 7 - square1 % 8 : square1 % 8;
    int y1 = isFlipped() ? square1 / 8 : 7 - square1 / 8;
    int x2 = isFlipped() ? 7 - square2 % 8 : square2 % 8;
    int y2 = isFlipped() ? square2 / 8 : 7 - square2 / 8;
    int w = m_theme.size().width();
    int h = m_theme.size().height();
    QPoint pos1((x1 * w) + (w/2), (y1 * h) + (h/2));
    QPoint pos2((x2 * w )+ (w/2), (y2 * h) + (h/2));

    QColor color = Qt::red;
    if(colorChar == 'Y') {
        color = Qt::yellow;
    } else if(colorChar == 'G') {
        color = Qt::green;
    } else if(colorChar == 'B') {
        color = Qt::blue;
    }

    // Now to Draw Arrow Head
    qreal headWidth = 16.0;
    qreal headLength = 16.0;
    qreal headIndent = 4.0;
    qreal netIndent = headLength- headIndent;

    qreal halfHead = headWidth / 2;
    int px1 = pos1.x();
    int px2 = pos2.x();
    int py1 = pos1.y();
    int py2 = pos2.y();
    int dX = px2 - px1;
    int dY = py2 - py1;

    qreal  arrowLength = qSqrt( dX * dX + dY * dY );

    QPointF arrowPts[7];

    // we will shorten the line somewhat to avoid arrows all colliding in the center of the square
    int adjust = ( w + h ) / 6;

    px1 = px1 + (( adjust * dX ) / arrowLength);
    px2 = px2 - (( adjust * dX ) / arrowLength);
    py1 = py1 + (( adjust * dY ) / arrowLength);
    py2 = py2 - (( adjust * dY ) / arrowLength);


    // calculate the points that form the arrow
    arrowPts[0].setX( px2 - ((netIndent * dX) / arrowLength));
    arrowPts[0].setY( py2 - ((netIndent * dY) / arrowLength));
    arrowPts[4].setX( px2 - ((headLength * dX) / arrowLength));
    arrowPts[4].setY( py2 - ((headLength * dY) / arrowLength));
    arrowPts[1].setX( arrowPts[4].x() - ((halfHead * (dY)) / arrowLength));
    arrowPts[1].setY( arrowPts[4].y() - ((halfHead * (-dX)) / arrowLength));
    arrowPts[3].setX( arrowPts[4].x() + ((halfHead * (dY)) / arrowLength));
    arrowPts[3].setY( arrowPts[4].y() + ((halfHead * (-dX)) / arrowLength));
    arrowPts[2].setX( px2);
    arrowPts[2].setY( py2);

    QPoint pos3(px1,py1);
    QPoint pos4(px2,py2);

    p.save();
    QPen pen(color);
    pen.setWidth(2);
    p.setPen(pen);

    p.drawLine(pos3,pos4);

    // For now only draw part of the arrowhead
    //p.drawLine(arrowPts[2],arrowPts[1]);
    //p.drawLine(arrowPts[2],arrowPts[3]);

    QBrush brush(color);
    p.setBrush(brush);


    p.drawPolygon(arrowPts,4);
    p.restore();

}
#endif
