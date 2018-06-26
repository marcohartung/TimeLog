#ifndef CONFIGDLG_H
#define CONFIGDLG_H

#include <QDialog>
#include "tlSettings.h"
#include "tlData.h"

namespace Ui {
class ConfigDlg;
}

class ConfigDlg : public QDialog
{
    Q_OBJECT

public:
    explicit ConfigDlg(QWidget *parent = 0);
    ~ConfigDlg();

    void SetData( tlData* pd );

private:
    Ui::ConfigDlg *ui;

    tlSettings settings;

    tlData* pData;

private slots:
    void accept();
    void rejected();

    void ProjAdd();
    void ProjDel();

    void PathChangeClicked();
};

#endif // CONFIGDLG_H
