#include "dayviewdlg.h"
#include "ui_dayviewdlg.h"
#include "tltools.h"

DayViewDlg::DayViewDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DayViewDlg)
{
    ui->setupUi(this);

    pData = 0;

    ui->deDispDay->setDate( QDate::currentDate() );
}

DayViewDlg::~DayViewDlg()
{
    delete ui;
}

void DayViewDlg::SetData( tlData* pd ){
    pData = pd;
    UpdateView();
}

void DayViewDlg::UpdateView( ){

    ui->twDayContent->clear();
    ui->cbAddProj->clear();
    ui->cbAddSubProj->clear();
    ui->teAddSpan->clear();
    ui->teAddStart->clear();
    ui->teAddStop->clear();

    if( pData ){
        QTreeWidgetItem* pTreeItem;
        QTreeWidgetItem* pTreeSubItem;
        QVector<tlData::worktime_t>::iterator i;
        QVector<tlData::worktask_t>::iterator ii;

        QVector<tlData::worktime_t> workday = pData->GetWorktimesOfDay( ui->deDispDay->date() );

        if( !workday.empty() ){

            for( i = workday.begin(); i < workday.end(); i++ ){
                pTreeItem = new QTreeWidgetItem;
                pTreeItem->setText( 0, "" );
                pTreeItem->setText( 1, "" );
                pTreeItem->setText( 2, i->timeStart.toString() );
                pTreeItem->setText( 3, i->timeStop.toString() );
                pTreeItem->setText( 3, i->timeStop.toString() );

                for( ii = i->tasks.begin(); ii < i->tasks.end(); ii++ ){
                    pTreeSubItem = new QTreeWidgetItem;

                    pTreeSubItem->setText( 0, ii->TaskName );
                    pTreeSubItem->setText( 1, ii->TaskSubName );
                    pTreeSubItem->setText( 2, ii->timeStart.toString() );
                    pTreeSubItem->setText( 3, ii->timeStop.toString() );
                    pTreeSubItem->setText( 4, tlTools::formatWorkTime( ii->timeSpan ) );
                    pTreeItem->addChild( pTreeSubItem );
                }

                ui->twDayContent->addTopLevelItem( pTreeItem );
            }
        }
    }
}
