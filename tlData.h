#ifndef TLDATA_H
#define TLDATA_H

#include <QString>
#include <QDate>
#include <QTime>
#include <QVector>

class tlData
{
public:

    enum TimeType_t {
        enuStart,
        enuStop,
    };

    enum TimeTask_t {
        enuWork,
        enuBreak,
        enuProject,
    };

    struct worktime_t {
         QTime time;
         TimeType_t type;
         TimeTask_t task;
    };

    struct workday_t {
        QDate date;
        QVector<worktime_t> times;
    };

    tlData();

    bool ReadXml( QString strfile );
    bool WriteXml( const QString& FileName );

    bool AddTime( QDate date, QTime time, TimeType_t type, TimeTask_t task);

    //bool WorkingNow();

protected:

private:

    QVector<workday_t> days;
    bool fModified;


};

#endif // TLDATA_H
