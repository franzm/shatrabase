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
#include "ushienginetester.h"
#include "playenginetester.h"

int main(int argc, char** argv)
{
	AppSettings = new Settings;
    QApplication a(argc, argv);
/*
    PlayEngineTester t;
    t.run();
    a.exec();
    return 0;*/


#ifdef Q_OS_MAC
    signal(SIGPIPE, SIG_IGN);
#endif

    // quick hack to test engine vs. engine in console
    if (argc>1 && 0==QString(argv[1]).compare("test", Qt::CaseInsensitive))
        return USHIEngineTester::debugTest(a, argc>2? QString(argv[2]) : QString());

    // update execution counter
    AppSettings->setValue("/General/executed",
        AppSettings->getValue("/General/executed").toInt() + 1);

    // --- prepare application ---

    // the common exit condition
    a.connect(&a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()));
    // XXX findout: why is this not the .icn file
    a.setWindowIcon(QIcon(":/images/shatrabase.png"));

    // -- translation --

    QString langFile = QString("shatrabase_%1.qm").arg(AppSettings->getValue("/General/Language").toString());

    QTranslator translator;
    if (translator.load(AppSettings->dataPath() + "/lang/" + langFile) ||
         translator.load(QString(":translation/") + langFile))
		a.installTranslator(&translator);

    // -- "the app" --

	MainWindow* mainWindow = new MainWindow;

	mainWindow->show();

    // -- run --

    startFileLog();

    int result = a.exec();

    stopFileLog();


    // -- cleanup --

	delete AppSettings;

    return result;
}

