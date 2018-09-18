#ifndef TLTOOLS_H
#define TLTOOLS_H

#include <QtCore/qobject.h>

class tlTools
{
public:
    tlTools();

    static QString formatWorkTime( qint64 time );

    static QString TimesToSpanString( QTime Start, QTime End );

    static qint64 TimesToSpan( QTime Start, QTime End );

    // read a string and return time as QTime
    // valid formats are:
    // "hh:mm:ss"
    // "hh:mm"
    // "hh"
    // <hh>h<mm> e.g 6h50
    static QTime StringToTime( QString Time );
};

#endif // TLTOOLS_H
