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

#include "tltools.h"
#include <QTime>
#include <QDir>

tlTools::tlTools()
{

}


QString tlTools::formatWorkTime( qint64 time ){
    return QString::asprintf( "%02lld:%02lld:%02lld", time / (60*60), qAbs( (time % (60*60)) / 60 ), qAbs( time % 60 ) );
}

QString tlTools::TimesToSpanString( QTime Start, QTime End ){
    return formatWorkTime( TimesToSpan( Start, End ) );
}

qint64 tlTools::TimesToSpan( QTime Start, QTime End ){
    return (End.msecsSinceStartOfDay() - Start.msecsSinceStartOfDay()) / 1000;
}

QTime tlTools::StringToTime( QString Time ){
    QStringList strParts;
    QTime timeT;
    int hms[] = {0,0,0};
    bool ok_hms[] = {false,true,true};

    Time.remove( " " );
    Time.replace( 'h', ':' );
    strParts = Time.split( ':' );
    for( int i = 0; i < strParts.count(); i++ ){
        hms[i] = strParts[i].toInt( &ok_hms[i] );
    }
    if( ok_hms[0] && ok_hms[1] && ok_hms[2] ){
        timeT.setHMS( hms[0], hms[1], hms[2] );
    }
    return timeT;
}

QString tlTools::AppendPath(const QString& path1, const QString& path2){
    return QDir::cleanPath(path1 + QDir::separator() + path2);
}
