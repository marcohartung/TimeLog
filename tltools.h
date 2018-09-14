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
};

#endif // TLTOOLS_H
