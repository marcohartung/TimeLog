#include "configdlg.h"
#include <QFileDialog>
#include "ui_configdlg.h"


ConfigDlg::ConfigDlg(QWidget *parent) :
    QDialog(parent),
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

    ui->leDataPath->setText( settings.DataPath() );
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
        for( size_t i = 0; i < projecsts.size(); i++ ){
            ui->cbProj->addItem( projecsts[i].Name );
        }
    }

}

void ConfigDlg::accept(){
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
    settings.SetDataPath( fileName );
    ui->leDataPath->setText( settings.DataPath() );
}
