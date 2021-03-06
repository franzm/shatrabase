/***************************************************************************
 *                           Shatrabase                                    *
 *   (C) 2012-2014 AFKM <francis@afkm.com>                                 *
 *                   derived from ChessX with thanks                       *
 ***************************************************************************/

#ifndef BOARDSETUPTOOLBUTTON_H
#define BOARDSETUPTOOLBUTTON_H

#include "common.h"

#include <QLabel>
#include <QPixmap>

class BoardSetupToolButton : public QLabel
{
    Q_OBJECT
public:
    explicit BoardSetupToolButton(QWidget *parent = 0);
    QPixmap m_pixmap;
    SHATRA::Piece m_piece;

signals:
    void signalDragStarted(QWidget*, QMouseEvent*);
    void signalClicked(SHATRA::Piece p);

public slots:
    void slotClearBackground(SHATRA::Piece p);
    void slotSetSelected();
    
protected:

    virtual void mousePressEvent ( QMouseEvent * e );
    virtual void mouseReleaseEvent ( QMouseEvent * e );
    virtual void resizeEvent(QResizeEvent * e);
};

#endif // BOARDSETUPTOOLBUTTON_H
