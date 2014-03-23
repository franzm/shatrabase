#ifndef STATISTICS_H
#define STATISTICS_H

#include <QWidget>

class DatabaseModel;
class DatabaseStats;
class DatabaseStatsModel;
class Histogram;
class TableView;

namespace Ui { class Statistics; }

/** A view of numerical properties of a Database and it's games */
class Statistics : public QWidget
{
    Q_OBJECT

public:
    explicit Statistics(QWidget *parent = 0);
    ~Statistics();


    /** Gathers statistics from the database.
        The data is created and the object is not longer needed. */
    void setDatabaseModel(const DatabaseModel & db);


private:
    Ui::Statistics *ui_;

    TableView * table_;

    DatabaseStats * stats_;
    DatabaseStatsModel * statsModel_;

    Histogram * histogram_;
};

#endif // STATISTICS_H
