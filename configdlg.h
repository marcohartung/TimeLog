#ifndef CONFIGDLG_H
#define CONFIGDLG_H

#include <QDialog>
#include "tlSettings.h"

namespace Ui {
class ConfigDlg;
}

class ConfigDlg : public QDialog
{
    Q_OBJECT

public:
    explicit ConfigDlg(QWidget *parent = 0);
    ~ConfigDlg();

private:
    Ui::ConfigDlg *ui;

    tlSettings settings;

private slots:
    void accept();
    void rejected();

    void PathChangeClicked();
};

#endif // CONFIGDLG_H
