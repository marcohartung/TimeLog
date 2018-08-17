#ifndef REPORTDLG_H
#define REPORTDLG_H

#include <QDialog>
#include "tlData.h"

namespace Ui {
class ReportDlg;
}

class ReportDlg : public QDialog
{
    Q_OBJECT

public:
    explicit ReportDlg(QWidget *parent = 0);
    ~ReportDlg();

    void SetData( tlData* pd );

private:
    Ui::ReportDlg *ui;

    tlData* pData;

};

#endif // REPORTDLG_H
