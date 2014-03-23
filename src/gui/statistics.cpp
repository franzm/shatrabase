#include "statistics.h"
#include "ui_statistics.h"
#include "databasestats.h"
#include "databasestatsmodel.h"
#include "histogram.h"
#include "tableview.h"

#include "databasemodel.h"

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

    table_ = new TableView(this);
    ui_->tab->layout()->addWidget(table_);

    histogram_ = new Histogram(ui_->tab_2);
    ui_->tab_2->layout()->addWidget(histogram_);
}

Statistics::~Statistics()
{
    delete ui_;
    delete stats_;
}


void Statistics::setDatabaseModel(const DatabaseModel &db)
{
    table_->setModel(0);
    stats_->setDatabaseModel(db);
    table_->setModel(statsModel_);

    histogram_->setDatabaseModel(db);
    update();
}

