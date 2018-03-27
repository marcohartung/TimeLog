#include "tlData.h"
#include <QtXml>
#include <QMessageBox>

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

    QDomNode node = root.firstChild();
    while( !node.isNull() ){

        if( node.toElement().tagName() == "Info" ){          //TimeLogDataInfo
            QDomElement e = node.firstChild().toElement();
            //d->m_Info.Name = e.attribute( "name" );
            //d->m_Info.id = e.attribute( "id" ).toLongLong();
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
                            QDomAttr aTime = e.attributeNode("time");
                            QDomAttr aTask = e.attributeNode("task");
                            QDomAttr aType = e.attributeNode("type");
                            QTime time;
                            time = QTime::fromString( aTime.value(), Qt::ISODate );

                            worktime_t worktime;
                            worktime.time = time;
                            worktime.task = (TimeTask_t)aTask.value().toInt();
                            worktime.type = (TimeType_t)aTask.value().toInt();
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

bool tlData::WriteXml( const QString& FileName ){
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

    // Save Data
    if( days.size() ){
        QDomElement data = doc.createElement("Data");

        for( int i = 0; i < days.size(); i++ ){
            QDomElement day = doc.createElement("Day");
            day.setAttribute( "date", days[i].date.toString( Qt::ISODate ) );

            for( int n = 0; n < days[i].times.size(); n++ ){
                   QDomElement t = doc.createElement( "Time" );
                   t.setAttribute( "time", days[i].times[n].time.toString( Qt::ISODate )  );
                   t.setAttribute( "type", QString::number( days[i].times[n].type ) );
                   t.setAttribute( "task", QString::number( days[i].times[n].task ) );
                   day.appendChild( t );
            }
            data.appendChild(day);
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


bool tlData::AddTime( QDate date, QTime time, TimeType_t type, TimeTask_t task){
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

    worktime_t t;
    t.time = time;
    t.type = type;
    t.task = task;

    i->times.append( t );
    fModified = true;

    return true;
}
