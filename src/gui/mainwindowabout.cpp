/***************************************************************************
 *                           Shatrabase                                    *
 *   (C) 2012-2014 AFKM <francis@afkm.com>                                 *
 *                   derived from ChessX with thanks                       *
 *   (C) 2010 by Michal Rudolf mrudolf@kdewebdev.org                       *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "mainwindow.h"
#include "version.h"

#include <QMessageBox>
#include <QPixmap>

void MainWindow::slotHelpAbout()
{
    QString version = QString(STR_REVISION).replace(',','.');
    QMessageBox dialog(QMessageBox::NoIcon, tr("About Shatrabase"), tr("<h1>Shatrabase</h1>"
                        "<p>Free Shatra database Version %1</p>"
                        "<p>Derived from Chessx Version %1</p>"
                        COPYRIGHT
                        "<p>License: GNU Public License 2 - GPLv2</p>"
                        "<p>Developer: <a href=\"mailto:francis@afkm.com\">Francis Monkman</a></p>"
                        "<p>Chessx maintainer: <a href=\"mailto:jens-chessx@gmx.net\">Jens Nissen</a></p>"
                        "<p>Testing, usability and website: Bruno Rizzuti</p>"
                        "<p>Further maintainers: Michal Rudolf, Ejner Borgbjerg</p>"
                        "<p>Additional coding: James Coons, Marius Roets, Sean Estabrooks, Rico Zenklusen, "
                        "Heinz Hopfgartner, William Hoggarth.</p>"
                        "<p>Homepage: <a href=\"http://chessx.sourceforge.net\">http://chessx.sourceforge.net</a><br>"
                        "Mailing list: <a href=\"mailto:chessx-users@lists.sourceforge.net\">chessx-users@lists.sourceforge.net</a></p>")
                        .arg(version),
                QMessageBox::Ok);
    dialog.setIconPixmap((QPixmap(":/images/shatrabase.png")));
    dialog.exec();
}
