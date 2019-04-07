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
    QString DataBackupPath( void ) ;
    bool LogWorkTimeWithApp( void ) const { return fLogWorkTimeWithApp; }
    bool StartToTray( void ) const { return fStartToTray; }


    // TODO DataPath and FileName Seperate
    // create Folder in DataPath
    // Remove bkup Files > 10
    void SetDataPath( const QString path ){ strDataPath = path; }
    void SetLogWorkTimeWithApp( const bool enabled ){ fLogWorkTimeWithApp = enabled; }
    void SetStartToTray( const bool enabled ){ fStartToTray = enabled; }

 protected:

private:
    QString strDataPath;
    bool fLogWorkTimeWithApp;
    bool fStartToTray;
};

#endif // TLSETTINGS_H
