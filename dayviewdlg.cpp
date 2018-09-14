#include "dayviewdlg.h"
#include "ui_dayviewdlg.h"
#include "tltools.h"

#include <QtCore/QVariant>

DayViewDlg::DayViewDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DayViewDlg)
{
    ui->setupUi(this);

    pData = 0;

    cmDayContent = new QMenu( tr("Context menu"), this );
    cmaDayContentEdit = new QAction("Bearbeiten", this);
    cmaDayContentAdd = new QAction("Hinzufügen", this);
    cmDayContent->addAction(cmaDayContentEdit);
    cmDayContent->addAction(cmaDayContentAdd);

    connect(cmaDayContentEdit, SIGNAL(triggered()), this, SLOT(EditData()));
    connect(cmaDayContentAdd, SIGNAL(triggered()), this, SLOT(AddData()));
    connect( ui->pbNextDay, SIGNAL(clicked()), ui->deDispDay, SLOT(stepUp()) );
    connect( ui->pbPrevDay, SIGNAL(clicked()), ui->deDispDay, SLOT(stepDown()) );
    connect( ui->deDispDay, SIGNAL(editingFinished()), this, SLOT(UpdateView()) );
    connect( ui->twDayContent, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(ShowContextMenu(const QPoint &)));

    ui->deDispDay->setWrapping( true );
    ui->deDispDay->setDate( QDate::currentDate() );

    //ui->twDayContent->setColumnCount( 2 );
    QStringList HeaderLabel;
    HeaderLabel.push_back( "Task/Project" );
    HeaderLabel.push_back( "SubTask" );
    HeaderLabel.push_back( "Time" );
    HeaderLabel.push_back( "Start" );
    HeaderLabel.push_back( "End" );
    ui->twDayContent->setHeaderLabels( HeaderLabel );
    ui->twDayContent->setContextMenuPolicy( Qt::CustomContextMenu );
    ui->twDayContent->setSelectionMode( QAbstractItemView::SingleSelection );
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

                pTreeItem->setText( 2, tlTools::TimesToSpanString( i->timeStart, i->timeStop ) );
                pTreeItem->setText( 3, i->timeStart.toString() );
                pTreeItem->setText( 4, i->timeStop.toString() );

                for( ii = i->tasks.begin(); ii < i->tasks.end(); ii++ ){
                    pTreeSubItem = new QTreeWidgetItem;

                    if( ii->task == tlData::enuBreak ){
                        pTreeSubItem->setText( 0, "Pause" );
                    }
                    else{
                        pTreeSubItem->setText( 0, ii->TaskName );
                        pTreeSubItem->setText( 1, ii->TaskSubName );
                    }
                    if( ii->timeSpan < 0 ){
                        pTreeSubItem->setText( 2, tlTools::TimesToSpanString( ii->timeStart, ii->timeStop ) );
                        pTreeSubItem->setText( 3, ii->timeStart.toString() );
                        pTreeSubItem->setText( 4, ii->timeStop.toString() );
                    }
                    else{
                        pTreeSubItem->setText( 2, tlTools::formatWorkTime( ii->timeSpan ) );
                        pTreeSubItem->setText( 3, "--" );
                        pTreeSubItem->setText( 4, "--" );
                    }
                    pTreeItem->addChild( pTreeSubItem );
                }

                ui->twDayContent->addTopLevelItem( pTreeItem );
            }
        }

//        QVector<tlData::project_t> projecsts = pData->GetProjectList();
//        for( size_t i = 0; i < projecsts.size(); i++ ){
//            ui->cbAddProj->addItem( projecsts[i].Name );
//        }
    }
}

void DayViewDlg::ShowContextMenu( const QPoint &pos )
{
    // for most widgets
    // QPoint globalPos = myWidget->mapToGlobal(pos);
    // for QAbstractScrollArea and derived classes you would use:
    QPoint globalPos = ui->twDayContent->viewport()->mapToGlobal(pos);

    QModelIndex index = ui->twDayContent->indexAt(pos);
    if( index.isValid() ){
         cmDayContent->exec( globalPos );
    }
}

void DayViewDlg::EditData(  ){
    QPoint pos = ui->twDayContent->viewport()->mapFromGlobal( cmDayContent->pos() );
    //QModelIndex index = ui->twDayContent->indexAt( pos );
    QTreeWidgetItem* pTreeItem =  ui->twDayContent->itemAt( pos );
    pTreeItem->setFlags( pTreeItem->flags() | Qt::ItemIsEditable);
    ui->twDayContent->editItem( pTreeItem, ui->twDayContent->columnAt( pos.x() ) );
}

void DayViewDlg::AddData(  ){

    if( ui->twDayContent->topLevelItemCount() <= 0 ){
        QTreeWidgetItem* pTreeItem = new QTreeWidgetItem;
        QTime t;
        pTreeItem->setText( 0, "" );
        pTreeItem->setText( 1, "" );
        pTreeItem->setText( 2, tlTools::formatWorkTime( 0 ) );
        pTreeItem->setText( 3, t.toString()  );
        pTreeItem->setText( 4, t.toString() );
        ui->twDayContent->addTopLevelItem( pTreeItem );
    }

    QTreeWidgetItem* pTreeSubItem;
    pTreeSubItem = new QTreeWidgetItem;

    pTreeSubItem->setText( 0, "proj" );
    pTreeSubItem->setText( 1, "arg" );
    pTreeSubItem->setText( 2, "0:00" );
    pTreeSubItem->setText( 3, "--" );
    pTreeSubItem->setText( 4, "--" );

    pTreeSubItem->setFlags( pTreeSubItem->flags() | Qt::ItemIsEditable);
    ui->twDayContent->topLevelItem( 0 )->addChild( pTreeSubItem );
    ui->twDayContent->editItem( pTreeSubItem, 0 );
}
