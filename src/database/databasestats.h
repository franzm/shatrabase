#ifndef DATABASESTATS_H
#define DATABASESTATS_H

#include <QString>
#include <QStringList>
#include <QVector>
#include <QMap>

class DatabaseModel;

/** A container for statistics about Databases. */
class DatabaseStats
{
public:

    // --------- types --------

    /** Each numerical field in a Game gets it's own Data entry
        It collects min/max/average, a histogram, etc...
      */
    struct Data
    {
        QString key;
        /** vector representing the actual value for each row under the key */
        QVector<int> v;
        /** vector representing the counted frequencies */
        QVector<int> histogram;
        /** smallest value in v */
        int min_value,
            max_value;
        float
        /** average value */
            mean,
        /** ratio of mean in whole value range [0,1] */
            mean_ratio,
        /** standard deviation */
            deviation,
        /** ratio of mean in whole value range [0,1] */
            deviation_ratio,
        /** ratio between overall and unique values [0,1] */
            unique_ratio;
    };


    // -------------- ctor ---------------

    DatabaseStats();

    /** Return if data is present */
    bool ok() const { return ok_; }

    // ------- database handling ---------

    /** Set all internal data to readable entries in the db */
    void setDatabaseModel(const DatabaseModel & db);

    // ------ raw data handling ----------

    /** Clears all */
    void clearData();

    /** Clears specific key */
    void clearData(const QString& key);

    /** Returns the Data struct for the given key, or 0 */
    const Data * data(const QString& key) const;

    /** Returns number of keys (numerical cols in database) */
    unsigned int numKeys() const { return map_.size(); }

    /** Returns the list of all keys (numerical cols in database) */
    QStringList keys() const;

private:
    // _________ PRIVATE FUNCTIONS _____________

    /** Creates a Data entry from a raw data curve.
        Everything for key will be overwritten. */
    Data * createData_(const QString key, const QVector<int>& values);

    /** Sets up Data statistics after Data::v is set */
    void calculateData_(Data &d) const;

    // __________ PRIVATE MEMBER _______________

    /** container of stats for each entry */
    QMap<QString, Data> map_;
    typedef QMap<QString, Data>::Iterator Iter;
    typedef QMap<QString, Data>::ConstIterator ConstIter;

    bool ok_;

};

#endif // DATABASESTATS_H
