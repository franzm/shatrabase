/***************************************************************************

Copyright (C) 2014  stefan.berke @ modular-audio-graphics.com

This source is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either
version 3.0 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License
along with this software; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA

****************************************************************************/

#ifndef PLAYGAMEENGINEDIALOG_H
#define PLAYGAMEENGINEDIALOG_H

#include <QDialog>
#include <QString>
#include <QStringList>
#include <QVariant>
#include <QMap>

namespace Ui { class PlayGameEngineDialog; }

class PlayGame;

class PlayGameEngineDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PlayGameEngineDialog(PlayGame * play, int playerNumber, QWidget *parent = 0);
    ~PlayGameEngineDialog();

public slots:

    /** Get settings from PlayGame */
    void slotReconfigure();


private slots:

    /** Sets the gui values to current settings_ for selected player_ */
    void updateWidgets_();

    /** Store all settings_ values to AppSettings */
    void saveSettings_();


    // --- widget callbacks ---

    void changedPlayer_(int);

    void changedMinTime_(double);
    void changedMaxTime_(double);

protected:

    virtual void showEvent(QShowEvent *);
    virtual void accept();

private:

    Ui::PlayGameEngineDialog *ui_;
    PlayGame * play_;
    int player_;
    QString url_;

    QStringList options_;
    QMap<QString, QVariant> settings_;
};

#endif // PLAYGAMEENGINEDIALOG_H
