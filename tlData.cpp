#include "tlData.h"
#include <QtXml>
#include <QMessageBox>


const QTime tlData::invalidTime( QTime(0, 0, 0, 0) );


tlData::tlData()
{

}

bool tlData::ReadXml( const QString strfile ){
    QDomDocument doc;
    QString errorStr;
    int errorLine;
    int errorColumn;

    QFile file( strfile );

    if( !doc.setContent( &file, true, &errorStr, &errorLine, &errorColumn) ){
        QMessageBox::warning( 0, QObject::tr("DOM Parser"),
                              QObject::tr("Parse error at line %1, column %2:\n%3")
                              .arg(errorLine)
                              .arg(errorColumn)
                              .arg(errorStr));
        return false;
    }

    QDomElement root = doc.documentElement();
    if( root.tagName() != "TimeLogData" )
        return false;

    days.clear();
    projects.clear();

    QDomNode node = root.firstChild();
    while( !node.isNull() ){

        if( node.toElement().tagName() == "Info" ){          //TimeLogDataInfo
            QDomElement e = node.firstChild().toElement();
            //d->m_Info.Name = e.attribute( "name" );
            //d->m_Info.id = e.attribute( "id" ).toLongLong();
        }
        else if( node.toElement().tagName() == "Project" ){     //Parse Project List
            AddProject( node.toElement().attributeNode("Name").value() );
        }
        else if( node.toElement().tagName() == "Data" ){     //Parse WorkDay's
            QDomNode DataNode = node.firstChild();

            while( DataNode.toElement().tagName() == "Day" ){
                workday_t workday;
                QDomAttr aDate = DataNode.toElement().attributeNode("date");
                workday.date = QDate::fromString( aDate.value(), Qt::ISODate );;

                QDomNode childNode = DataNode.firstChild();

                while (!childNode.isNull()) {
                    QDomElement e = childNode.toElement(); // try to convert the node to an element.
                    if(!e.isNull()) {
                        if( e.tagName() == "Time" ){
                            QDomAttr aTimeStart = e.attributeNode("timeStart");
                            QDomAttr aTimeStop = e.attributeNode("timeStop");
                            QDomAttr aTask = e.attributeNode("task");
                            QTime time;

                            worktime_t worktime;
                            time = QTime::fromString( aTimeStart.value(), Qt::ISODate );
                            worktime.timeStart = time;
                            time = QTime::fromString( aTimeStop.value(), Qt::ISODate );
                            worktime.timeStop = time;
    //                        worktime.task = (TimeTask_t)aTask.value().toInt();

    //                        if( e.hasAttribute( "taskName" ) ){
    //                            worktime.TaskName = e.attribute( "taskName" );
    //                        }
    //                        if( e.hasAttribute( "taskSubName" ) ){
    //                            worktime.TaskSubName = e.attribute( "taskSubName" );
    //                        }

                            //worktime.type = (TimeType_t)aTask.value().toInt();



  ///////////////////////////////////////////////

                            QDomNode taskNode = childNode.firstChild();

                            while (!taskNode.isNull()) {
                                QDomElement e = taskNode.toElement(); // try to convert the node to an element.
                                if(!e.isNull()) {
                                    if( e.tagName() == "Task" ){
                                        worktask_t worktask;
                                        worktask.timeSpan = -1;
                                        worktask.timeStart = invalidTime;
                                        worktask.timeStop = invalidTime;

                                        if( e.hasAttribute( "timeSpan" ) ){
                                            worktask.timeSpan = e.attribute( "timeSpan" ).toInt();
                                        }
                                        else{
                                            worktask.timeStart = QTime::fromString( e.attribute("timeStart"), Qt::ISODate );
                                            worktask.timeStop =  QTime::fromString( e.attribute("timeStop"), Qt::ISODate );
                                        }
                                        worktask.task = (TimeTask_t)e.attribute("task").toInt();

                                        if( e.hasAttribute( "taskName" ) ){
                                            worktask.TaskName = e.attribute( "taskName" );
                                        }
                                        if( e.hasAttribute( "taskSubName" ) ){
                                            worktask.TaskSubName = e.attribute( "taskSubName" );
                                        }

                                        worktime.tasks.push_back( worktask );
                                    }
                                }
                                taskNode = taskNode.nextSibling();
                            }

  ///////////////////////////////////////////////



                            workday.times.push_back( worktime );
                        }
                    }
                    childNode = childNode.nextSibling();
                }

                days.push_back( workday );
                DataNode = DataNode.nextSibling();
            }

        }
        node = node.nextSibling();
    }

    //    d->setModified( false );

    return true;
}

bool tlData::WriteXml( const QString FileName ){
    const int Indent = 4;
    QDomDocument doc;
    doc.createProcessingInstruction( "xml", "version=\"1.0\" encoding=\"UTF-8\"" );
    QDomElement root = doc.createElement("TimeLogData");
    root.setAttribute( "version", "1.00" );
    doc.appendChild( root );

    // Save TimeLogDataInfo
//    QDomElement info = doc.createElement("Info");
//	info.setAttribute( "name", d.m_Info.Name );
//	info.setAttribute( "id", QString::number( d.m_Info.id ) );
//    root.appendChild( info );

    // save Project list
    for( int i = 0; i < projects.size(); i++ ){
        QDomElement proj = doc.createElement("Project");
        proj.setAttribute( "Name", projects[i].Name );
        root.appendChild(proj);
    }

    // Save Data
    if( days.size() ){
        QDomElement data = doc.createElement("Data");

        // save workdays
        for( int i = 0; i < days.size(); i++ ){
            QDomElement day = doc.createElement("Day");
            day.setAttribute( "date", days[i].date.toString( Qt::ISODate ) );

            for( int n = 0; n < days[i].times.size(); n++ ){
                   QDomElement tim = doc.createElement( "Time" );
                   tim.setAttribute( "timeStart", days[i].times[n].timeStart.toString( Qt::ISODate )  );
                   tim.setAttribute( "timeStop", days[i].times[n].timeStop.toString( Qt::ISODate )  );

                   // save tasks
                   for( int t = 0; t < days[i].times[n].tasks.size(); t++ ){
                          QDomElement tsk = doc.createElement( "Task" );
                          if( days[i].times[n].tasks[t].timeSpan > 0 ){
                            tsk.setAttribute( "timeSpan", QString::number( days[i].times[n].tasks[t].timeSpan ) );
                          }
                          else{
                            tsk.setAttribute( "timeStart", days[i].times[n].tasks[t].timeStart.toString( Qt::ISODate )  );
                            tsk.setAttribute( "timeStop", days[i].times[n].tasks[t].timeStop.toString( Qt::ISODate )  );
                          }
                            tsk.setAttribute( "task", QString::number( days[i].times[n].tasks[t].task ) );

                          if( !days[i].times[n].tasks[t].TaskName.isEmpty() ){
                              tsk.setAttribute( "taskName", days[i].times[n].tasks[t].TaskName );
                          }
                          if( !days[i].times[n].tasks[t].TaskSubName.isEmpty() ){
                              tsk.setAttribute( "taskSubName", days[i].times[n].tasks[t].TaskSubName );
                          }

                          tim.appendChild( tsk );
                   }

                   day.appendChild( tim );
            }
            data.appendChild( day );
        }
        root.appendChild( data );
    }

    QFile file( FileName  );
    file.open( QIODevice::ReadWrite );
    QTextStream out(&file);
    doc.save(out, Indent);
    file.close();

    fModified = false;

    return true;
}


bool tlData::AddTime( QDate date, QTime time,
                      TimeType_t type, TimeTask_t task,
                      QString TaskName /* = "" */ , QString TaskSubName /* = "" */ )
{
    QVector<workday_t>::iterator i;

    // find day
    for( i = days.begin(); i < days.end(); i++ ){
        if( i->date == date ){
            break;
        }
    }

    if( i == days.end() ){
        workday_t d;
        d.date = date;
        days.append( d );
        i = days.end() - 1;
    }

    // work time
    if( task == enuWork ){
        if( type == enuStart ){
            worktime_t t;
            t.timeStart = time;
            t.timeStop =  invalidTime;

            i->times.append( t );
        }
        else if( type == enuStop ){
            if( i->times.size() == 0 ){
                // TODO No Time found
            }
            else{
                i->times.last().timeStop = time;
                //ii->timeStop = time;
                // TODO set stop time for all sub task here
            }
        }

    }
    else{ // work task

        if( i->times.size() == 0 ){
            // TODO No Time found perhaps start new worktime here
        }
        else{

            if( type == enuStart ){
                worktask_t t;
                t.timeStart = time;
                t.timeStop =  invalidTime;
                t.timeSpan = -1;
                t.task = task;
                if( TaskName != "" ){
                    t.TaskName = TaskName;
                }
                if( TaskSubName != "" ){
                    t.TaskSubName = TaskSubName;
                }

                i->times.last().tasks.append( t );

            }
            else if( type == enuStop ){
                // todo check if task exists timeStop is invalide
                i->times.last().tasks.last().timeStop = time;
            }
            else if( type == enuSpan ){
                worktask_t t;
                t.timeStart = invalidTime;
                t.timeStop =  invalidTime;
                t.timeSpan = time.msecsSinceStartOfDay() / 1000;
                t.task = task;
                if( TaskName != "" ){
                    t.TaskName = TaskName;
                }
                if( TaskSubName != "" ){
                    t.TaskSubName = TaskSubName;
                }

                i->times.last().tasks.append( t );
            }
        }
    }

    fModified = true;

    return true;
}

bool tlData::AddProject( const QString projName ){
    bool ret = false;
    QVector<project_t>::iterator i;

    for( i = projects.begin(); i < projects.end(); i++ ){
        if( i->Name == projName ){
            break;
        }
    }

    if( i == projects.end() ){
        project_t p;
        p.Name = projName;
        projects.append( p );
        fModified = true;
        ret = true;
    }

    return ret;
}

bool tlData::DelProject( const QString projName ){
    bool ret = false;
    QVector<project_t>::iterator i;

    for( i = projects.begin(); i < projects.end(); i++ ){
        if( i->Name == projName ){
            break;
        }
    }

    if( i < projects.end() ){
        projects.erase( i );
        ret = true;
    }

    return ret;
}

QVector<tlData::project_t>& tlData::GetProjectList( void ){
    return projects;
}

void tlData::Clear( void ){

    days.clear();
    fModified = true;
}

QVector<tlData::worktime_t> tlData::GetWorktimesOfDay( QDate date ){
    QVector<worktime_t> retWorktimes;
    QVector<workday_t>::iterator i;

    for( i = days.begin(); i < days.end(); i++ ){
        if( i->date == date ){
            break;
        }
    }

    if( i < days.end() ){
        retWorktimes = i->times;
    }

    return retWorktimes;
}

tlData::WorkSummery_t tlData::GetWorktimeSummery( QDate StartDate, QDate EndDate, bool IgnoreSubTasks /* = true */ ){
    WorkSummery_t ws;
    QVector<worktime_t>::iterator i;
    QVector<worktask_t>::iterator ii;
    QVector<tasksummery_t>::iterator ts_i;

    ws.TimeBreak_sec = 0;
    ws.TimeWork_sec = 0;
    ws.WorkDays = 0;

    for( QDate date = StartDate; date <= EndDate; date = date.addDays(1) ){
        QVector<tlData::worktime_t> workday = GetWorktimesOfDay( date );

        if( !workday.empty() ){
            ws.WorkDays++;
        }

        for( i = workday.begin(); i < workday.end(); i++ ){

            ws.TimeWork_sec += (i->timeStop.msecsSinceStartOfDay() - i->timeStart.msecsSinceStartOfDay() ) / 1000;

            for( ii = i->tasks.begin(); ii < i->tasks.end(); ii++ ){

                if( ii->task == enuBreak ){
                    if( ii->timeSpan > 0 ){
                        ws.TimeBreak_sec += ii->timeSpan;
                    }
                    else{
                        ws.TimeBreak_sec += (ii->timeStop.msecsSinceStartOfDay() - ii->timeStart.msecsSinceStartOfDay() ) / 1000.0;
                    }
                }
                else{
                    for( ts_i = ws.tasks.begin(); ts_i < ws.tasks.end(); ts_i++ ){
                        if(  ts_i->task == ii->task &&
                             ts_i->TaskName == ii->TaskName &&
                             ( IgnoreSubTasks || ts_i->TaskSubName == ii->TaskSubName ) )
                        {
                            break;
                        }
                    }

                    if( ts_i == ws.tasks.end() ){
                        tasksummery_t newentry;
                        newentry.task = ii->task;
                        newentry.TaskName = ii->TaskName;
                        if( !IgnoreSubTasks ){
                            newentry.TaskSubName = ii->TaskSubName;
                        }
                        newentry.time_sec = 0;
                        ws.tasks.push_back( newentry );
                        ts_i = ws.tasks.end() - 1;
                    }
                    if( ii->timeSpan > 0 ){
                        ts_i->time_sec += ii->timeSpan;
                    }
                    else{
                        ts_i->time_sec += (ii->timeStop.msecsSinceStartOfDay() - ii->timeStart.msecsSinceStartOfDay() ) / 1000.0;
                    }
                }
            }
        }
    }

    // worktime did not include break time
    ws.TimeWork_sec -= ws.TimeBreak_sec;

    return ws;
}
