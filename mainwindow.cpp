#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>
#include <QFileInfo>
#include <QMessageBox>
#include <QCloseEvent>
#include <QDesktopWidget>
#include <QFileDialog>
#include <QDir>
#include "tltools.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    QCoreApplication::setOrganizationName("hEektronik");
    QCoreApplication::setOrganizationDomain("helektronik.de");
    QCoreApplication::setApplicationName("TimeLog");

    f_working = false;
    worktime = 0;
    f_break = false;
    breaktime = 0;
    f_project = false;
    projecttime = 0;

    ui->setupUi(this);

    confdlg = 0;
    reportdlg = 0;
    dayviewdlg = 0;
    aboutdlg = 0;

    settings.ReadSettings();

    createActions();
    createTrayIcon();

    // hiden import menu
    if(0){
        QMenu *importMenu = new QMenu( "Import", this );
        QAction* bmasImportAction = new QAction(tr("BMAS Import"), this);
        QAction* projImportAction = new QAction(tr("Project Import"), this);

        importMenu->addAction( bmasImportAction );
        importMenu->addAction( projImportAction );
        ui->menuBar->addMenu( importMenu );

        connect(bmasImportAction, SIGNAL(triggered()), this, SLOT(BmasImport()));
        connect(projImportAction, SIGNAL(triggered()), this, SLOT(ProjImport()));
    }

    createTimer();

    ui->pbBreakStartStop->setEnabled(false);
    ui->pbProjStartStop->setEnabled(false);

    connect( ui->actionOptions, SIGNAL(triggered()), this, SLOT(tbSettingsClicked()) );
    connect( ui->actionReport, SIGNAL(triggered()), this, SLOT(pbOverviewClicked()) );
    connect( ui->actionDayView, SIGNAL(triggered()), this, SLOT(pbDayViewClicked()) );
    connect( ui->actionAbout, SIGNAL(triggered()), this, SLOT(ShowAboutDlg()) );
    connect( ui->actionQuit, SIGNAL(triggered()), qApp, SLOT(quit()) );

    connect( ui->pbWorkStartStop, SIGNAL(clicked()), this, SLOT(WorkStartStopClicked()) );
    connect( ui->pbBreakStartStop, SIGNAL(clicked()), this, SLOT(BreakStartStopClicked()) );
    connect( ui->pbProjStartStop, SIGNAL(clicked()), this, SLOT(ProjStartStopClicked()) );

    if( ReadDataBase() == false ){
        QMessageBox msgBox;
        msgBox.setText( tr("Konnte Datenbank nicht laden!") );
        msgBox.setInformativeText( settings.DataPath() );
        msgBox.setIcon( QMessageBox::Critical );
        msgBox.exec();
        exit(-1);
    }

    updateDataFields();

    if( settings.LogWorkTimeWithApp() ){

        QVector<tlData::worktime_t> workday = data.GetWorktimesOfDay( QDate::currentDate() );

        if( workday.empty() ) {
            WorkStartStopClicked();
        }
        else {
            QVector<tlData::worktime_t>::iterator i = workday.end()-1;

            QMessageBox msgBox;
            msgBox.setText( tr("Heute hast du schon gearbeitet!") );
            msgBox.setInformativeText("Start: " + i->timeStart.toString() + " - Stop: " + i->timeStop.toString() );
            QPushButton* pbResume = msgBox.addButton( tr("Arbeit fortsetzen"), QMessageBox::AcceptRole );
            QPushButton* pbStart = msgBox.addButton( tr("Neu Starten"), QMessageBox::AcceptRole );
            msgBox.addButton( tr("Ignorieren"), QMessageBox::AcceptRole );
            msgBox.exec();
            if( msgBox.clickedButton() == pbResume ){

                ui->pbWorkStartStop->setText( tr("Stop") );
                ui->pbBreakStartStop->setEnabled(true);
                ui->pbProjStartStop->setEnabled(true);

                actionToggleWork->setText( "Arbeit Stop" );
                actionToggleBreak->setText( "Pause Start" );
                actionToggleBreak->setDisabled(false);

                worktime = i->timeStart.msecsTo( QTime::currentTime() ) / 1000;

                f_working = true;
            }
            else if( msgBox.clickedButton() == pbStart ){
                WorkStartStopClicked();
            }
        }
    }

    trayIcon->show();
    fFirstRun = true;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::show(){
    if( !(settings.StartToTray() && fFirstRun) ){
        this->QMainWindow::show();
    }
    fFirstRun = false;
}

void MainWindow::createActions()
{
    closeAction = new QAction(tr("&Beenden"), this);
//    closeAction->setIcon(QIcon(":/resource/cancel.png") );
    connect(closeAction, SIGNAL(triggered()), qApp, SLOT(quit()));

    actionToggleWork = new QAction( "Arbeit Start",this);
    connect(actionToggleWork, SIGNAL(triggered()), this, SLOT(WorkStartStopClicked()));
    actionToggleBreak  = new QAction( "Pause Start",this);
    actionToggleBreak->setDisabled(true);
    connect(actionToggleBreak, SIGNAL(triggered()), this, SLOT(BreakStartStopClicked()));

    actionShowDayViewDlg = new QAction( "Tagesansicht...",this);
    connect(actionShowDayViewDlg, SIGNAL(triggered()), this, SLOT(pbDayViewClicked()));
    actionShowReportDlg = new QAction( "Übersicht...",this);
    connect(actionShowReportDlg, SIGNAL(triggered()), this, SLOT(pbOverviewClicked()));
}

void MainWindow::createTrayIcon()
{
    trayIconMenu = new QMenu(this);

    trayIconMenu->addAction(actionShowDayViewDlg);
    trayIconMenu->addAction(actionShowReportDlg);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(actionToggleWork);
    trayIconMenu->addAction(actionToggleBreak);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(closeAction);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);
    trayIcon->setIcon(QIcon(":/resource/clock.png"));

    connect( trayIcon,
             SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
             this,
             SLOT(trayIconClicked(QSystemTrayIcon::ActivationReason)) );
}

void MainWindow::createTimer(){
    timer = new QTimer( this );
    connect( timer, SIGNAL(timeout()), this, SLOT(ticTimer()) );
    timer->start( 1000 );
}

void MainWindow::ticTimer(){

    if( f_break ){
        breaktime++;
    }
    else if( f_working ){
        worktime ++;

        if( f_project ){
            projecttime++;
        }
    }

    QString strWorkTime = tlTools::formatWorkTime( worktime );
    QString strBreakTime = tlTools::formatWorkTime( breaktime );
    QString strToolTip = "Arbeitszeit:  " + strWorkTime + " - Pause: " + strBreakTime;
    ui->lWorkTime->setText( strWorkTime );
    ui->lBreak->setText( strBreakTime );
    ui->lProjTime->setText( tlTools::formatWorkTime( projecttime ) );
    trayIcon->setToolTip( strToolTip );
}

void MainWindow::AboutToQuitSignaled(){

    if( settings.LogWorkTimeWithApp() && f_working ){
       WorkStartStopClicked();
    }

    WriteDataBase();

}

void MainWindow::tbSettingsClicked(){

    if( confdlg == 0){
        confdlg = new ConfigDlg( this );
    }

    confdlg->SetData( &data );
    if( confdlg->exec() == QDialog::Accepted ){
        settings.ReadSettings();
        WriteDataBase( );
        updateDataFields();
    }

}

void MainWindow::pbOverviewClicked(){

    if( reportdlg == 0){
        reportdlg = new ReportDlg( this );
    }

    reportdlg->SetData( &data );
    reportdlg->exec();
}

void MainWindow::pbDayViewClicked(){

    if( dayviewdlg == 0){
        dayviewdlg = new DayViewDlg( this );
    }

    dayviewdlg->SetData( &data );
    dayviewdlg->exec();
}

void MainWindow::ShowAboutDlg(){

    if( aboutdlg == 0){
        aboutdlg = new AboutDlg( this );
    }
    aboutdlg->exec();
}

void MainWindow::trayIconClicked( QSystemTrayIcon::ActivationReason reason )
{
    if(reason == QSystemTrayIcon::Trigger){
        if( this->isVisible() ){
            this->hide();
        }
        else{

//            QRect trayIconPos = trayIcon->geometry();
//            QDesktopWidget desktop;
//            QRect availableGeo = desktop.availableGeometry( trayIconPos.center() );

//            QSize mySize = this->size();
////            QPoint myCentre( trayIconPos.center().x() - mySize.width()/2,
////                             avilableGeo.bottom() - mySize.height() );

//            // centre above the tray icon
//            QPoint myPos( trayIconPos.center().x() - mySize.width()/2,
//                          trayIconPos.center().y() - mySize.height()/2 );
//            // move to available area
//            if( myPos.x() < availableGeo.left() ){
//                myPos.setX( availableGeo.left() );
//            }
//            if( myPos.x()+mySize.width() > availableGeo.right() ){
//                myPos.setX( availableGeo.right() - mySize.width()/2 );
//            }
//            if( myPos.y() < availableGeo.top() ){
//                myPos.setY( availableGeo.top() );
//            }
//            if( myPos.y()+mySize.height() > availableGeo.bottom() ){
//                myPos.setY( availableGeo.bottom() - mySize.height() );
//            }
//            this->move( myPos );

            this->show();
            this->setWindowState( (windowState() & ~Qt::WindowMinimized) | Qt::WindowActive);
            this->raise();  // for MacOS
            this->activateWindow(); // for Windows
        }
    }
}

void MainWindow::closeEvent( QCloseEvent *event ){

    if( trayIcon->isVisible() ){
        hide();
         event->ignore(); // Don't let the event propagate to the base class
    }
}

void MainWindow::updateDataFields( void )
{
    QVector<tlData::project_t> projecsts = data.GetProjectList();

    ui->cbProjSel->clear();
    for( int i = 0; i < projecsts.size(); i++ ){
        ui->cbProjSel->addItem( projecsts[i].Name );
    }
}

bool MainWindow::ReadDataBase(void )
{
    QFileInfo datafile(settings.DataPath());
    bool state = false;

    while( 1 ){

        if( datafile.exists() && datafile.isFile() ) {
            data.SetDataFile( settings.DataPath() );
            if( data.ReadXml() == true ){
                state = true;
            }
            break;
        }
        else{
            QMessageBox msgBox;
            msgBox.setText( tr("Keine Datenbank gefunden!") );
            msgBox.setInformativeText( settings.DataPath() );
            QPushButton* pbCreate = msgBox.addButton( tr("Erstelle neue Datenbank"), QMessageBox::AcceptRole );
            QPushButton* pbSelect = msgBox.addButton( tr("Wähle Datenbank"), QMessageBox::AcceptRole );
            QPushButton* pbAbort = msgBox.addButton( tr("Abbrechen"), QMessageBox::AcceptRole );
            msgBox.exec();
            if( msgBox.clickedButton() == pbCreate ){
                DBCreate();
            }
            else if( msgBox.clickedButton() == pbSelect ){
                DBSelect();
                datafile.setFile( settings.DataPath() );
            }
            else if( msgBox.clickedButton() == pbAbort ){
                break;
            }
        }
    }

    return state;
}

bool MainWindow::WriteDataBase( void ){

    QFileInfo datafile( settings.DataPath() );
    if( datafile.exists() && datafile.isFile() ) {
        QFile oldDb( settings.DataPath() );
        QDir bkupFolder( settings.DataBackupPath() );
        if( !bkupFolder.exists() ){
            bkupFolder.mkpath( bkupFolder.absolutePath() );
        }
        QStringList nameFilters;
        nameFilters << "*" + datafile.fileName();
        QFileInfoList bkupFiles = bkupFolder.entryInfoList( nameFilters, QDir::NoFilter, QDir::Time );

        // crate one Backup a day
        if( bkupFiles.size() == 0 ||
            bkupFiles[0].created() < QDateTime::currentDateTime().addDays(-1) )
        {
            QString BackupFileName = QDateTime::currentDateTime().toString( "yyyy.MM.dd_hhmmsszzz_" );
            BackupFileName +=  datafile.fileName();
            oldDb.open( QIODevice::ReadWrite );
            oldDb.copy( tlTools::AppendPath( settings.DataBackupPath(), BackupFileName ) );

            // only keep 10 Backup Files
            for( int i = 10; i < bkupFiles.size(); i++ ){
                QFile f( bkupFiles[i].absoluteFilePath() );
                f.remove();
            }
        }
    }
    return data.WriteXml();
}

void MainWindow::DBSelect( void ){

    QString fileName = QFileDialog::getOpenFileName( this, tr("TimeLog Data"), settings.DataPath(), tr("TimeLog Data (*.xml)") );
    if( !fileName.isEmpty() ){
        settings.SetDataPath( fileName );
        settings.SaveSettings();
    }
}

void MainWindow::DBCreate( void ){

    QFileInfo datafile( settings.DataPath() );
    if( !datafile.absoluteDir().exists() ){
        QDir fileDir;
        fileDir.mkpath( datafile.absoluteDir().absolutePath() );
    }

    data.Clear();
    data.SetDataFile( settings.DataPath() );
    data.WriteXml( );
}

void MainWindow::WorkStartStopClicked(){

    if( f_working ){
        data.AddTime( QDate::currentDate(), QTime::currentTime(), tlData::enuStop, tlData::enuWork );
        ui->pbWorkStartStop->setText( tr("Start") );
        ui->pbBreakStartStop->setEnabled(false);
        ui->pbProjStartStop->setEnabled(false);

        actionToggleWork->setText( "Arbeit Start" );
        actionToggleBreak->setText( "Pause Start" );
        actionToggleBreak->setDisabled(true);

        f_working = false;
        if( f_break ){
            // stop break also if work is finished
            BreakStartStopClicked();
        }
    }
    else{
        data.AddTime( QDate::currentDate(), QTime::currentTime(), tlData::enuStart, tlData::enuWork );
        ui->pbWorkStartStop->setText( tr("Stop") );
        ui->pbBreakStartStop->setEnabled(true);
        ui->pbProjStartStop->setEnabled(true);

        actionToggleWork->setText( "Arbeit Stop" );
        actionToggleBreak->setText( "Pause Start" );
        actionToggleBreak->setDisabled(false);

        f_working = true;
    }

    WriteDataBase();
}

void MainWindow::BreakStartStopClicked(){
    static bool projwasrunning = false;

    if( f_break ){
        data.AddTime( QDate::currentDate(), QTime::currentTime(), tlData::enuStop, tlData::enuBreak );
        if( projwasrunning ){
            projwasrunning = false;
            ProjStartStopClicked();
        }
        ui->pbBreakStartStop->setText( tr("Pause") );

        actionToggleBreak->setText( "Pause Start" );

        f_break = false;
    }
    else{
        if( f_project ){
            projwasrunning = true;
            ProjStartStopClicked();
        }
        data.AddTime( QDate::currentDate(), QTime::currentTime(), tlData::enuStart, tlData::enuBreak );
        ui->pbBreakStartStop->setText( tr("Pause Ende") );

        actionToggleBreak->setText( "Pause Stop" );

        f_break = true;
    }

    WriteDataBase();
}


void MainWindow::ProjStartStopClicked(){

    if( f_project ){
        data.AddTime( QDate::currentDate(), QTime::currentTime(),
                      tlData::enuStop, tlData::enuProject );
        ui->pbProjStartStop->setText( tr("Start") );
        ui->cbProjSel->setEnabled( true );
        ui->cbProjComment->setEnabled( true );
        f_project = false;
    }
    else{
        ui->cbProjSel->setEnabled( false );
        ui->cbProjComment->setEnabled( false );
        data.AddTime( QDate::currentDate(), QTime::currentTime(),
                      tlData::enuStart, tlData::enuProject,
                      ui->cbProjSel->currentText(),
                      ui->cbProjComment->currentText()
                     );
        ui->pbProjStartStop->setText( tr("Stop") );
        f_project = true;
    }

    WriteDataBase();
}

void MainWindow::BmasImport(){

    // Import BMAS App Times

    QString strFile = "/home/marco/Projekte/TimeLog/ZeitLog.txt";
    QFile file( strFile );

    enum enuDecodeState_t{
        dsNothing,
        dsDateOk,
        dsStartOk,
        dsEndOk,
    };

    file.open( QIODevice::ReadOnly );

    const int64_t maxLineLength = 255;
    char line[maxLineLength];
    qint64 len;
    QString strLine;
    QStringList strParts;
    enuDecodeState_t ds = dsNothing;
    QDate date;
    QTime timeStart, timeEnd, timeBreak;
    while( ( len = file.readLine( line, maxLineLength ) ) > 0 ){

        line[len] = '\0';
        strLine = line;

        if( strLine.startsWith( "Datum:" ) ){
            strLine.remove( "Datum:" );
            strParts = strLine.split( '.' );
            date.setDate(  strParts[2].toInt() , strParts[1].toInt() , strParts[0].toInt() );
            if( date.isValid() ){
                ds = dsDateOk;
                continue;
            }
        }

        if( ds == dsDateOk ){
            if( strLine.startsWith( "Arbeitsbeginn:" ) ){
                strLine.remove( "Arbeitsbeginn:" );
                strLine.remove( " " );
                strLine.remove( "h" );
                strParts = strLine.split( ':' );
                timeStart.setHMS( strParts[0].toInt() , strParts[1].toInt() , 0 );
                if( timeStart.isValid() ){
                    ds = dsStartOk;
                    continue;
                }
            }
        }
        else if( ds == dsStartOk ){
            if( strLine.startsWith( "Arbeitsende:" ) ){
                strLine.remove( "Arbeitsende:" );
                strLine.remove( " " );
                strLine.remove( "h" );
                strParts = strLine.split( ':' );
                timeEnd.setHMS( strParts[0].toInt() , strParts[1].toInt() , 0 );
                if( timeEnd.isValid() ){
                    ds = dsEndOk;
                    continue;
                }
            }
        }
        else if( ds == dsEndOk ){
            if( strLine.startsWith( "Pausendauer:" ) ){
                strLine.remove( "Pausendauer:" );
                strLine.remove( " " );
                strLine.remove( "h" );
                strParts = strLine.split( ':' );
                timeBreak.setHMS( strParts[0].toInt() , strParts[1].toInt() , 0 );
                if( timeBreak.isValid() ){
                    ds = dsNothing;

                    data.AddTime( date, timeStart, tlData::enuStart, tlData::enuWork );
                    data.AddTime( date, timeBreak, tlData::enuSpan, tlData::enuBreak );
                    data.AddTime( date, timeEnd, tlData::enuStop, tlData::enuWork );

                    continue;
                }
            }
        }

    }
}

void MainWindow::ProjImport(){
    // Import text-style
    // dd.mm.yyyy
    // proj,task,time in h

    QString strFile = "/home/marco/Projekte/TimeLog/Zeiten2018.txt";
    QFile file( strFile );

    file.open( QIODevice::ReadOnly );

    const int64_t maxLineLength = 255;
    char line[maxLineLength];
    qint64 len;
    QString strLine;
    QStringList dateParts;
    QStringList TaskParts;
    QDate date;
    while( ( len = file.readLine( line, maxLineLength ) ) > 0 ){

        line[len] = '\0';
        strLine = line;

        dateParts = strLine.split( '.' );
        if( dateParts.size() == 3 ){
            date.setDate(  dateParts[2].toInt() , dateParts[1].toInt() , dateParts[0].toInt() );
            if( date.isValid() ){
               continue;
            }
        }

        if(  date.isValid() ){
            TaskParts = strLine.split( QRegExp("(\\;|\\,)") );
            if( TaskParts.size() == 3 ){
                QTime taskTime;
                TaskParts[2].remove( 'h' );
                taskTime.setHMS( TaskParts[2].toInt(), 0, 0 );
                data.AddTime( date, taskTime,
                              tlData::enuSpan, tlData::enuProject,
                              TaskParts[0],
                              TaskParts[1]
                             );

            }
        }
    }
}
