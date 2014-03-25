/***************************************************************************
 *                           Shatrabase                                    *
 *   (C) 2012-2014 AFKM <francis@afkm.com>                                 *
 *                   derived from ChessX with thanks                       *
 *   (C) 2006 William Hoggarth <whoggarth@users.sourceforge.net>           *
 *   (C) 2006-2009 Michal Rudolf <mrudolf@kdewebdev.org>                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "nag.h"

void NagSet::addNag(Nag nag)
{
	if (contains(nag) || nag == NullNag || nag >= NagCount)
		return;
    if (nag >= MoveNagStart && nag <= MoveNagEnd && nag != ForcedMove && nag != SingularMove)
		removeNagRange(MoveNagStart, MoveNagEnd);

    if (conditionalRemoveNagRange(nag, EvaluationNagStart, EvaluationNagEnd))
    {
        removeNag(WhiteHasModerateCounterplay);
    }
    if (conditionalRemoveNagRange(nag, WhiteHasModerateCounterplay, BlackHasModerateCounterplay))
    {
        removeNagRange(EvaluationNagStart, EvaluationNagEnd);
    }
    conditionalRemoveNagRange(nag, BishopNagStart,     BishopNagEnd);
    conditionalRemoveNagRange(nag, OpeningNagStart,    OpeningNagEnd);
    conditionalRemoveNagRange(nag, MiddleGameNagStart, MiddleGameNagEnd);
    conditionalRemoveNagRange(nag, EndingNagStart,     EndingNagEnd);

    append(nag);
}

bool NagSet::conditionalRemoveNagRange(Nag nag, Nag start, Nag end)
{
    if (nag >= start && nag <= end)
    {
        removeNagRange(start, end);
        return true;
    }
    return false;
}

void NagSet::removeNag(Nag nag)
{
	if (nag == NullNag) return;

	int index = indexOf(nag);
	if (index != -1) remove(index);
}

QString NagSet::toString(unsigned format) const
{
	QString moveNag;
	QString evaluationNag;
	QString otherNags;

    for (int i = 0; i < count(); ++i) {
		if (at(i) >= MoveNagStart && at(i) <= MoveNagEnd)
			moveNag.append(format == SGN ? " $" + QString::number(at(i)) : nagToString(at(i)));
		else if (at(i) >= EvaluationNagStart && at(i) <= EvaluationNagEnd)
			evaluationNag.append(format == SGN ? " $" + QString::number(at(i)) : nagToString(at(i)));
		else
			otherNags.append(format == SGN ? " $" + QString::number(at(i)) : nagToString(at(i)));
	}
	return moveNag + evaluationNag + otherNags;
}

static const QString g_nagStringList[NagCount] = {
	"",
	"!",
	"?",
	"!!",
	"??",
	"!?",
	"?!",
	QString::fromUtf8("□"),
	QString::fromUtf8("□"),
	"??",
	"=",
	"=",
	"=",
	QString::fromUtf8("∞"),
	"+=",
	"=+",
	QString::fromUtf8("±"),
	QString::fromUtf8("∓"),
	"+-",
	"-+",
    "++--",
    "--++",
    QString::fromUtf8("⨀"), // Zugzwang
    QString::fromUtf8("⨀"), // Zugzwang
    QObject::tr("with slight space advantage"),
	QObject::tr("with slight space advantage"),
	QObject::tr("with moderate space advantage"),
	QObject::tr("with moderate space advantage"),
	QObject::tr("with decisive space advantage"),
	QObject::tr("with decisive space advantage"),
	QObject::tr("with slight development advantage"),
	QObject::tr("with slight development advantage"),
    QString::fromUtf8("⟳"), // "with moderate development advantage"
    QString::fromUtf8("⟳"), // "with moderate development advantage"
	QObject::tr("with decisive development advantage"),
	QObject::tr("with decisive development advantage"),
    QString::fromUtf8("→"), // "with initiative"
    QString::fromUtf8("→"), // "with initiative"
    QObject::tr("with lasting initiative"),
    QObject::tr("with lasting initiative"),
    QString::fromUtf8("↑"), // "with attack"
    QString::fromUtf8("↑"), // "with attack"
	QObject::tr("with insufficient compensation for material deficit"),
	QObject::tr("with insufficient compensation for material deficit"),
	QString::fromUtf8("∞="),
	QString::fromUtf8("∞="),
	QObject::tr("with more than adequate compensation for material deficit"),
	QObject::tr("with more than adequate compensation for material deficit"),
	QObject::tr("with slight center control advantage"),
	QObject::tr("with slight center control advantage"),
	QObject::tr("with moderate center control advantage"),
	QObject::tr("with moderate center control advantage"),
	QObject::tr("with decisive center control advantage"),
	QObject::tr("with decisive center control advantage"),
	QObject::tr("with slight kingside control advantage"),
	QObject::tr("with slight kingside control advantage"),
	QObject::tr("with moderate kingside control advantage"),
	QObject::tr("with moderate kingside control advantage"),
	QObject::tr("with decisive kingside control advantage"),
	QObject::tr("with decisive kingside control advantage"),
	QObject::tr("with slight queenside control advantage"),
	QObject::tr("with slight queenside control advantage"),
	QObject::tr("with moderate queenside control advantage"),
	QObject::tr("with moderate queenside control advantage"),
	QObject::tr("with decisive queenside control advantage"),
	QObject::tr("with decisive queenside control advantage"),
	QObject::tr("with vulnerable first rank"),
	QObject::tr("with vulnerable first rank"),
	QObject::tr("with well protected first rank"),
	QObject::tr("with well protected first rank"),
	QObject::tr("with poorly protected king"),
	QObject::tr("with poorly protected king"),
	QObject::tr("with well protected king"),
	QObject::tr("with well protected king"),
	QObject::tr("with poorly placed king"),
	QObject::tr("with poorly placed king"),
	QObject::tr("with well placed king"),
	QObject::tr("with well placed king"),
	QObject::tr("with very weak pawn structure"),
	QObject::tr("with very weak pawn structure"),
	QObject::tr("with moderately weak pawn structure"),
	QObject::tr("with moderately weak pawn structure"),
	QObject::tr("with moderately strong pawn structure"),
	QObject::tr("with moderately strong pawn structure"),
	QObject::tr("with very strong pawn structure"),
	QObject::tr("with very strong pawn structure"),
	QObject::tr("with poor knight placement"),
	QObject::tr("with poor knight placement"),
	QObject::tr("with good knight placement"),
	QObject::tr("with good knight placement"),
	QObject::tr("with poor bishop placement"),
	QObject::tr("with poor bishop placement"),
	QObject::tr("with good bishop placement"),
	QObject::tr("with good bishop placement"),
	QObject::tr("with poor rook placement"),
	QObject::tr("with poor rook placement"),
	QObject::tr("with good rook placement"),
	QObject::tr("with good rook placement"),
	QObject::tr("with poor queen placement"),
	QObject::tr("with poor queen placement"),
	QObject::tr("with good queen placement"),
	QObject::tr("with good queen placement"),
	QObject::tr("with poor piece coordination"),
	QObject::tr("with poor piece coordination"),
	QObject::tr("with good piece coordination"),
	QObject::tr("with good piece coordination"),
	QObject::tr("White played the opening very poorly"),
	QObject::tr("Black played the opening very poorly"),
	QObject::tr("White played the opening poorly"),
	QObject::tr("Black played the opening poorly"),
	QObject::tr("White played the opening well"),
	QObject::tr("Black played the opening well"),
	QObject::tr("White played the opening very well"),
	QObject::tr("Black played the opening very well"),
	QObject::tr("White played the middlegame very poorly"),
	QObject::tr("Black played the middlegame very poorly"),
	QObject::tr("White played the middlegame poorly"),
	QObject::tr("Black played the middlegame poorly"),
	QObject::tr("White played the middlegame well"),
	QObject::tr("Black played the middlegame well"),
	QObject::tr("White played the middlegame very well"),
	QObject::tr("Black played the middlegame very well"),
	QObject::tr("White played the ending very poorly"),
	QObject::tr("Black played the ending very poorly"),
	QObject::tr("White played the ending poorly"),
	QObject::tr("Black played the ending poorly"),
	QObject::tr("White played the ending well"),
	QObject::tr("Black played the ending well"),
	QObject::tr("White played the ending very well"),
	QObject::tr("Black played the ending very well"),
	QString::fromUtf8("⇄"),
	QString::fromUtf8("⇄"),
	QString::fromUtf8("⇄"),
	QString::fromUtf8("⇄"),
	QString::fromUtf8("⇄"),
	QString::fromUtf8("⇄"),
	QObject::tr("Moderate time control pressure"),
	QObject::tr("Moderate time control pressure"),
    QString::fromUtf8("⊕"), // "Severe time control pressure"
    QString::fromUtf8("⊕"), // "Severe time control pressure"
	QString::fromUtf8("△"),
	QObject::tr("aimed against"),
    QString::fromUtf8("⌓"), // "Better was"
    "<", // "Worse was"
	QObject::tr("Equivalent was"),
	QObject::tr("RR"),
	QObject::tr("N"),
    QString::fromUtf8("✕"), // "weak point"
    QString::fromUtf8("⊥"), // "endgame"
    QString::fromUtf8("⇔"), // "line"
    QString::fromUtf8("⇗"), // "diagonal",
    QString::fromUtf8("☍"), // "pair of bishops"
    QString::fromUtf8("☍"), // "pair of bishops"
    QString::fromUtf8("❐"), // "bishops of opposite color"
    QString::fromUtf8("▞")  // "bishops of the same color"
};

static const QString g_nagStringListLong[NagCount] = {
    "",
    QObject::tr("Good Move"),
    QObject::tr("Poor Move"),
    QObject::tr("Very good Move"),
    QObject::tr("Blunder"),
    QObject::tr("Speculative Move"),
    QObject::tr("Dubious Move"),
    QObject::tr("Only Move"),
    QObject::tr("Only Move"),
    QObject::tr("Blunder"),
    QObject::tr("Equal"),
    QObject::tr("Equal"),
    QObject::tr("Equal"),
    QObject::tr("Unclear"),
    QObject::tr("White has slight Advantage"),
    QObject::tr("Black has slight Advantage"),
    QObject::tr("White has moderate Advantage"),
    QObject::tr("Black has moderate Advantage"),
    QObject::tr("White has decisive Advantage"),
    QObject::tr("Black has decisive Advantage"),
    QObject::tr("White has crushing Advantage"),
    QObject::tr("Black has crushing Advantage"),
    QObject::tr("Zugzwang"),
    QObject::tr("Zugzwang"),
    QObject::tr("with slight space advantage"),
    QObject::tr("with slight space advantage"),
    QObject::tr("with moderate space advantage"),
    QObject::tr("with moderate space advantage"),
    QObject::tr("with decisive space advantage"),
    QObject::tr("with decisive space advantage"),
    QObject::tr("with slight development advantage"),
    QObject::tr("with slight development advantage"),
    QObject::tr("with moderate development advantage"),
    QObject::tr("with moderate development advantage"),
    QObject::tr("with decisive development advantage"),
    QObject::tr("with decisive development advantage"),
    QObject::tr("with initiative"),
    QObject::tr("with initiative"),
    QObject::tr("with lasting initiative"),
    QObject::tr("with lasting initiative"),
    QObject::tr("with attack"),
    QObject::tr("with attack"),
    QObject::tr("with insufficient compensation for material deficit"),
    QObject::tr("with insufficient compensation for material deficit"),
    QObject::tr("with compensation"),
    QObject::tr("with compensation"),
    QObject::tr("with more than adequate compensation for material deficit"),
    QObject::tr("with more than adequate compensation for material deficit"),
    QObject::tr("with slight center control advantage"),
    QObject::tr("with slight center control advantage"),
    QObject::tr("with moderate center control advantage"),
    QObject::tr("with moderate center control advantage"),
    QObject::tr("with decisive center control advantage"),
    QObject::tr("with decisive center control advantage"),
    QObject::tr("with slight kingside control advantage"),
    QObject::tr("with slight kingside control advantage"),
    QObject::tr("with moderate kingside control advantage"),
    QObject::tr("with moderate kingside control advantage"),
    QObject::tr("with decisive kingside control advantage"),
    QObject::tr("with decisive kingside control advantage"),
    QObject::tr("with slight queenside control advantage"),
    QObject::tr("with slight queenside control advantage"),
    QObject::tr("with moderate queenside control advantage"),
    QObject::tr("with moderate queenside control advantage"),
    QObject::tr("with decisive queenside control advantage"),
    QObject::tr("with decisive queenside control advantage"),
    QObject::tr("with vulnerable first rank"),
    QObject::tr("with vulnerable first rank"),
    QObject::tr("with well protected first rank"),
    QObject::tr("with well protected first rank"),
    QObject::tr("with poorly protected king"),
    QObject::tr("with poorly protected king"),
    QObject::tr("with well protected king"),
    QObject::tr("with well protected king"),
    QObject::tr("with poorly placed king"),
    QObject::tr("with poorly placed king"),
    QObject::tr("with well placed king"),
    QObject::tr("with well placed king"),
    QObject::tr("with very weak pawn structure"),
    QObject::tr("with very weak pawn structure"),
    QObject::tr("with moderately weak pawn structure"),
    QObject::tr("with moderately weak pawn structure"),
    QObject::tr("with moderately strong pawn structure"),
    QObject::tr("with moderately strong pawn structure"),
    QObject::tr("with very strong pawn structure"),
    QObject::tr("with very strong pawn structure"),
    QObject::tr("with poor knight placement"),
    QObject::tr("with poor knight placement"),
    QObject::tr("with good knight placement"),
    QObject::tr("with good knight placement"),
    QObject::tr("with poor bishop placement"),
    QObject::tr("with poor bishop placement"),
    QObject::tr("with good bishop placement"),
    QObject::tr("with good bishop placement"),
    QObject::tr("with poor rook placement"),
    QObject::tr("with poor rook placement"),
    QObject::tr("with good rook placement"),
    QObject::tr("with good rook placement"),
    QObject::tr("with poor queen placement"),
    QObject::tr("with poor queen placement"),
    QObject::tr("with good queen placement"),
    QObject::tr("with good queen placement"),
    QObject::tr("with poor piece coordination"),
    QObject::tr("with poor piece coordination"),
    QObject::tr("with good piece coordination"),
    QObject::tr("with good piece coordination"),
    QObject::tr("White played the opening very poorly"),
    QObject::tr("Black played the opening very poorly"),
    QObject::tr("White played the opening poorly"),
    QObject::tr("Black played the opening poorly"),
    QObject::tr("White played the opening well"),
    QObject::tr("Black played the opening well"),
    QObject::tr("White played the opening very well"),
    QObject::tr("Black played the opening very well"),
    QObject::tr("White played the middlegame very poorly"),
    QObject::tr("Black played the middlegame very poorly"),
    QObject::tr("White played the middlegame poorly"),
    QObject::tr("Black played the middlegame poorly"),
    QObject::tr("White played the middlegame well"),
    QObject::tr("Black played the middlegame well"),
    QObject::tr("White played the middlegame very well"),
    QObject::tr("Black played the middlegame very well"),
    QObject::tr("White played the ending very poorly"),
    QObject::tr("Black played the ending very poorly"),
    QObject::tr("White played the ending poorly"),
    QObject::tr("Black played the ending poorly"),
    QObject::tr("White played the ending well"),
    QObject::tr("Black played the ending well"),
    QObject::tr("White played the ending very well"),
    QObject::tr("Black played the ending very well"),
    QObject::tr("with counterplay"),
    QObject::tr("with counterplay"),
    QObject::tr("with counterplay"),
    QObject::tr("with counterplay"),
    QObject::tr("with counterplay"),
    QObject::tr("with counterplay"),
    QObject::tr("Moderate time control pressure"),
    QObject::tr("Moderate time control pressure"),
    QObject::tr("Severe time control pressure"),
    QObject::tr("Severe time control pressure"),
    QObject::tr("with the idea"),
    QObject::tr("aimed against"),
    QObject::tr("Better was"),
    QObject::tr("Worse was"),
    QObject::tr("Equivalent was"),
    QObject::tr("RR"),
    QObject::tr("N"),
    QObject::tr("weak point"),
    QObject::tr("endgame"),
    QObject::tr("line"),
    QObject::tr("diagonal"),
    QObject::tr("pair of bishops"),
    QObject::tr("pair of bishops"),
    QObject::tr("bishops of opposite color"),
    QObject::tr("bishops of the same color")
};

const QString& NagSet::nagToString(Nag nag)
{
	return g_nagStringList[nag];
}

QString NagSet::nagToMenuString(Nag nag)
{
    if (g_nagStringList[nag].length() <= 2)
    {
        return QString("%1 (%2)").arg(g_nagStringList[nag]).arg(g_nagStringListLong[nag]);
    }
    else
    {
        return NagSet::nagToString(nag);
    }
}

Nag NagSet::fromString(const QString &nag)
{
    for (int i = 1; i < NagCount; ++i)
		if (g_nagStringList[i] == nag)
			return Nag(i);
	return NullNag;
}

int NagSet::prefixCount(const QString &nag)
{
	QSet<QString> matches;
    for (int i = 1; i < NagCount; ++i)
		if (g_nagStringList[i].startsWith(nag))
			matches.insert(g_nagStringList[i]);
	return matches.count();

}

void NagSet::removeNagRange(Nag from, Nag to)
{
	for (int i = count() - 1; i >= 0; i--)
		if (at(i) >= from && at(i) <= to)
			remove(i);
}

