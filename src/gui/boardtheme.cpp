/***************************************************************************
 *                           Shatrabase                                    *
 *   (C) 2012-2014 AFKM <francis@afkm.com>                                 *
 *                   derived from ChessX with thanks                       *
 *   (C) 2005 Michal Rudolf <mrudolf@kdewebdev.org>                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "boardtheme.h"
#include "settings.h"
#include "messagedialog.h"

#include <QDebug>
#include <QPixmap>

BoardTheme::BoardTheme()
{
    m_pieceFilename = "new QString()";
    m_originalUrgent.load(":/images/urgent.png");
}

BoardTheme::~BoardTheme()
{}

const QPixmap& BoardTheme::piece(Piece p) const
{
	return m_piece[p];
}

const QPixmap& BoardTheme::square(bool dark) const
{
    return m_square[dark?1:0];
}

const QPixmap& BoardTheme::urgent() const
{
    return m_urgent;
}

const QPixmap& BoardTheme::originalPiece(Piece p) const
{
	return m_originalPiece[p];
}

const QPixmap& BoardTheme::originalSquare(bool dark) const
{
    return m_originalSquare[dark?1:0];
}

QString BoardTheme::pieceThemeName() const
{
	int start = m_pieceFilename.lastIndexOf('/') + 1;
	return m_pieceFilename.mid(start + 1, m_pieceFilename.length() - start - 4);
}

QString BoardTheme::boardThemeName() const
{
	int start = m_boardFilename.lastIndexOf('/') + 1;
	return m_boardFilename.mid(start + 1, m_pieceFilename.length() - start - 4);
}

bool BoardTheme::isValid() const
{
	return !m_pieceFilename.isNull();
}

bool BoardTheme::loadPieces(const QString& pieces, int effect)
{
    QString effectPath;
    if (effect == Outline) effectPath = "outline";
    else if (effect == Shadow) effectPath = "shadow";
    QString themePath = QString("%1/%2/%3.png").arg(themeDirectory()).arg(effectPath).arg(pieces);

	QPixmap big;
	if (!big.load(themePath) || big.width() < 128)
		return false;

    int realsize = big.height() / 3;
    if (realsize != big.width() / 5)
		return false;
    int rs2 = realsize + realsize;

	/* Cut big theme bitmap into separate pieces */
	m_originalPiece[WhiteTura] = big.copy(0 * realsize, 0, realsize, realsize);
	m_originalPiece[WhiteYalkyn] = big.copy(1 * realsize, 0, realsize, realsize);
	m_originalPiece[WhiteBatyr] = big.copy(2 * realsize, 0, realsize, realsize);
	m_originalPiece[WhiteBiy] = big.copy(3 * realsize, 0, realsize, realsize);
	m_originalPiece[WhiteShatra] = big.copy(4 * realsize, 0, realsize, realsize);
	m_originalPiece[BlackTura] = big.copy(0 * realsize, realsize, realsize, realsize);
	m_originalPiece[BlackYalkyn] = big.copy(1 * realsize, realsize, realsize, realsize);
	m_originalPiece[BlackBatyr] = big.copy(2 * realsize, realsize, realsize, realsize);
	m_originalPiece[BlackBiy] = big.copy(3 * realsize, realsize, realsize, realsize);
	m_originalPiece[BlackShatra] = big.copy(4 * realsize, realsize, realsize, realsize);
    m_originalPiece[WasTura] = big.copy(0 * realsize, rs2, realsize, realsize);
    m_originalPiece[WasYalkyn] = big.copy(1 * realsize, rs2, realsize, realsize);
    m_originalPiece[WasBatyr] = big.copy(2 * realsize, rs2, realsize, realsize);
    m_originalPiece[WasShatra] = big.copy(4 * realsize, rs2, realsize, realsize);
    m_pieceFilename = themePath;

	if (size().isEmpty())
		setSize(QSize(realsize, realsize));
    else setSize(size());
	return true;
}

bool BoardTheme::loadBoard(const QString& board)
{
	if (board.isEmpty()) {
		m_boardFilename = board;
		updateSquares();
		return true;
	}
    QString themePath = QString("%1/%2.png").arg(boardDirectory()).arg(board);
	QPixmap big;
	if (!big.load(themePath))
		return false;
	int realsize = big.height() / 2;
	if (realsize != big.width())
		return false;

	m_originalSquare[0] = big.copy(0, 0, realsize, realsize);
	m_originalSquare[1] = big.copy(0, realsize, realsize, realsize);
	m_boardFilename = themePath;

	updateSquares();
	return true;
}

void BoardTheme::configure()
{
    AppSettings->beginGroup("/Board/");
    QString pieceTheme = AppSettings->getValue("pieceTheme").toString();
    int pieceEffect = AppSettings->getValue("pieceEffect").toInt();
    QString boardTheme = AppSettings->getValue("boardTheme").toString();
    AppSettings->endGroup();

    if (!loadPieces(pieceTheme, pieceEffect) &&
         !loadPieces(pieceTheme, Plain) &&
            !loadPieces("motifshatra", Plain))
    {
        MessageDialog::error(tr("Cannot find piece data.\nPlease check your installation."));
    }
    loadBoard(boardTheme);
}

QSize BoardTheme::size() const
{
	return m_size;
}

QPoint BoardTheme::pieceCenter() const
{
	return QPoint(m_size.width() / 2, m_size.height() / 2);
}

QRect BoardTheme::rect() const
{
	return QRect(QPoint(0, 0), m_size);
}

void BoardTheme::setSize(const QSize& value)
{
	if (!isValid())
		return;
	m_size = value;
	for (int i = 1; i < ConstPieceTypes; i++)
		m_piece[i] = m_originalPiece[i].scaled(m_size, 
		    Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	updateSquares();
    m_urgent = m_originalUrgent.scaled(m_size,
        Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
}

void BoardTheme::updateSquares()
{
	if (!isValid())
		return;
	bool scale = m_size.width() > m_originalPiece[WhiteTura].width()
			  || m_size.height() > m_originalPiece[WhiteTura].height()
			  || m_size.width() < 30 || m_size.height() < 30;
	if (isBoardPlain()) {
		m_square[0] = QPixmap(m_size);
		m_square[0].fill(color(LightSquare).rgb());
		m_square[1] = QPixmap(m_size);
		m_square[1].fill(color(DarkSquare).rgb());
	} else if (scale) {
		m_square[0] =  m_originalSquare[0].scaled(size(), Qt::IgnoreAspectRatio,
				Qt::SmoothTransformation);
		m_square[1] =  m_originalSquare[1].scaled(size(), Qt::IgnoreAspectRatio,
				Qt::SmoothTransformation);
	} else {
        m_square[0] =  m_originalSquare[0].copy(rect());
        m_square[1] =  m_originalSquare[1].copy(rect());
	}
}

QString BoardTheme::themeDirectory() const
{
    QString path = AppSettings->dataPath() + "/themes";
    if (QFile::exists(path))
        return path;
    else
        return QString(":/themes");
}

QString BoardTheme::boardDirectory() const
{
    QString path = AppSettings->dataPath() + "/themes/boards";
    if (QFile::exists(path))
        return path;
    else
        return QString(":/themes/boards");
}

bool BoardTheme::isBoardPlain() const
{
	return m_boardFilename.isEmpty();
}

void BoardTheme::setColor(ColorRole role, const QColor& value)
{
	m_colors[role] = value;
	if (role == LightSquare || role == DarkSquare)
		updateSquares();
}

QColor BoardTheme::color(ColorRole role) const
{
	return m_colors[role];
}

