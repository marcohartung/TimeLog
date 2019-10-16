/* TimeLog - Copyright (C) 2019  Marco Hartung
 *
 * This file is part of TimeLog.
 *
 * TimeLog is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * TimeLog is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with TimeLog.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "tlSettings.h"
#include "tltools.h"
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

QString tlSettings::DataBackupPath( void ){
   QFileInfo dbFileInfo( strDataPath );
   return   tlTools::AppendPath( dbFileInfo.absolutePath(), "Backup" );
}
