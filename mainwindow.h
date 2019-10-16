/* TimeLog - Copyright (C) 2019  Marco Hartung
 *
 * This file is part of TimeLog.
 *
 * TimeLog is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * TimeLog is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with TimeLog.  If not, see <http://www.gnu.org/licenses/>.
 */

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

    bool ReadDataBase( void );
    bool WriteDataBase( void );
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
    qint64 worktime;
    qint64 breaktime;

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

    void ProjImport();
    void BmasImport();
};

#endif // MAINWINDOW_H
