#include "tlSettings.h"
#include <QSettings>
#include <QDir>

tlSettings::tlSettings()
{

}

bool tlSettings::ReadSettings( void )
{
    QSettings settings;

    strDataPath = settings.value( "DataPath", QDir::homePath() + "/.TimeLog/UserData/TimeLogData.xml" ).toString();
    fLogWorkTimeWithApp = settings.value( "LogWorkTimeWithApp", false ).toBool();
    fStartToTray = settings.value( "StartToTray", false ).toBool();

    return true;
}

bool tlSettings::SaveSettings( void )
{
    QSettings settings;

    settings.setValue( "DataPath", strDataPath );
    settings.setValue( "LogWorkTimeWithApp", fLogWorkTimeWithApp );
    settings.setValue( "StartToTray", fStartToTray );

    return true;
}
