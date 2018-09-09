#ifndef EDITTASKDLG_H
#define EDITTASKDLG_H

#include <QDialog>

namespace Ui {
class EditTaskDlg;
}

class EditTaskDlg : public QDialog
{
    Q_OBJECT

public:
    explicit EditTaskDlg(QWidget *parent = 0);
    ~EditTaskDlg();

private:
    Ui::EditTaskDlg *ui;
};

#endif // EDITTASKDLG_H
