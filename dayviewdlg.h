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

    tlData* pData;

 private slots:
    void UpdateView( );
    void ShowContextMenu( const QPoint &pos );
    void AddData( );

};

#endif // DAYVIEWDLG_H
