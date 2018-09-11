#ifndef DAYVIEWDLG_H
#define DAYVIEWDLG_H

#include <QDialog>
#include "tlData.h"

namespace Ui {
class DayViewDlg;
}

class DayViewDlg : public QDialog
{
    Q_OBJECT

public:
    explicit DayViewDlg(QWidget *parent = 0);
    ~DayViewDlg();

    void SetData( tlData* pd );

private:
    Ui::DayViewDlg *ui;

    void UpdateView( );


    tlData* pData;
};

#endif // DAYVIEWDLG_H
