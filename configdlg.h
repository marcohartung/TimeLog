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

#ifndef CONFIGDLG_H
#define CONFIGDLG_H

#include <QDialog>
#include "tlSettings.h"
#include "tlData.h"

namespace Ui {
class ConfigDlg;
}

class ConfigDlg : public QDialog
{
    Q_OBJECT

public:
    explicit ConfigDlg(QWidget *parent = 0);
    ~ConfigDlg();

    void SetData( tlData* pd );

private:
    Ui::ConfigDlg *ui;

    tlSettings settings;

    tlData* pData;

private slots:
    void accept();
    void rejected();

    void ProjAdd();
    void ProjDel();

    void PathChangeClicked();
};

#endif // CONFIGDLG_H
