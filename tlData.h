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
        enuSpan,
    };

    enum TimeTask_t {
        enuWork,
        enuBreak,
        enuProject,
    };

    struct tasksummery_t {
        TimeTask_t task;
        QString TaskName;
        QString TaskSubName;
        qint64 time_sec;
    };

    struct WorkSummery_t {
        qint64 TimeWork_sec;
        qint64 TimeBreak_sec;
        qint64 WorkDays;
        QVector<tasksummery_t> tasks;
    };

    struct worktask_t {
        TimeTask_t task;
        QTime timeStart;
        QTime timeStop;
        qint64 timeSpan;
        QString TaskName;
        QString TaskSubName;
    };

    struct worktime_t {
        // TimeTask_t task; // allways enuWork
        QTime timeStart;
        QTime timeStop;
        QVector<worktask_t> tasks;
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

    bool AddTime( QDate date, QTime time,
                  TimeType_t type, TimeTask_t task,
                  QString TaskName = "" , QString TaskSubName = "" );

    bool AddProject( const QString projName );
    bool DelProject( const QString projName );
    QVector<project_t>& GetProjectList( void );

    void Clear( void );

    QVector<worktime_t> GetWorktimesOfDay( QDate date );

    WorkSummery_t GetWorktimeSummery( QDate StartDate, QDate EndDate, bool IgnoreSubTasks = true );

protected:

private:

    static const QTime invalidTime;

    QVector<workday_t> days;
    QVector<project_t> projects;
    bool fModified;


};

#endif // TLDATA_H
