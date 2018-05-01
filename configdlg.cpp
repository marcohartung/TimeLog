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

    settings.ReadSettings();

    ui->leDataPath->setText( settings.DataPath() );
}

ConfigDlg::~ConfigDlg()
{
    delete ui;
}

void ConfigDlg::accept(){
    settings.SaveSettings();
    this->setResult( QDialog::Accepted );
    this->close();
}

void ConfigDlg::rejected(){

}

void ConfigDlg::PathChangeClicked(){

    QString fileName = QFileDialog::getOpenFileName( this, tr("TimeLog Data"), settings.DataPath(), tr("TimeLog Data (*.xml)") );
    settings.SetDataPath( fileName );
    ui->leDataPath->setText( settings.DataPath() );
}
