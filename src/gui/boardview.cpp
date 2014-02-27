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


BoardView::BoardView(QWidget* parent, int flags) : QWidget(parent),
    m_view(0),
    m_showCurrentMove(true),
    m_showAllMoves(true),
    m_selectedSquare(InvalidSquare),
    m_hoverSquare(InvalidSquare),
    m_currentFrom(InvalidSquare),
    m_currentTo(InvalidSquare),
    m_flags(flags),
    m_dragged(Empty),
    m_clickUsed(false),
    m_wheelCurrentDelta(0),
    m_minDeltaWheel(0),
    m_moveListCurrent(0)
{
    //QSizePolicy policy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    //policy.setHeightForWidth(true);
    //setSizePolicy(policy);
    setMouseTracking(true);
    //installEventFilter(this); XXX currently not used
	m_board.setStandardPosition();

//    setBackgroundRole(QPalette::Link);
//    setAutoFillBackground(true);

    // to place the BoardPainter in
    m_layout = new QVBoxLayout(this);
    m_layout->setContentsMargins(0,0,0,0);
}

BoardView::~BoardView()
{

}

/*bool BoardView::eventFilter(QObject *obj, QEvent *ev)
{
	return QWidget::eventFilter(obj, ev);
}*/

void BoardView::setFlags(int flags)
{
    m_flags = flags;
}

void BoardView::setBoard(const Board& value,int from, int to)
{
    //qDebug() << "setBoard(from=" << from << ", to=" << to << ")";

    m_clickUsed = true;
	m_board = value;
    m_currentFrom = from;
    m_currentTo = to;
    if (m_view)
        m_view->setBoard(value,from,to);
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

        if ((sq == temdekAt[Black] && m_board.temdekOn(Black)) ||
            (sq == temdekAt[White] && m_board.temdekOn(White)))
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
    /*
	int x = p.x(), y = p.y();
	int width = m_theme.size().width();
	int height = m_theme.size().height();
	if (m_coordinates) {
		x -= CoordinateSize;
		y -= CoordinateSize;
	}
    if (x <= 0 || y <= 0 || x >= width*8 || y >= height*15)
		return InvalidSquare;
	x /= width;
	y /= height;
    return isFlipped() ? BN[((8 - x)<<4) + y + 1] :
                         BN[(x<<4) + 14 - y];
    */
}

void BoardView::mousePressEvent(QMouseEvent* event)
{
    m_dragStart = event->pos();
    m_dragStartSquare = squareAt(event->pos());
}

void BoardView::mouseMoveEvent(QMouseEvent *event)
{
    m_button = event->button() + event->modifiers();

/*
    if (event->modifiers() & Qt::ControlModifier)
    {
        setCursor(QCursor(Qt::UpArrowCursor));
    }
    else if (event->modifiers() & Qt::AltModifier)
    {
        setCursor(QCursor(Qt::DragCopyCursor));
    }
    else
    {
        setCursor(QCursor(Qt::ArrowCursor));
    }

    if (event->modifiers() & Qt::ShiftModifier)
    {
        return;
    }
*/
    // on hover
	if (!(event->buttons() & Qt::LeftButton))
	{
        Square s = squareAt(event->pos());
        if (m_board.isMovable(s))
        {
        	m_currentFrom = s;
            setCursor(QCursor(Qt::OpenHandCursor));
            setHoverSquare(s);
            if (m_showAllMoves) showPossibleMoves(s);
        }
        else
        {
        	m_currentFrom = InvalidSquare;
            setCursor(QCursor(Qt::ArrowCursor));
            setHoverSquare(InvalidSquare);
            if (m_showAllMoves && m_view) m_view->clearReachableSquares();
        }
		return;
	}

    // set drag endpoint
    if (m_dragged != Empty)
    {
        Square s = squareAt(event->pos());
        if (m_showCurrentMove)
        {
            if (m_board.canMoveTo(m_currentFrom, s))
                selectSquare(s);
            else
                selectSquare();
        }
        m_dragPoint = event->pos();

        // update painter
        if (m_view)
            m_view->setDragPiece(m_dragStartSquare, m_dragged, m_dragPoint);

        return;
    }

    // start dragging
    if ((event->pos() - m_dragStart).manhattanLength()
            < QApplication::startDragDistance())  // Click and move - start dragging
        return;
    // can piece be moved
    Square s = squareAt(m_dragStart);
    if (!canDrag(s))
        return;
    // doit
    m_dragged = m_board.pieceAt(s);
    m_dragPoint = event->pos() - m_theme.pieceCenter();

    // XXX why should this be needed? special flags?
    //m_board.removeFrom(s);
}

void BoardView::mouseReleaseEvent(QMouseEvent* event)
{
    setCursor(QCursor(Qt::ArrowCursor));
    int button = event->button() + event->modifiers();
    Square s = squareAt(event->pos());
    m_clickUsed = false;

    if (m_showAllMoves && m_view) m_view->clearReachableSquares();

    if (!(event->button() & Qt::LeftButton))
    {
        if (s != InvalidSquare)
        {
            emit clicked(s, button, mapToGlobal(event->pos()));
        }
        else
        {
            // XXX what means this?
            // dragging with wrong mouse button is invalid move? sb
            Square from = m_dragStartSquare;
            emit invalidMove(from);
        }
        // reset drag (if any)
        m_dragged = Empty;
        if (m_view)
            m_view->setDragPiece();
        return;
    }

    // l-button
    else
    {
        if (event->modifiers() & Qt::ShiftModifier)
        {
            if (s != InvalidSquare)
            {
                emit clicked(s, button, mapToGlobal(event->pos()));
            }
            return;
        }
    }

    // end of drag action
    if (m_dragged != Empty)
    {
        Square from = m_dragStartSquare;
        // XXX probably irrelevant
        //m_board.setAt(from, m_dragged);

        // clear highlights
        m_dragged = Empty;
        if (m_view) m_view->setDragPiece();
        selectSquare();

        // check if valid move
        if (s != InvalidSquare)
        {
            // copy piece
            if ((m_flags & AllowCopyPiece) && (event->modifiers() & Qt::AltModifier))
            {
                if (m_board.pieceAt(from) != Empty)
                {
                    emit copyPiece(from, s);
                }
            }
            // or move piece
            else emit moveMade(from, s, button);
        }
        else emit invalidMove(from);

    }

    // XXX not really sure what below functions do
    // they might be broken since i changed the select logic a bit

    // XXX probably single click execute move
    else if (m_selectedSquare != InvalidSquare)
    {
        Square from = m_selectedSquare;
        selectSquare();
        if (s != InvalidSquare)
        {
            emit moveMade(from, s, button);
        }
    }

    // XXX hiFrom seems not be used
    /*
    else if (m_hiFrom != InvalidSquare)
    {
        if (s == m_hiFrom || s == m_hiTo)
        {
            emit moveMade(m_hiFrom, m_hiTo, button);
        }
        setHoverSquare();
    }
    // ???
    else
    {
        if (s != InvalidSquare)
        {
            emit clicked(s, button, mapToGlobal(event->pos()));
            if (!m_clickUsed && m_board.isMovable(s))
            {
                selectSquare(s);
            }
        }
    }*/
}

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

void BoardView::setFlipped(bool flipped)
{
    if (m_view) m_view->setFlipped(flipped);
}

bool BoardView::isFlipped() const
{
    return (m_view)? m_view->isFlipped() : false;
}

void BoardView::configure()
{
    AppSettings->beginGroup("/Board/");
    m_showCurrentMove = AppSettings->getValue("showCurrentMove").toBool();
    m_showAllMoves = AppSettings->getValue("showAllMoves").toBool();
    m_minDeltaWheel = AppSettings->getValue("minWheelCount").toInt();
    AppSettings->endGroup();

    m_theme.configure();
    m_theme.setSize(QSize(100,100));

    selectSquare();

    // recreate BoardPainter
    if (m_view) delete m_view;
    m_view = new BoardPainter(&m_theme, this);
    m_view->setBoard(m_board);
    m_layout->addWidget(m_view);

	update();
}

void BoardView::selectSquare(Square s)
{
    if (s == m_selectedSquare) return;

    // unset view
    if (m_selectedSquare != InvalidSquare && m_view)
        m_view->clearSquareColor(m_selectedSquare);

    // unselect
    m_selectedSquare = InvalidSquare;
    if (s == InvalidSquare) return;

    // set view
    if (m_view)
        m_view->setSquareColor(s, m_theme.color(BoardTheme::Highlight));

    m_selectedSquare = s;
}

void BoardView::setHoverSquare(Square s)
{
    if (s == m_hoverSquare) return;

    // reset previous hover view
    if (m_hoverSquare != InvalidSquare && m_view)
        m_view->clearSquareColor(m_hoverSquare);

    // clear flag
    if (s == InvalidSquare)
    {
        m_hoverSquare = s;
        return;
    }

    if (m_view)
        m_view->setSquareColor(s, m_theme.color(BoardTheme::Highlight));

    m_hoverSquare = s;
}

void BoardView::showPossibleMoves(Square s)
{
    if (!m_view) return;

    std::vector<Square> squares;
    m_board.getReachableSquares(s, squares);
    m_view->setReachableSquares(squares);
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
    if (m_dragged != Empty) // already dragging
        return false;
    if (s == InvalidSquare)
        return false;
    else if (m_flags & IgnoreSideToMove)
        return m_board.pieceAt(s) != Empty;
    else if (m_board.isMovable(s))
    {
        setCursor(QCursor(Qt::OpenHandCursor));
        return true;
    }
    else return false;
}

int BoardView::heightForWidth(int width) const
{
    return width + (width>>1);
}

void BoardView::dragEnterEvent(QDragEnterEvent *event)
{
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
