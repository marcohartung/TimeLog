#include "edittaskdlg.h"
#include "ui_edittaskdlg.h"

EditTaskDlg::EditTaskDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditTaskDlg)
{
    ui->setupUi(this);
}

EditTaskDlg::~EditTaskDlg()
{
    delete ui;
}
