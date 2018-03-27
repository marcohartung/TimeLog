#include "tlSettings.h"
#include <QSettings>

tlSettings::tlSettings()
{

}

bool tlSettings::ReadSettings( void )
{
    QSettings settings;

    settings.value( "DataPath", "" ).toInt();

    return true;
}

bool tlSettings::SaveSettings( void )
{
    QSettings settings;

    settings.setValue( "DataPath", strDataPath );

    return true;
}
