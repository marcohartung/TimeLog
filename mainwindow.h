#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "tlSettings.h"
#include "tlData.h"
#include "configdlg.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void createActions();
//    void createTrayIcon();
    void createTimer();

    void closeEvent(QCloseEvent *); // Overriding the window's close event

    void ReadDataBase( void );
    void DBSelect( void );
    void DBCreate( void );

    QString formatWorkTime( qint64 time );

    Ui::MainWindow *ui;

    ConfigDlg* confdlg;

    tlSettings settings;
    tlData data;

    bool f_working;
    bool f_break;
    qint64 worktime;
    qint64 breaktime;

//    QSystemTrayIcon *trayIcon;
//    QMenu *trayIconMenu;

//    QAction *configAction;
//    QAction *closeAction;

    QTimer *timer;

private slots:
    void ticTimer( );

    void tbSettingsClicked();
//    void trayIconClicked( QSystemTrayIcon::ActivationReason );

    void WorkStartStopClicked();
    void BreakStartStopClicked();
};

#endif // MAINWINDOW_H
