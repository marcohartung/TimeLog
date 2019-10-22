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

#include "aboutdlg.h"
#include "ui_aboutdlg.h"

AboutDlg::AboutDlg(QWidget *parent) :
    QDialog(parent, Qt::WindowSystemMenuHint | Qt::WindowTitleHint ),
    ui(new Ui::AboutDlg)
{
    ui->setupUi(this);
    connect( ui->pbClose, SIGNAL(clicked()), this, SLOT(close()) );

    setFixedSize( this->size() );

    ui->label_AppVersion->setText( APP_VERSION );
    ui->label_Copyright->setText( QString(APP_COPYRIGHT) + QString("\nAll rights reserved.") );
}

AboutDlg::~AboutDlg()
{
    delete ui;
}
