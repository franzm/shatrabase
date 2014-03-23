#include "statistics.h"
#include "ui_statistics.h"
#include "databasestats.h"
#include "databasestatsmodel.h"
#include "histogram.h"

#include <QBoxLayout>
#include <QTableView>
#include <QListView>

Statistics::Statistics(QWidget *parent)
    :   QWidget         (parent),
        ui_             (new Ui::Statistics),
        stats_          (new DatabaseStats),
        statsModel_     (new DatabaseStatsModel(stats_, this)),
        histogram_      (0)
{
    setObjectName("DatabaseStats");
    setWindowTitle(tr("Database Statistics"));

    ui_->setupUi(this);

    //ui_->table->setModel(statsModel_);
    //ui_->table->set

    QVBoxLayout * l = new QVBoxLayout(ui_->tab_2);
    histogram_ = new Histogram(ui_->tab_2);
    l->addWidget(histogram_);
    l->setContentsMargins(1,1,1,1);
}

Statistics::~Statistics()
{
    delete ui_;
    delete stats_;
}


void Statistics::setDatabaseModel(const DatabaseModel &db)
{
    stats_->setDatabaseModel(db);
    ui_->table->setModel(statsModel_);
    histogram_->setDatabaseModel(db);
    update();
}

