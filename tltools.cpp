#include "tltools.h"

tlTools::tlTools()
{

}


QString tlTools::formatWorkTime( qint64 time ){
    return QString::asprintf( "%02lld:%02lld:%02lld", time / (60*60),(time % (60*60)) / 60, time % 60 );
}
