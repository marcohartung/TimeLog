#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "tlSettings.h"
#include "tlData.h"

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

//    void closeEvent(QCloseEvent *); // Overriding the window's close event

    void checkState( void );

    QString formatWorkTime( qint64 time );

    Ui::MainWindow *ui;

    tlSettings settings;
    tlData data;

    bool f_working;
    qint64 worktime;

//    QSystemTrayIcon *trayIcon;
//    QMenu *trayIconMenu;

//    QAction *configAction;
//    QAction *closeAction;

//    ascConfigDlg* configDlg;
//    QVector<ascServerInfo*> servers;

    QTimer *timer;

//    QUdpSocket* udpSocket;

private slots:
    void ticTimer( );
//    void trayIconClicked( QSystemTrayIcon::ActivationReason );

    void WorkStartStopClicked();
};

#endif // MAINWINDOW_H
