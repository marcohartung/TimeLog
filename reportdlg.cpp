#include "reportdlg.h"
#include "ui_reportdlg.h"
#include "tltools.h"

ReportDlg::ReportDlg(QWidget *parent) :
    QDialog(parent, Qt::WindowSystemMenuHint | Qt::WindowTitleHint ),
    ui(new Ui::ReportDlg)
{
    ui->setupUi(this);

    pData = 0;

    const QDate today( QDate::currentDate() );
    ui->deDateStart->setCalendarPopup( true );
    ui->deDateEnd->setCalendarPopup( true );
    ui->deDateStart->setDate( QDate( today.year(), today.month(), 1 ) );
    ui->deDateEnd->setDate( today.addDays(-1) );

    ui->twOverview->setColumnCount( 4 );
    QStringList HeaderLabel;
    HeaderLabel.push_back( "Datum" );
    HeaderLabel.push_back( "Zeit" );
    HeaderLabel.push_back( "Projekt Zeit [%]" );
    HeaderLabel.push_back( "" );
    ui->twOverview->setHeaderLabels( HeaderLabel );

    connect( ui->deDateStart, SIGNAL(editingFinished()), this, SLOT(UpdateView()) );
    connect( ui->deDateEnd, SIGNAL(editingFinished()), this, SLOT(UpdateView()) );
}

ReportDlg::~ReportDlg()
{
    delete ui;
}

void ReportDlg::SetData( tlData* pd ){
    pData = pd;
    UpdateView();
}

void ReportDlg::UpdateView( ){

    ui->teSummery->clear();
    ui->twOverview->clear();

    if( pData ){
        QTreeWidgetItem* pTreeItem;
        QTreeWidgetItem* pTreeSubItem;

        for( QDate date = ui->deDateStart->date(); date <= ui->deDateEnd->date(); date = date.addDays( 1) ){

            tlData::WorkSummery_t ws = pData->GetWorktimeSummery( date, date, false );
            if( ws.WorkDays == 1 ){
                qint64 ProjTime_sec = 0;

                pTreeItem = new QTreeWidgetItem;
                pTreeItem->setText( 0, date.toString() );
                pTreeSubItem = new QTreeWidgetItem;
                pTreeSubItem->setText( 0, "Pause" );
                if( ws.WorkDaysInvalidData == 0 ){
                    pTreeItem->setText( 1, tlTools::formatWorkTime( ws.TimeWork_sec ) );
                    pTreeSubItem->setText( 1, tlTools::formatWorkTime( ws.TimeBreak_sec ) );
                }
                else {
                    pTreeItem->setIcon( 3, QIcon(":/resource/error.png") );
                }
                pTreeItem->addChild( pTreeSubItem );


                QVector<tlData::tasksummery_t>::iterator i;
                for( i = ws.tasks.begin(); i < ws.tasks.end(); i++ ){
                    pTreeSubItem = new QTreeWidgetItem;
                    QString InfoText;

                    ProjTime_sec += i->time_sec;
                    InfoText = i->TaskName + " (" + i->TaskSubName + ")";
                    pTreeSubItem->setText( 0, InfoText );
                    pTreeSubItem->setText( 1, tlTools::formatWorkTime( i->time_sec ) );
                    pTreeItem->addChild( pTreeSubItem );
                }

                // print project time / work time ration in percent
                float ProjTime_percent = 0;
                if( ws.TimeWork_sec > 0 ){
                    ProjTime_percent = (float)ProjTime_sec / (float)ws.TimeWork_sec * 100.0;
                }
                pTreeItem->setText( 2, QString::asprintf( "%.1f", ProjTime_percent ) );
                if( ProjTime_percent > 100 ){
                    pTreeItem->setTextColor( 2, QColor(255,0,0,255) );
                }

                ui->twOverview->addTopLevelItem( pTreeItem );
            }
        }

        // print big overwiew
        tlData::WorkSummery_t ws = pData->GetWorktimeSummery( ui->deDateStart->date(), ui->deDateEnd->date() );
        QString WorkInfo;

        if( ws.WorkDays > 0 ){
            qint64 WorkTimePerDay_sec = 0;
            if( (ws.WorkDays - ws.WorkDaysInvalidData) > 0 ){
                WorkTimePerDay_sec = ws.TimeWork_sec / (ws.WorkDays - ws.WorkDaysInvalidData);
            }

            WorkInfo = ui->deDateStart->date().toString() + " - " + ui->deDateEnd->date().toString();
            WorkInfo += "\r\n";
            WorkInfo += "Arbeitszeit gesammt:\t" + tlTools::formatWorkTime( ws.TimeWork_sec ) + "\r\n";
            WorkInfo += "Arbeittage:         \t" + QString::number( ws.WorkDays );
            if( ws.WorkDaysInvalidData > 0 ){
                WorkInfo += "(" + QString::number( ws.WorkDaysInvalidData ) + "!)";
            }
            WorkInfo += "\r\n";
            WorkInfo += "Arbeitszeit pro Tag:\t" + tlTools::formatWorkTime( WorkTimePerDay_sec ) + "\r\n";
            WorkInfo += "\r\n";
            QVector<tlData::tasksummery_t>::iterator tasks_i;
            for( tasks_i = ws.tasks.begin(); tasks_i < ws.tasks.end(); tasks_i++ ) {
                WorkInfo += tasks_i->TaskName + "\t" + tlTools::formatWorkTime( tasks_i->time_sec );
                WorkInfo += "\t" + QString::number( ((double)(tasks_i->time_sec) / (double)(ws.TimeWork_sec)) * 100.0, 'f', 1 ) + "%\r\n"  ;
            }
            ui->teSummery->setText( WorkInfo );
        }
    }
}
