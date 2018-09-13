#ifndef REPORTDLG_H
#define REPORTDLG_H

#include <QDialog>
#include "tlData.h"
#include "edittaskdlg.h"

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

    EditTaskDlg* etskdlg;
    tlData* pData;

private slots:
    void UpdateView( );
   // void ShowContextMenu( const QPoint &pos );

    void EditData( );
    void AddData();

};

#endif // REPORTDLG_H
