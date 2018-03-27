#ifndef TLSETTINGS_H
#define TLSETTINGS_H

#include <QString>

class tlSettings
{
public:
    tlSettings();

    bool ReadSettings( void );
    bool SaveSettings( void );

    QString DataPath( void ) const { return strDataPath; }

    void SetDataPath( const QString path ){ strDataPath = path; }

 protected:

private:
    QString strDataPath;
};

#endif // TLSETTINGS_H
