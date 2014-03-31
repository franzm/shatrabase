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

#include "playgameenginedialog.h"
#include "ui_playgameenginedialog.h"
#include "playgame.h"
#include "playgameengine.h"
#include "settings.h"


PlayGameEngineDialog::PlayGameEngineDialog(PlayGame * play, int player, QWidget *parent) :
    QDialog (parent),
    ui_     (new Ui::PlayGameEngineDialog),
    play_   (play),
    player_ (player)
{
    ui_->setupUi(this);

    connect(ui_->cb_player, SIGNAL(activated(int)), SLOT(changedPlayer_(int)));
    connect(ui_->sb_mintime, SIGNAL(valueChanged(double)), SLOT(changedMinTime_(double)));
    connect(ui_->sb_maxtime, SIGNAL(valueChanged(double)), SLOT(changedMaxTime_(double)));

    for (int i=1; i<=2; ++i)
    {
        ui_->cb_player->addItem(tr("Player %1").arg(i), QVariant(i-1));
    }

    options_.append("engine");
    options_.append("name");
    options_.append("minTime");
    options_.append("maxTime");
}

PlayGameEngineDialog::~PlayGameEngineDialog()
{
    delete ui_;
}



void PlayGameEngineDialog::slotReconfigure()
{
    // read options for all players
    for (int j=1; j<=2; ++j)
    {
        QString url = QString("/PlayGame/Player%1/").arg(j);

        for (QStringList::iterator i=options_.begin(); i!=options_.end(); ++i)
        {
            settings_.insert(url + *i, AppSettings->getValue(url + *i));
        }
    }
}

void PlayGameEngineDialog::updateWidgets_()
{
    // set current gui elements

    ui_->sb_mintime->setValue( settings_[url_ + "minTime"].toDouble() / 1000.0 );
    ui_->sb_maxtime->setValue( settings_[url_ + "maxTime"].toDouble() / 1000.0 );
}

void PlayGameEngineDialog::saveSettings_()
{
    // save options for all players
    for (int j=1; j<=2; ++j)
    {
        QString url = QString("/PlayGame/Player%1/").arg(j);

        for (QStringList::iterator i=options_.begin(); i!=options_.end(); ++i)
        {
            AppSettings->setValue( url + *i, settings_[url + *i]);
        }
    }

}

void PlayGameEngineDialog::showEvent(QShowEvent *)
{
    slotReconfigure();

    ui_->cb_player->setCurrentIndex(player_);
    changedPlayer_(player_);
}

void PlayGameEngineDialog::accept()
{
    saveSettings_();
    QDialog::accept();
}

void PlayGameEngineDialog::changedPlayer_(int p)
{
    player_ = p;
    url_ = QString("/PlayGame/Player%1/").arg(player_+1);
    updateWidgets_();
}

void PlayGameEngineDialog::changedMinTime_(double v)
{
    settings_[url_ + "minTime"].setValue((int)(v * 1000));
}

void PlayGameEngineDialog::changedMaxTime_(double v)
{
    settings_[url_ + "maxTime"].setValue((int)(v * 1000));
}
