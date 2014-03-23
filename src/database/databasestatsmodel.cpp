#include "databasestatsmodel.h"
#include "databasestats.h"

#include <QDebug>
#include <QColor>


DatabaseStatsModel::DatabaseStatsModel(const DatabaseStats * stats, QObject *parent)
    :   QAbstractItemModel(parent),
        stats_  (stats)
{
    names_
    << tr("property")
    << tr("min")
    << tr("max")
    << tr("mean")
    << tr("deviation")
    << tr("unique values");
}


QModelIndex DatabaseStatsModel::index(int row, int column, const QModelIndex &parent) const
{
    if (parent.isValid()) return QModelIndex();

    return createIndex(row, column);
}

QModelIndex DatabaseStatsModel::parent(const QModelIndex &child) const
{
    Q_UNUSED(child);

    return QModelIndex();
}

int DatabaseStatsModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) return 0;

    return stats_->numKeys();
}


int DatabaseStatsModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid()) return 0;

    return names_.size();
}

QVariant DatabaseStatsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal)
        return (section < names_.size())? names_[section] : "-";
    else
        return QString::number(section);
}


QVariant DatabaseStatsModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < (int)stats_->numKeys()
        && index.column() < names_.size())
    {

        if (keys_.size() != (int)stats_->numKeys())
            keys_ = stats_->keys();

        const int i = index.row();
        const DatabaseStats::Data * d = stats_->data(keys_[i]);

        if (role == Qt::DisplayRole && d)
        {
            switch (index.column())
            {
                case 0: return d->key;
                case 1: return d->min_value;
                case 2: return d->max_value;
                case 3: return QString("%1 (%2%)").arg(d->mean).arg((int)(d->mean_ratio*100));
                case 4: return QString("%1 (%2%)").arg(d->deviation).arg((int)(d->deviation_ratio*100));
                case 5: return QString("%1 (%2%)").arg(d->histogram.size()).arg((int)(d->unique_ratio*100));
            }
        }
    }
    return QVariant();
}
