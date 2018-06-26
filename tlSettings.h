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

    // TODO DataPath and FileName Seperate
    // create Folder in DataPath
    // Remove bkup Files > 10
    void SetDataPath( const QString path ){ strDataPath = path; }

 protected:

private:
    QString strDataPath;
};

#endif // TLSETTINGS_H
