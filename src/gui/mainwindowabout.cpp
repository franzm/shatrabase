/***************************************************************************
 *                           Shatrabase                                    *
 *   (C) 2012-2014 AFKM <francis@afkm.com>                                 *
 *                   derived from ChessX with thanks                       *
 *   (C) 2010 by Michal Rudolf mrudolf@kdewebdev.org                       *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "mainwindow.h"
#include "version.h"

#include <QMessageBox>
#include <QPixmap>

void MainWindow::slotHelpAbout()
{
    QString version = QString(STR_REVISION).replace(',','.');
    QMessageBox dialog(QMessageBox::NoIcon,
                        tr("About Shatrabase"),
                        tr("<h1>Shatrabase</h1>"
                        "<p>Free Shatra database Version %1</p>"
                        "<p>Derived from Chessx Version 0.9.5</p>"
                        COPYRIGHT
                        "<p>License: GNU Public License 3 - GPLv3</p>"
                        "<p>Developers: <a href=\"mailto:francis@afkm.com\">Francis Monkman</a>"
                        " and <a href=\"mailto:stefan.berke@cymatrix.org\">Stefan Berke</a></p>"
                        "<p>Homepage: <a href=\"https://github.com/franzm/shatrabase\">https://github.com/franzm/shatrabase</a>"
                        "<p>Chessx maintainer: <a href=\"mailto:jens-chessx@gmx.net\">Jens Nissen</a></p>"
                        "<p>Testing, usability and website: Bruno Rizzuti</p>"
                        "<p>Further maintainers: Michal Rudolf, Ejner Borgbjerg</p>"
                        "<p>Additional coding: James Coons, Marius Roets, Sean Estabrooks, Rico Zenklusen, "
                        "Heinz Hopfgartner, William Hoggarth.</p>"
                        "<p>Chessx homepage: <a href=\"http://chessx.sourceforge.net\">http://chessx.sourceforge.net</a><br>")
                        .arg(version),
                QMessageBox::Ok);
    dialog.setIconPixmap((QPixmap(":/images/shatrabase.png")));
    dialog.exec();
}

