#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include "tlSettings.h"
#include "tlData.h"
#include "configdlg.h"
#include "reportdlg.h"
#include "dayviewdlg.h"
#include "aboutdlg.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void show(); //overwrite show, so we can start hidden

private:
    void createActions();
    void createTrayIcon();
    void createTimer();

    void closeEvent(QCloseEvent *); // Overriding the window's close event

    void updateDataFields( void );

    void ReadDataBase( void );
    void WriteDataBase( void );
    void DBSelect( void );
    void DBCreate( void );

    Ui::MainWindow *ui;

    ConfigDlg* confdlg;
    ReportDlg* reportdlg;
    DayViewDlg* dayviewdlg;
    AboutDlg* aboutdlg;

    tlSettings settings;
    tlData data;

    bool f_working;
    bool f_break;
    bool f_project;
    qint64 worktime;
    qint64 breaktime;
    qint64 projecttime;

    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;

    bool fFirstRun;

    QAction *configAction;
    QAction *closeAction;
    QAction *actionToggleWork;
    QAction *actionToggleBreak;
    QAction *actionShowDayViewDlg;
    QAction *actionShowReportDlg;

    QTimer *timer;

private slots:
    void ticTimer( );

    void AboutToQuitSignaled();

    void tbSettingsClicked();
    void pbOverviewClicked();
    void pbDayViewClicked();
    void ShowAboutDlg();
    void trayIconClicked( QSystemTrayIcon::ActivationReason );

    void WorkStartStopClicked();
    void BreakStartStopClicked();
    void ProjStartStopClicked();

    void ProjImport();
    void BmasImport();
};

#endif // MAINWINDOW_H
