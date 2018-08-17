#include "reportdlg.h"
#include "ui_reportdlg.h"
#include "tltools.h"

ReportDlg::ReportDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReportDlg)
{
    ui->setupUi(this);

    const QDate today( QDate::currentDate() );
    ui->deDateStart->setDate( QDate( today.year(), today.month(), 1 ) );
    ui->deDateEnd->setDate( today );

    pData = 0;
}

ReportDlg::~ReportDlg()
{
    delete ui;
}

void ReportDlg::SetData( tlData* pd ){
    pData = pd;

    if( pData ){
        QTreeWidgetItem* pTreeItem;
        QTreeWidgetItem* pTreeSubItem;

        for( QDate date = ui->deDateStart->date(); date <= ui->deDateEnd->date(); date = date.addDays( 1) ){
            QVector<tlData::worktime_t> worktimeofday = pData->GetWorktimesOfDay( date );

            if( !worktimeofday.empty() ){

                pTreeItem = new QTreeWidgetItem;

                QVector<tlData::tasksummery_t> tasksummery = pData->GetWorktimeSummery( worktimeofday );

                if( tasksummery.size() >= 1 ){
                    pTreeItem->setText( 0, date.toString() );
                    pTreeItem->setText( 1, tlTools::formatWorkTime( tasksummery.first().time_sec ) );

                    QVector<tlData::tasksummery_t>::iterator i;
                    for( i = tasksummery.begin() + 1; i < tasksummery.end(); i++ ){
                        pTreeSubItem = new QTreeWidgetItem;
                        QString InfoText;

                        if( i->task == tlData::enuBreak ){
                            InfoText = "Pause";
                        }
                        else{
                            InfoText = i->TaskName + " (" + i->TaskSubName + ")";
                        }

                        pTreeSubItem->setText( 0, InfoText );
                        pTreeSubItem->setText( 1, tlTools::formatWorkTime( i->time_sec ) );

                        pTreeItem->addChild( pTreeSubItem );
                    }
                }
                ui->twOverview->addTopLevelItem( pTreeItem );
            }
        }
    }


    ui->twOverview->setColumnCount( 2);
    QStringList HeaderLabel;
    HeaderLabel.push_back( "Datum" );
    HeaderLabel.push_back( "Zeit" );
    ui->twOverview->setHeaderLabels( HeaderLabel );

    // print big overwiew
    tlData::WorkSummery_t ws = pData->GetWorktimeSummeryEx( ui->deDateStart->date(), ui->deDateEnd->date() );
    QString WorkInfo;

    WorkInfo = ui->deDateStart->date().toString() + " - " + ui->deDateEnd->date().toString();
    WorkInfo += "\r\n";
    WorkInfo += "Arbeitszeit:\t" + tlTools::formatWorkTime( ws.TimeWork_sec );

    ui->teSummery->setText( WorkInfo );
}
