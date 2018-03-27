#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    QCoreApplication::setOrganizationName("hEektronik");
    QCoreApplication::setOrganizationDomain("helektronik.de");
    QCoreApplication::setApplicationName("TimeLog");

    f_working = false;
    worktime = 0;

    ui->setupUi(this);

    settings.ReadSettings();

    settings.SetDataPath( "/home/marco/test.xml" );

    createActions();
    //createTrayIcon();

    createTimer();

    ui->tbSettings->setIcon( QIcon(":/resource/configure.png") );
    //connect( ui->tbShowConfig, SIGNAL(clicked()), configDlg, SLOT(show()) );
    //connect( ui->tbShowConfig, SIGNAL(clicked()), this, SLOT(hide()) );
    //connect( closeAction, SIGNAL(triggered()), qApp, SLOT(quit()) );

    connect( ui->pbWorkStartStop, SIGNAL(clicked()), this, SLOT(WorkStartStopClicked()) );

    checkState();

    //trayIcon->show();

    tlData d;
    d.ReadXml( settings.DataPath() );

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
    if( f_working ){
        worktime ++;
        ui->lWorkTime->setText( formatWorkTime( worktime ) );
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

//void MainWindow::closeEvent(QCloseEvent *event)
//{
//    if (trayIcon->isVisible()) {
//        hide();

//        event->ignore(); // Don't let the event propagate to the base class
//    }
//}


void MainWindow::checkState(){

    //TODO: if no data base -> create
    data.ReadXml( settings.DataPath() );

}


void MainWindow::WorkStartStopClicked(){

    if( f_working ){
        f_working = false;
    }
    else{
        f_working = true;
    }
}

QString MainWindow::formatWorkTime( qint64 time ){
    return QString::asprintf( "%02lld:%02lld:%02lld", time / (60*60),(time % (60*60)) / 60, time % 60 );
}
