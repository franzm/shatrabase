#include "databasestats.h"
#include "databasemodel.h"

#include <QDebug>

#include <cmath>

DatabaseStats::DatabaseStats()
    :   ok_ (false)
{
}


void DatabaseStats::clearData()
{
    ok_ = false;
    map_.clear();
}

void DatabaseStats::clearData(const QString& key)
{
    Iter i = map_.find(key);
    if (i != map_.end())
    {
        map_.erase(i);
    }
    ok_ = !map_.empty();
}

const DatabaseStats::Data *
DatabaseStats::data(const QString &key) const
{
    ConstIter i = map_.find(key);
    if (i != map_.end())
        return &i.value();
    return 0;
}

QStringList DatabaseStats::keys() const
{
    QStringList keys;
    for (ConstIter i = map_.begin(); i!=map_.end(); ++i)
        keys << i.key();
    return keys;
}

/* XXX Might want to use direct access to Database someday. */
void DatabaseStats::setDatabaseModel(const DatabaseModel & db)
{
    clearData();

    int rows = db.rowCount(QModelIndex());
    if (rows)
    for (int j = 0; j<db.columnCount(QModelIndex()); ++j)
    {
        // determine if numerical column
        const QVariant v = db.data(db.index(0,j,QModelIndex()), Qt::DisplayRole);
        bool ok;
        v.toInt(&ok);
        if (ok)
        {
            QString key = db.headerData(j, Qt::Horizontal, Qt::DisplayRole).toString();

            // XXX quick hack, won't work with translations
            // don't add these
            if (   key.compare("nr", Qt::CaseInsensitive) == 0
                || key.compare("date", Qt::CaseInsensitive) == 0)
                continue;

            // get vector from database game
            QVector<int> vec;
            for (int i=0; i<rows; ++i)
            {
                vec.push_back
                (
                    db.data(db.index(i,j,QModelIndex()), Qt::DisplayRole).toInt()
                );
            }

            // create data entry
            createData_(key, vec);
        }
    }
}


DatabaseStats::Data *
DatabaseStats::createData_(const QString key, const QVector<int>& values)
{
    Iter i = map_.find(key);

    // create or reuse entry
    if (i == map_.end())
    {
        i = map_.insert(key, Data());
    }

    i.value().v = values;
    i.value().key = key;
    calculateData_(i.value());

    ok_ = true;

    return &(i.value());
}

void DatabaseStats::calculateData_(Data &d) const
{
    // reset Data struct
    d.min_value = d.max_value
        = d.mean = d.deviation = 0;

    if (!d.v.size()) return;

    // ----- mean & dev ------

    // get min / max / average
    d.max_value = d.min_value = d.mean = d.v[0];
    for (int i=1; i<d.v.size(); ++i)
    {
        d.max_value = std::max((int)d.max_value, d.v[i]);
        d.min_value = std::min((int)d.min_value, d.v[i]);
        d.mean += d.v[i];
    }
    d.mean /= d.v.size();

    // get deviation
    for (int i=0; i<d.v.size(); ++i)
    {
        d.deviation += (d.v[i] - d.mean) * (d.v[i] - d.mean);
    }
    d.deviation = std::sqrt(d.deviation / d.v.size());

    // ----- histogram -----

    // use a map to count each occurance
    std::map<int,int> set;
    for (int i=0; i<d.v.size(); ++i)
    {
        set[d.v[i]]++;
    }

    // transfer frequency part
    d.histogram.resize(set.size());
    std::map<int,int>::const_iterator k = set.begin();
    for (size_t i=0; i<set.size(); ++i, ++k)
        d.histogram[i] = k->second;
}
