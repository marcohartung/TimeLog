#ifndef EDITTASKDLG_H
#define EDITTASKDLG_H

#include <QDialog>
#include "tlData.h"

namespace Ui {
class EditTaskDlg;
}

class EditTaskDlg : public QDialog
{
    Q_OBJECT

public:
    explicit EditTaskDlg(QWidget *parent = 0);
    ~EditTaskDlg();

    void SetData( const tlData::worktask_t& defvalues );

private:
    Ui::EditTaskDlg *ui;
};

#endif // EDITTASKDLG_H
