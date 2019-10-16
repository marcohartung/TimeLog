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
