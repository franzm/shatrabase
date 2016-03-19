/***************************************************************************
 *                           Shatrabase                                    *
 *   (C) 2012-2014 AFKM <francis@afkm.com>                                 *
 *                   derived from ChessX with thanks                       *
 *   Copyright (C) 2012 by Jens Nissen jens-chessx@gmx.net                 *
 ***************************************************************************/

#include <QUrl>
#include "helpbrowser.h"
#include "settings.h"

HelpBrowser::HelpBrowser(QWidget *parent) :
    QTextBrowser(parent)
{
    setOpenExternalLinks(true);
    loadResource ( QTextDocument::StyleSheetResource, QUrl(":/help/about.css"));

#if 0
    QString lang = AppSettings->getValue("/General/Language").toString().toLower();
    setSource(QUrl(QString("qrc:///help/%1/about0.html").arg(lang)));
#else
    setSource(QUrl("qrc:///help/about0.html"));
#endif
}
