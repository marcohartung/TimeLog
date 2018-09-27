#include "aboutdlg.h"
#include "ui_aboutdlg.h"

AboutDlg::AboutDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDlg)
{
    ui->setupUi(this);
     connect( ui->pbClose, SIGNAL(clicked()), this, SLOT(close()) );
}

AboutDlg::~AboutDlg()
{
    delete ui;
}
