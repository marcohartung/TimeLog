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
                            worktime.task = (TimeTask_t)aTask.value().toInt();

                            if( e.hasAttribute( "taskName" ) ){
                                worktime.TaskName = e.attribute( "taskName" );
                            }
                            if( e.hasAttribute( "taskSubName" ) ){
                                worktime.TaskSubName = e.attribute( "taskSubName" );
                            }

                            //worktime.type = (TimeType_t)aTask.value().toInt();
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
                   QDomElement t = doc.createElement( "Time" );
                   t.setAttribute( "timeStart", days[i].times[n].timeStart.toString( Qt::ISODate )  );
                   t.setAttribute( "timeStop", days[i].times[n].timeStop.toString( Qt::ISODate )  );
                   //t.setAttribute( "type", QString::number( days[i].times[n].type ) );
                   t.setAttribute( "task", QString::number( days[i].times[n].task ) );

                   if( !days[i].times[n].TaskName.isEmpty() ){
                       t.setAttribute( "taskName", days[i].times[n].TaskName );
                   }
                   if( !days[i].times[n].TaskSubName.isEmpty() ){
                       t.setAttribute( "taskSubName", days[i].times[n].TaskSubName );
                   }

                   day.appendChild( t );
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

    if( type == enuStart ){
        worktime_t t;
        t.timeStart = time;
        t.timeStop =  invalidTime;
        t.task = task;
        if( TaskName != "" ){
            t.TaskName = TaskName;
        }
        if( TaskSubName != "" ){
            t.TaskSubName = TaskSubName;
        }

        i->times.append( t );
    }

    else if( type == enuStop ){
        QVector<worktime_t>::iterator ii;
        for( ii = i->times.begin(); ii < i->times.end(); ii++ ){
            if( ii->task == task && ii->timeStop == invalidTime ){
                break;
            }
        }
        if( ii == i->times.end() ){
            // TODO No Time found
        }
        else{
            ii->timeStop = time;
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
