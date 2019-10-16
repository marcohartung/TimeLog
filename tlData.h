/* TimeLog - Copyright (C) 2019  Marco Hartung
 *
 * This file is part of TimeLog.
 *
 * TimeLog is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * TimeLog is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with TimeLog.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef TLDATA_H
#define TLDATA_H

#include <QString>
#include <QDate>
#include <QTime>
#include <QVector>

class tlData
{
public:

    static const QTime invalidTime;

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
        qint64 WorkDaysInvalidData;
        QVector<tasksummery_t> tasks;
    };

    struct worktask_t {
        qint64 id;
        TimeTask_t task;
        QTime timeStart;
        QTime timeStop;
        qint64 timeSpan;
        QString TaskName;
        QString TaskSubName;
    };

    struct worktime_t {
        // TimeTask_t task; // allways enuWork
        qint64 id;
        QTime timeStart;
        QTime timeStop;
        QVector<worktask_t> tasks;
    };

    struct workday_t {
        qint64 id;
        QDate date;
        QVector<worktime_t> times;
    };

    struct project_t {
        QString Name;
    };

    tlData();

    bool SetDataFile( const QString strFileName );

    bool ReadXml( void );
    bool WriteXml( void );

    bool AddTime( QDate date, QTime time,
                  TimeType_t type, TimeTask_t task,
                  QString TaskName = "" , QString TaskSubName = "" );

    bool AddWorkTime( QDate date, tlData::worktime_t worktime );
    bool AddWorkTask( QDate date, tlData::worktask_t task );

    bool UpdateWorkTime( worktime_t worktime );
    bool UpdateWorkTask( worktask_t task );

    bool AddProject( const QString projName );
    bool DelProject( const QString projName );
    QVector<project_t>& GetProjectList( void );

    void Clear( void );

    QVector<worktime_t> GetWorktimesOfDay( QDate date );

    WorkSummery_t GetWorktimeSummery( QDate StartDate, QDate EndDate, bool IgnoreSubTasks = true );

protected:

private:

    QString strDataBaseFileName;

    qint64 nextDayId;
    qint64 nextWorkTimeId;
    qint64 nextTaskId;
    QVector<workday_t> days;
    QVector<project_t> projects;

};

#endif // TLDATA_H
