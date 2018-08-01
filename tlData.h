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
        TimeTask_t task;
        QTime timeStart;
        QTime timeStop;
        QString TaskName;
        QString TaskSubName;
    };

    struct workday_t {
        QDate date;
        QVector<worktime_t> times;
    };

    struct project_t {
        QString Name;
    };

    tlData();

    bool ReadXml( const QString strfile );
    bool WriteXml( const QString FileName );

    bool AddTime( QDate date, QTime time, TimeType_t type, TimeTask_t task);

    bool AddProject( const QString projName );
    bool DelProject( const QString projName );
    QVector<project_t>& GetProjectList( void );

    void Clear( void );
    //bool WorkingNow();

protected:

private:

    QVector<workday_t> days;
    QVector<project_t> projects;
    bool fModified;


};

#endif // TLDATA_H
