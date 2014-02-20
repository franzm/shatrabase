/***************************************************************************
 *                           Shatrabase                                    *
 *   (C) 2012-2014 AFKM <francis@afkm.com>                                 *
 *                   derived from ChessX with thanks                       *
 *   Copyright (C) 2012 by Jens Nissen jens-chessx@gmx.net                 *
 ***************************************************************************/

#ifndef PLAYERLISTWIDGET_H
#define PLAYERLISTWIDGET_H

#include <QWidget>
#include <QStringListModel>
#include "playerinfo.h"

namespace Ui {
class TagDetailWidget;
}

class Database;

class PlayerListWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit PlayerListWidget(QWidget *parent = 0);
    ~PlayerListWidget();

signals:
    void filterRequest(QString ts);
    void raiseRequest();

public slots:
    void findPlayers(const QString& s);
    void slotSelectPlayer(const QString& player);
    void setDatabase(Database* db);
    void showSelectedPlayer();
    void filterSelectedPlayer();
    void slotReconfigure();
    void selectionChangedSlot();
    void slotLinkClicked(const QUrl& url);

protected slots:
    void selectPlayer(const QString& player);

private:
    PlayerInfo m_player;
    QStringList m_list;
    Ui::TagDetailWidget *ui;
    QStringListModel* m_filterModel;
};

#endif // PLAYERLISTWIDGET_H
