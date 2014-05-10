/***************************************************************************
 *                           Shatrabase                                    *
 *   (C) 2012-2014 AFKM <francis@afkm.com>                                 *
 *                   derived from ChessX with thanks                       *
 *	 (C) 2006 Michal Rudolf <mrudolf@kdewebdev.org>                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "database.h"
#include "databasemodel.h"
#include "databaseinfo.h"
#include "game.h"
#include "gamelist.h"
#include "GameMimeData.h"
#include "settings.h"
#include "sortfiltermodel.h"

#include <QDrag>
#include <QHeaderView>
#include <QMenu>
#include <QPixmap>


GameList::GameList(DatabaseInfo* db, QWidget* parent)
    : TableView(parent),
      m_db  (db),
      m_model   (0)
{
	setObjectName("GameList");
	setWindowTitle(tr("Game list"));

    m_model = new DatabaseModel(m_db->database(), this);
    m_sort = new SortFilterModel(this);
    m_sort->setSourceModel(m_model);

    setSelectionMode(//QAbstractItemView::ContiguousSelection
                     QAbstractItemView::ExtendedSelection);

    setModel(m_sort);
	connect(this, SIGNAL(clicked(const QModelIndex&)), SLOT(itemSelected(const QModelIndex&)));
	connect(this, SIGNAL(activated(const QModelIndex&)), SLOT(itemSelected(const QModelIndex&)));
    connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), SLOT(slotContextMenu(const QPoint&)));

#if QT_VERSION < 0x050000
    horizontalHeader()->setClickable(true);
#else
    horizontalHeader()->setSectionsClickable(true);
#endif
    //connect(horizontalHeader(), SIGNAL(sectionClicked(int)), SLOT(sortByColumn(int)));
                                                    //SLOT(simpleSearch(int)));
    slotReconfigure();

    setSortingEnabled(true);
    setDragEnabled(true);
}


GameList::~GameList()
{
}

void GameList::itemSelected(const QModelIndex& index)
{
    //const int i = m_sort->data(m_sort->index(index.row(), 0)).toInt() - 1;
    emit selected(m_sort->mapToSource(index).row());
    // XXX startToDrag(index);
}

void GameList::setDatabaseInfo(DatabaseInfo* dbi)
{
    m_model->deleteLater();

    m_db = dbi;
    m_model = new DatabaseModel(m_db->database(), this);
    m_sort->setSourceModel(m_model);

    emit raiseRequest();
}

void GameList::slotContextMenu(const QPoint& pos)
{
    QModelIndex cell = indexAt(pos);
    QModelIndexList selection = selectedIndexes();
    // Make sure the right click occured on a cell!
    if (cell.isValid() && selection.contains(cell))
    {
        QMenu menu(this);
        menu.addAction(tr("Copy game(s) ..."), this, SLOT(slotCopyGame()));
        menu.addSeparator();

        QAction* a = menu.addAction(tr("Delete/Undelete game(s)"), this, SLOT(slotDeleteGame()));
        a->setCheckable(true);
        a->setEnabled(!m_db->database()->isReadOnly());
        a->setChecked(m_db->database()->deleted(cell.row()));
        menu.exec(mapToGlobal(pos));
    }
}

void GameList::simpleSearch(int tagid)
{
    Q_UNUSED(tagid);
    /*
	QuickSearchDialog dialog(this);

	dialog.setTag(tagid);
    if (m_model->sourceFilter()->filter()->count() <= 1)
		dialog.setMode(1);
	if (dialog.exec() != QDialog::Accepted)
		return;

    QString tag = m_model->sourceFilter()->GetColumnTags().at(dialog.tag());
	QString value = dialog.value();

	if (value.isEmpty())
    {
        m_model->sourceFilter()->filter()->setAll(1);
    }
    else if ((dialog.tag() == 0) || (dialog.tag() == 7) || (dialog.tag() == 11))
    {	// filter by number
        NumberSearch ns(m_model->sourceFilter()->filter()->database(), value);
		if (dialog.mode())
        {
            m_model->sourceFilter()->filter()->executeSearch(ns, Search::Operator(dialog.mode()));
        }
        else
        {
            m_model->sourceFilter()->filter()->executeSearch(ns);
        }
	}
    else
    {
        QStringList list = value.split("-", QString::SkipEmptyParts);
        if (list.size()>1)
        {
            // Filter a range
            TagSearch ts(m_model->sourceFilter()->filter()->database(), tag, list.at(0),list.at(1));
            if (dialog.mode())
            {
                m_model->sourceFilter()->filter()->executeSearch(ts, Search::Operator(dialog.mode()));
            }
            else
            {
                m_model->sourceFilter()->filter()->executeSearch(ts);
            }
        }
        else
        {
            // Filter tag using partial values
            TagSearch ts(m_model->sourceFilter()->filter()->database(), tag, value);
            if (dialog.mode())
            {
                m_model->sourceFilter()->filter()->executeSearch(ts, Search::Operator(dialog.mode()));
            }
            else
            {
                m_model->sourceFilter()->filter()->executeSearch(ts);
            }
        }
	}
    updateFilter();
	emit searchDone();
    */
}

/* Select and show current game in the list */
void  GameList::slotFilterListByPlayer(QString s)
{
    Q_UNUSED(s);
    /*
    TagSearch ts(m_model->sourceFilter()->filter()->database(), "White", s);
    TagSearch ts2(m_model->sourceFilter()->filter()->database(), "Black", s);
    m_model->sourceFilter()->filter()->executeSearch(ts);
    m_model->sourceFilter()->filter()->executeSearch(ts2,Search::Or);
    updateFilter();
    emit raiseRequest();	
    emit searchDone();
    */
}

void  GameList::slotFilterListByEvent(QString s)
{
    Q_UNUSED(s);
    /*
    TagSearch ts(m_model->sourceFilter()->filter()->database(), "Event", s);
    m_model->sourceFilter()->filter()->executeSearch(ts);
    updateFilter();
    emit raiseRequest();
    emit searchDone();
    */
}

void  GameList::slotFilterListByEventPlayer(QString event, QString player)
{
    Q_UNUSED(event);
    Q_UNUSED(player);
    /*
    TagSearch ts(m_model->sourceFilter()->filter()->database(), "White", player);
    TagSearch ts2(m_model->sourceFilter()->filter()->database(), "Black", player);
    TagSearch ts3(m_model->sourceFilter()->filter()->database(), "Event", event);
    m_model->sourceFilter()->filter()->executeSearch(ts);
    m_model->sourceFilter()->filter()->executeSearch(ts2,Search::Or);
    m_model->sourceFilter()->filter()->executeSearch(ts3,Search::And);
    updateFilter();
    emit raiseRequest();
    emit searchDone();
    */
}

void GameList::selectGame(int index)
{
    const QModelIndex i = m_sort->mapFromSource(m_model->index(index,0,QModelIndex()));
    setCurrentIndex(i);
    selectRow(i.row());
}

void GameList::updateFilter()
{
    //m_model->sourceFilter()->setFilter(m_model->sourceFilter()->filter());
}

void GameList::slotCopyGame()
{
    emit requestCopyGame();
}

void GameList::slotDeleteGame()
{
    if (numSelected() < 1)
        return;

    QVector<int> idx;
    for (int i=0; i<numSelected(); ++i)
        idx.push_back(selectedGame(i));

    emit requestDeleteGames(idx);
}

void GameList::startToDrag(const QModelIndex&)
{
    GameMimeData *mimeData = new GameMimeData;
    emit requestGameData(mimeData->m_game);
    QPixmap pixmap = style()->standardPixmap(QStyle::SP_FileIcon);

    QDrag* pDrag = new QDrag(this);
    pDrag->setMimeData(mimeData);
    pDrag->setPixmap(pixmap);
    // pDrag->setHotSpot(hotSpot);

    pDrag->exec(Qt::CopyAction | Qt::MoveAction, Qt::CopyAction);
}


int GameList::numSelected() const
{
    return selectionModel()->selectedRows().size();
}

int GameList::selectedGame(int index) const
{
    return m_sort->mapToSource(selectionModel()->selectedRows()[index]).row();
}
