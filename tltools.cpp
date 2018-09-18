#include "tltools.h"
#include <QTime>

tlTools::tlTools()
{

}


QString tlTools::formatWorkTime( qint64 time ){
    return QString::asprintf( "%02lld:%02lld:%02lld", time / (60*60),(time % (60*60)) / 60, time % 60 );
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
