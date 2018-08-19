#include "reportdlg.h"
#include "ui_reportdlg.h"
#include "tltools.h"

ReportDlg::ReportDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReportDlg)
{
    ui->setupUi(this);

    pData = 0;

    const QDate today( QDate::currentDate() );
    ui->deDateStart->setDate( QDate( today.year(), today.month(), 1 ) );
    ui->deDateEnd->setDate( today );

    ui->twOverview->setColumnCount( 2 );
    QStringList HeaderLabel;
    HeaderLabel.push_back( "Datum" );
    HeaderLabel.push_back( "Zeit" );
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
                pTreeItem = new QTreeWidgetItem;
                pTreeItem->setText( 0, date.toString() );
                pTreeItem->setText( 1, tlTools::formatWorkTime( ws.TimeWork_sec ) );

                pTreeSubItem = new QTreeWidgetItem;
                pTreeSubItem->setText( 0, "Pause" );
                pTreeSubItem->setText( 1, tlTools::formatWorkTime( ws.TimeBreak_sec ) );
                pTreeItem->addChild( pTreeSubItem );

                QVector<tlData::tasksummery_t>::iterator i;
                for( i = ws.tasks.begin(); i < ws.tasks.end(); i++ ){
                    pTreeSubItem = new QTreeWidgetItem;
                    QString InfoText;

                    InfoText = i->TaskName + " (" + i->TaskSubName + ")";
                    pTreeSubItem->setText( 0, InfoText );
                    pTreeSubItem->setText( 1, tlTools::formatWorkTime( i->time_sec ) );
                    pTreeItem->addChild( pTreeSubItem );
                }

                ui->twOverview->addTopLevelItem( pTreeItem );
            }
        }

        // print big overwiew
        tlData::WorkSummery_t ws = pData->GetWorktimeSummery( ui->deDateStart->date(), ui->deDateEnd->date() );
        QString WorkInfo;

        if( ws.WorkDays > 0 ){
            WorkInfo = ui->deDateStart->date().toString() + " - " + ui->deDateEnd->date().toString();
            WorkInfo += "\r\n";
            WorkInfo += "Arbeitszeit gesammt:\t" + tlTools::formatWorkTime( ws.TimeWork_sec ) + "\r\n";
            WorkInfo += "Arbeitszeit pro Tag:\t" + tlTools::formatWorkTime( ws.TimeWork_sec / ws.WorkDays ) + "\r\n";
            WorkInfo += "\r\n";
            QVector<tlData::tasksummery_t>::iterator tasks_i;
            for( tasks_i = ws.tasks.begin(); tasks_i < ws.tasks.end(); tasks_i++ ) {
                WorkInfo += tasks_i->TaskName + "\t" + tlTools::formatWorkTime( tasks_i->time_sec );
                WorkInfo += "\t" + QString::number( (tasks_i->time_sec / ws.TimeWork_sec) * 100, 1 ) + "%\r\n"  ;
            }
            ui->teSummery->setText( WorkInfo );
        }
    }
}
