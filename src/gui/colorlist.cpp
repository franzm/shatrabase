/***************************************************************************
 *                           Shatrabase                                    *
 *   (C) 2012-2014 AFKM <francis@afkm.com>                                 *
 *                   derived from ChessX with thanks                       *
 *   (C) 2006 Michal Rudolf <mrudolf@kdewebdev.org>                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "colorlist.h"

#include <QColorDialog>
#include <QKeyEvent>
#include <QListWidget>
#include <QMouseEvent>
#include <QPainter>
#include <QPixmap>

#include "settings.h"

ColorList::ColorList(QWidget* parent) : QListWidget(parent)
{
}

ColorList::~ColorList()
{}

void ColorList::addItem(const QString& text, const QColor& color)
{
	QListWidgetItem* item = new QListWidgetItem(this);
	item->setText(text);
	if (count() == 1)
		setIconSize(QSize(100, sizeHintForRow(0) - 4));
	setItemColor(item, color);

}

void ColorList::editItemColor(QListWidgetItem* item)
{
	if (!item)
		return;

    qDebug() << "diag";

    QColorDialog * cd = new QColorDialog(color(item), this);

    cd->setWindowTitle(tr("Choose color for %1").arg(item->text()));
    cd->setOptions(QColorDialog::DontUseNativeDialog);

    // read custom colors
    for (int i=0; i<QColorDialog::customCount(); ++i)
    {
        QColorDialog::setCustomColor(i,
            AppSettings->getValue(QString("/CustomColors/Color%1").arg(i)).value<QColor>());
    }

    if (cd->exec())
    {
        if (cd->currentColor().isValid())
            setItemColor(item, cd->currentColor());
    }

    // store custom colors
    for (int i=0; i<QColorDialog::customCount(); ++i)
    {
        AppSettings->setValue(QString("/CustomColors/Color%1").arg(i),
                              QColorDialog::customColor(i));
    }


}

void ColorList::mousePressEvent(QMouseEvent* event)
{
	QListWidgetItem* item = itemAt(event->pos());
	if (event->button() != Qt::LeftButton || item == 0)
		event->ignore();
	else if (item == currentItem())
		editItemColor(item);
	else
		setCurrentItem(item);
}

void ColorList::keyPressEvent(QKeyEvent* event)
{
#ifdef Q_WS_MAC
	if (event->key() == Qt::Key_0 && event->modifiers() == Qt::Key_Control)
#else
	if (event->key() == Qt::Key_Return)
#endif
		editItemColor(currentItem());
	else
		QListWidget::keyPressEvent(event);
}

void ColorList::setItemColor(QListWidgetItem* item, const QColor& color)
{
	QPixmap pix(iconSize());
	pix.fill(Qt::transparent);
	QPainter painter(&pix);
	painter.setBrush(palette().color(QPalette::Background));
	painter.setPen(palette().color(QPalette::Text));
	painter.setBrush(color);
	painter.drawRect(0, 0, 99, iconSize().height() - 1);
	QIcon icon;
	icon.addPixmap(pix, QIcon::Normal);
	icon.addPixmap(pix, QIcon::Selected);
	icon.addPixmap(pix, QIcon::Active);
	item->setIcon(icon);
	item->setData(Qt::UserRole, QVariant::fromValue<QColor>(color));
}

QColor ColorList::color(QListWidgetItem* item) const
{
	return item->data(Qt::UserRole).value<QColor>();
}

QColor ColorList::color(int index) const
{
	return item(index)->data(Qt::UserRole).value<QColor>();
}

