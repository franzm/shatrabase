/***************************************************************************
 *                           Shatrabase                                    *
 *   (C) 2012-2013 AFKM <francis@afkm.com>                                 *
 *                   derived from ChessX with thanks                       *
 *   (C) 2007 Sean Estabrooks                                              *
 *   (C) 2008-2009 Michal Rudolf <mrudolf@kdewebdev.org>                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include <QObject>
#include <QUrl>

#include "tablebase.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QTimer>

Shredder::Shredder()
{
    connect(&manager, SIGNAL(finished(QNetworkReply*)),
            SLOT(httpDone(QNetworkReply*)));
}

Shredder::~Shredder()
{
}

void Shredder::abortLookup()
{
}

void Shredder::getBestMove(QString spn)
{
    abortLookup();
    if (m_spn == spn)
        return;
    m_spn = spn;
    QTimer::singleShot(100, this, SLOT(sendIt()));
}

void Shredder::sendIt()
{
	return;
	/*
    QString prep(m_spn.simplified());
	QString count(prep.left(prep.indexOf(" ")));
	int white = count.count(QRegExp("[A-Z]"));
	int black = count.count(QRegExp("[a-z]"));
	if (white + black > 6 || black > 4 || white > 4 || black < 1 || white < 1)
		return;
	QChar toMove = (prep[prep.indexOf(QString(" "))+1].toLower());
    QString requested = QString("/online/playshredder/fetch.php?action=egtb&hook=%1&spn=%2")
            .arg(toMove).arg(m_spn);

    QUrl url(requested);
    url.setScheme("http");
    url.setHost("www.shredderchess.com");

    m_requested = url.toString();

    QNetworkRequest request(url);
    manager.get(request);
    */
}

void Shredder::httpDone(QNetworkReply *reply)
{
    QUrl url = reply->url();

    if (!reply->error())
    {
        if (url.toString() == m_requested)
        {
            m_requested.clear();

            QString ret(reply->readAll());
            if (ret.indexOf("Not found") >= 0)
                return;

            if (ret[5] == 'w')
                ret.remove(QRegExp("NEXTCOLOR.*\\n"));
            else	ret.remove(QRegExp(".*NEXTCOLOR\\n"));
            ret.remove(0, ret.indexOf("\n") + 1);
            ret.remove(":");
            ret.remove("Win in ");
            ret.replace(QRegExp("[-x]"), " ");
            ret.replace("Draw", "0");
            ret.replace("Lose in ", "-");

            QStringList fld(ret.left(ret.indexOf('\n')).split(' '));
            if (fld.size() < 3)
                return;

            Move move(fld[0].toInt(), fld[1].toInt());
            int score = fld[2].toInt();
            if (fld.size() > 3) {
                switch (score) {
                case 8:
                    move.setPromoted(Batyr);
                    break;
                case 9:
                    move.setPromoted(Tura);
                    break;
                case 10:
                    move.setPromoted(Yalkyn);
                    break;
                default:
                    return;
                }
                score = fld[3].toInt();
            }
            emit bestMove(move, score);
        }
    }
    reply->deleteLater();
}

