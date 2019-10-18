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

#include "configdlg.h"
#include <QFileDialog>
#include "ui_configdlg.h"

ConfigDlg::ConfigDlg(QWidget *parent) :
    QDialog(parent, Qt::WindowSystemMenuHint | Qt::WindowTitleHint ),
    ui(new Ui::ConfigDlg)
{
    ui->setupUi(this);

    connect( ui->buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect( ui->buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
    connect( ui->pbPathChange, SIGNAL(clicked()), this, SLOT(PathChangeClicked()) );
    connect( ui->pbProjAdd, SIGNAL(clicked()), this, SLOT(ProjAdd()) );
    connect( ui->pbProjDel, SIGNAL(clicked()), this, SLOT(ProjDel()) );

    pData = 0;
    settings.ReadSettings();

    ui->groupBox_Project->setVisible(false);

    ui->leDataPath->setText( settings.DataPath() );
    ui->cbLogWorkTimeWithApp->setChecked( settings.LogWorkTimeWithApp() );
    ui->cbStartupToTray->setChecked( settings.StartToTray() );
}

ConfigDlg::~ConfigDlg()
{
    delete ui;
}

void ConfigDlg::SetData( tlData* pd ){
    pData = pd;

    ui->cbProj->clear();

    if( pData ){
        QVector<tlData::project_t> projecsts = pData->GetProjectList();
        for( int i = 0; i < projecsts.size(); i++ ){
            ui->cbProj->addItem( projecsts[i].Name );
        }
    }

}

void ConfigDlg::accept(){

    settings.SetLogWorkTimeWithApp( ui->cbLogWorkTimeWithApp->isChecked() );
    settings.SetStartToTray( ui->cbStartupToTray->isChecked() );

    settings.SaveSettings();
    this->setResult( QDialog::Accepted );
    this->close();
}

void ConfigDlg::rejected(){

}

void ConfigDlg::ProjAdd(){

    if( pData ){
        if( pData->AddProject( ui->cbProj->currentText() ) ){
            ui->cbProj->addItem( ui->cbProj->currentText() );
        }
        else{
            //TODO add ErrorMsg]
        }
    }
}

void ConfigDlg::ProjDel(){

    if( pData ){
        if( pData->DelProject( ui->cbProj->currentText() ) ){
            ui->cbProj->removeItem( ui->cbProj->currentIndex() );
        }
        else{
            //TODO add ErrorMsg
        }
    }

}

void ConfigDlg::PathChangeClicked(){

    QString fileName = QFileDialog::getOpenFileName( this, tr("TimeLog Data"), settings.DataPath(), tr("TimeLog Data (*.xml)") );
    if( !fileName.isEmpty() ){
        settings.SetDataPath( fileName );
        ui->leDataPath->setText( settings.DataPath() );
    }
}
