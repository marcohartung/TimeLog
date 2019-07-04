#include "aboutdlg.h"
#include "ui_aboutdlg.h"

AboutDlg::AboutDlg(QWidget *parent) :
    QDialog(parent, Qt::WindowSystemMenuHint | Qt::WindowTitleHint ),
    ui(new Ui::AboutDlg)
{
    ui->setupUi(this);
     connect( ui->pbClose, SIGNAL(clicked()), this, SLOT(close()) );

     ui->label_AppVersion->setText( APP_VERSION );
     ui->label_Copyright->setText( QString(APP_COPYRIGHT) + QString("\nAll rights reserved.") );
}

AboutDlg::~AboutDlg()
{
    delete ui;
}
