#ifndef TLTOOLS_H
#define TLTOOLS_H

#include <QtCore/qobject.h>

class tlTools
{
public:
    tlTools();

    static QString formatWorkTime( qint64 time );
};

#endif // TLTOOLS_H
