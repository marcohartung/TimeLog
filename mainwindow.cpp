#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>
#include <QFileInfo>
#include <QMessageBox>
#include "configdlg.h"

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

    ui->setupUi(this);

    confdlg = new ConfigDlg( this );

    settings.ReadSettings();

    createActions();
    //createTrayIcon();

    createTimer();

    ui->pbBreakStartStop->setEnabled(false);

    ui->tbSettings->setIcon( QIcon(":/resource/configure.png") );
    connect( ui->tbSettings, SIGNAL(clicked()), this, SLOT(tbSettingsClicked()) );
    //connect( ui->tbShowConfig, SIGNAL(clicked()), this, SLOT(hide()) );
    //connect( closeAction, SIGNAL(triggered()), qApp, SLOT(quit()) );

    connect( ui->pbWorkStartStop, SIGNAL(clicked()), this, SLOT(WorkStartStopClicked()) );
    connect( ui->pbBreakStartStop, SIGNAL(clicked()), this, SLOT(BreakStartStopClicked()) );

    //trayIcon->show();

    ReadDataBase();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createActions()
{
//    configAction = new QAction(tr("&Configure"), this);
//    configAction->setIcon( QIcon(":/resource/configure.png") );
//    connect(configAction, SIGNAL(triggered()), configDlg, SLOT(show()));

//    closeAction = new QAction(tr("&Quit"), this);
//    closeAction->setIcon(QIcon(":/resource/cancel.png") );
//    connect(closeAction, SIGNAL(triggered()), qApp, SLOT(quit()));
}

//void MainWindow::createTrayIcon()
//{
//    trayIconMenu = new QMenu(this);


//    trayIconMenu->addAction(configAction);
//    trayIconMenu->addSeparator();
//    trayIconMenu->addAction(closeAction);

//    trayIcon = new QSystemTrayIcon(this);
//    trayIcon->setContextMenu(trayIconMenu);

//    trayIcon->setIcon(QIcon(":/resource/computer.png"));

//    connect( trayIcon,
//             SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
//             this,
//             SLOT(trayIconClicked(QSystemTrayIcon::ActivationReason)) );
//}

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
    }

    ui->lWorkTime->setText( formatWorkTime( worktime ) );
    ui->lBreak->setText( formatWorkTime( breaktime ) );
}

void MainWindow::tbSettingsClicked(){

    // TODO
    if( confdlg->exec() == QDialog::Accepted ){
        settings.ReadSettings();
        ReadDataBase();
    }

}

//void MainWindow::trayIconClicked(QSystemTrayIcon::ActivationReason reason)
//{
//    if(reason == QSystemTrayIcon::Trigger){
//        if( this->isVisible() ){
//            this->hide();
//        }
//        else{

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

//            this->show();
//        }
//    }
//}

void MainWindow::closeEvent( QCloseEvent *event ){

    data.WriteXml( settings.DataPath() );

//    if (trayIcon->isVisible()) {
//        hide();

//        event->ignore(); // Don't let the event propagate to the base class
//    }
}


void MainWindow::ReadDataBase(){

    QFileInfo datafile(settings.DataPath());
    bool state = false;

    while( 1 ){

        if( datafile.exists() && datafile.isFile() ) {
            if( data.ReadXml( settings.DataPath() ) == true ){
                state = true;
                break;
            }
        }
        else{
            QMessageBox msgBox;
            msgBox.setText( tr("Keine Datenbank gefunden!") );
            msgBox.setInformativeText( settings.DataPath() );
            QPushButton* pbCreate = msgBox.addButton( tr("Erstelle neue Datenbank"), QMessageBox::AcceptRole );
            QPushButton* pbSelect = msgBox.addButton( tr("Wähle Datenbank"), QMessageBox::AcceptRole );
            msgBox.exec();
            if( msgBox.clickedButton() == pbCreate ){
                DBCreate();
            }
            else if( msgBox.clickedButton() == pbSelect ){
                DBSelect();
            }
        }
    }

    if( state == false ){
        QMessageBox msgBox;
        msgBox.setText( tr("Konnte Datenbank nicht laden!") );
        msgBox.setInformativeText( settings.DataPath() );
        msgBox.setIcon( QMessageBox::Critical );
        msgBox.exec();
    }
}

void MainWindow::DBSelect( void ){
// TODO
}

void MainWindow::DBCreate( void ){
    data.Clear();
    data.WriteXml( settings.DataPath() );
}

void MainWindow::WorkStartStopClicked(){

    if( f_working ){
        data.AddTime( QDate::currentDate(), QTime::currentTime(), tlData::enuStop, tlData::enuWork );
        ui->pbWorkStartStop->setText( tr("Start") );
        ui->pbBreakStartStop->setEnabled(false);
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
        f_working = true;
    }
}

void MainWindow::BreakStartStopClicked(){

    if( f_break ){
        data.AddTime( QDate::currentDate(), QTime::currentTime(), tlData::enuStop, tlData::enuBreak );
        ui->pbBreakStartStop->setText( tr("Pause") );
        f_break = false;
    }
    else{
        data.AddTime( QDate::currentDate(), QTime::currentTime(), tlData::enuStart, tlData::enuBreak );
        ui->pbBreakStartStop->setText( tr("Pause Ende") );
        f_break = true;
    }
}


QString MainWindow::formatWorkTime( qint64 time ){
    return QString::asprintf( "%02lld:%02lld:%02lld", time / (60*60),(time % (60*60)) / 60, time % 60 );
}
