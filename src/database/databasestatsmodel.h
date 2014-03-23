#ifndef DATABASESTATSMODEL_H
#define DATABASESTATSMODEL_H

#include <QAbstractItemModel>
#include <QStringList>

class DatabaseStats;

/** Model to display DatabaseStats in a TableView */
class DatabaseStatsModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit DatabaseStatsModel(const DatabaseStats * stats, QObject *parent = 0);

    // --- itemmodel interface impl. ---

    virtual QModelIndex index(int row, int column, const QModelIndex &parent) const;
    virtual QModelIndex parent(const QModelIndex &child) const;
    virtual int rowCount(const QModelIndex &parent) const;
    virtual int columnCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;

    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;

signals:

public slots:

private:
    const DatabaseStats * stats_;

    QStringList names_;
    mutable QStringList keys_;
};

#endif // DATABASESTATSMODEL_H
