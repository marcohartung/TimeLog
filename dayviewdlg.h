#ifndef DAYVIEWDLG_H
#define DAYVIEWDLG_H

#include <QDialog>
#include <QMenu>
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
    QMenu* cmDayContent;
    QAction* cmaDayContentEdit;
    QAction* cmaDayContentAdd;

    tlData* pData;
    QDate day;

 private slots:
    void accept();
    void rejected();
    void UpdateView( );
    void ShowContextMenu( const QPoint &pos );
    void AddData( );
    void EditData( );

};

#endif // DAYVIEWDLG_H
