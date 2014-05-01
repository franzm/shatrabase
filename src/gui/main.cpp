/***************************************************************************
 *                           Shatrabase                                    *
 *   (C) 2012-2014 AFKM <francis@afkm.com>                                 *
 *                   derived from ChessX with thanks                       *
 *	 (C) 2005 Michal Rudolf <mrudolf@kdewebdev.org>                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QtPlugin>

#include "mainwindow.h"
#include "settings.h"
#include "logstream.h"

// debug
// #include "boardview.h"
// #include "ushiengine.h"
//#include "ushienginetester.h"
int main(int argc, char** argv)
{
	AppSettings = new Settings;
    QApplication a(argc, argv);

    //return USHIEngineTester::debugTest(a);

    AppSettings->setValue("/General/executed",
        AppSettings->getValue("/General/executed").toInt() + 1);

    a.setWindowIcon(QIcon(":/images/shatrabase.png"));

#ifdef Q_OS_MAC
    signal(SIGPIPE, SIG_IGN);
#endif

    QString langFile = QString("shatrabase_%1.qm").arg(AppSettings->getValue("/General/Language").toString());

    QTranslator translator;
    if (translator.load(AppSettings->dataPath() + "/lang/" + langFile) ||
         translator.load(QString(":translation/") + langFile))
		a.installTranslator(&translator);

	MainWindow* mainWindow = new MainWindow;

	mainWindow->show();

	// Destroy main window and close application
	a.connect(&a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()));

    startFileLog();
	int result = a.exec();

    stopFileLog();

	delete AppSettings;
	return result;
}

