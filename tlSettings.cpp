#include "tlSettings.h"
#include <QSettings>
#include <QDir>

tlSettings::tlSettings()
{

}

bool tlSettings::ReadSettings( void )
{
    QSettings settings;

    strDataPath = settings.value( "DataPath", QDir::homePath() + "/TimeLogData.xml" ).toString();

    return true;
}

bool tlSettings::SaveSettings( void )
{
    QSettings settings;

    settings.setValue( "DataPath", strDataPath );

    return true;
}
