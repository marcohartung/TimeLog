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
