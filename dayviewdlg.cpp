#include "dayviewdlg.h"
#include "ui_dayviewdlg.h"
#include "tltools.h"

#include <QtCore/QVariant>

DayViewDlg::DayViewDlg(QWidget *parent) :
    QDialog(parent, Qt::WindowSystemMenuHint | Qt::WindowTitleHint ),
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
    connect( ui->pbNextDay, SIGNAL(clicked()), this, SLOT(on_NextDay_clicked()) );
    connect( ui->pbPrevDay, SIGNAL(clicked()), this, SLOT(on_PrevDay_clicked()) );
    connect( ui->deDispDay, SIGNAL(editingFinished()), this, SLOT(UpdateView()) );
    connect( ui->twDayContent, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(ShowContextMenu(const QPoint &)));
    connect( ui->twDayContent, SIGNAL(itemChanged(QTreeWidgetItem *, int)), this, SLOT(on_twDayContent_itemChanged(QTreeWidgetItem*,int)));

    ui->deDispDay->setWrapping( true );
    ui->deDispDay->setCalendarPopup( true );
    ui->deDispDay->setDate( QDate::currentDate() );

    ui->twDayContent->blockSignals(true);
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
    ui->twDayContent->blockSignals(false);
}

DayViewDlg::~DayViewDlg()
{
    delete ui;
}

void DayViewDlg::SetData( tlData* pd ){
    pData = pd;
    UpdateView();
}

bool DayViewDlg::WriteViewToDB( void ){

    int tlic = ui->twDayContent->topLevelItemCount();
    int cchild;
    QTreeWidgetItem* pTreeItem;
    QTreeWidgetItem* pTreeSubItem;
    bool fault_occurred = false, item_ok;

    ui->twDayContent->blockSignals(true);
    for( int i = 0; i < tlic; i++ ){
        pTreeItem = ui->twDayContent->topLevelItem( i );

        if( pTreeItem == 0 ){
            qCritical( "pTreeItem = null; i = %d",  i );
        }
        else if( pTreeItem->flags() & Qt::ItemIsEditable ){
            tlData::worktime_t worktime;
            QTime StartTime, EndTime;
            item_ok = false;

            worktime.id = pTreeItem->data( 0, Qt::UserRole ).toLongLong();
            StartTime = tlTools::StringToTime( pTreeItem->text( 3 ) );
            EndTime = tlTools::StringToTime( pTreeItem->text( 4 ) );
            if( StartTime.isValid() && EndTime.isValid() ){
                if( EndTime.msecsSinceStartOfDay() > StartTime.msecsSinceStartOfDay() ){
                    worktime.timeStart = StartTime;
                    worktime.timeStop = EndTime;
                    item_ok = true;
                }
                else{
                    pTreeItem->setTextColor( 3, QColor(255,0,0,255) );
                    pTreeItem->setTextColor( 4, QColor(255,0,0,255) );
                }
            }
            if( item_ok ){

                if( worktime.id  == 0 ){  // new item
                    if( pData->AddWorkTime( day, worktime ) == false ){
                        // TODO ERROR msg
                    }
                }
                else{           // modified item
                    if( pData->UpdateWorkTime( worktime ) == false ){
                        // TODO ERROR msg
                    }
                }
                pTreeItem->setFlags( pTreeItem->flags() & ~Qt::ItemIsEditable );
            }
            else{
                fault_occurred = true;
            }
        }


        cchild = pTreeItem->childCount();
        for( int ii = 0; ii < cchild; ii++ ){
            pTreeSubItem = pTreeItem->child( ii );

            if( pTreeSubItem == 0 ){
                qCritical( "pTreeSubItem = null; ii = %d", ii );
            }
            else if( pTreeSubItem->flags() & Qt::ItemIsEditable ){
                qint64 id = pTreeSubItem->data( 0, Qt::UserRole ).toLongLong();

                // check item
                item_ok = false;
                tlData::worktask_t taskdata;
                QTime StartTime, EndTime, Span;

                taskdata.id = id;
                if( pTreeSubItem->text( 0 ) == "Pause" ){
                    taskdata.task = tlData::enuBreak;
                }
                else{
                    taskdata.task = tlData::enuProject;
                    taskdata.TaskName = pTreeSubItem->text( 0 );
                    taskdata.TaskSubName = pTreeSubItem->text( 1 );
                }

                StartTime = tlTools::StringToTime( pTreeSubItem->text( 3 ) );
                EndTime = tlTools::StringToTime( pTreeSubItem->text( 4 ) );
                Span = tlTools::StringToTime( pTreeSubItem->text( 2 ) );
                if( StartTime.isValid() && EndTime.isValid() ){
                    if( EndTime.msecsSinceStartOfDay() > StartTime.msecsSinceStartOfDay() ){
                        taskdata.timeSpan = -1;
                        taskdata.timeStart = StartTime;
                        taskdata.timeStop = EndTime;
                        item_ok = true;
                    }
                    else{
                        pTreeSubItem->setTextColor( 3, QColor(255,0,0,255) );
                        pTreeSubItem->setTextColor( 4, QColor(255,0,0,255) );
                    }
                }
                else if( Span.isValid() ){
                    int secs = Span.msecsSinceStartOfDay() / 1000;
                    if( secs > 0 && secs < (24*60*60) ){
                        taskdata.timeSpan = secs;
                        taskdata.timeStart = tlData::invalidTime;
                        taskdata.timeStop = tlData::invalidTime;
                        item_ok = true;
                    }
                    else{
                        pTreeSubItem->setTextColor( 2, QColor(255,0,0,255) );
                    }
                }
                else{
                    pTreeSubItem->setTextColor( 2, QColor(255,0,0,255) );
                    pTreeSubItem->setTextColor( 3, QColor(255,0,0,255) );
                    pTreeSubItem->setTextColor( 4, QColor(255,0,0,255) );
                }


                if( item_ok ){

                    if( id == 0 ){  // new item
                        if( pData->AddWorkTask( day, taskdata ) == false ){
                            // TODO ERROR msg
                        }
                    }
                    else{           // modified item
                        if( pData->UpdateWorkTask( taskdata ) == false ){
                            // TODO ERROR msg
                        }
                    }

                    pTreeSubItem->setFlags( pTreeSubItem->flags() & ~Qt::ItemIsEditable );
                }
                else{
                    fault_occurred = true;
                }
            }
        }
    }
    ui->twDayContent->blockSignals(false);
    ui->twDayContent->clearSelection();

    return !fault_occurred;
}

void DayViewDlg::accept(){

    if( WriteViewToDB() == true ){
        this->setResult( QDialog::Accepted );
        this->close();
    }
}

void DayViewDlg::rejected(){

}

void DayViewDlg::on_NextDay_clicked(){

    if( WriteViewToDB() == true ){
        ui->deDispDay->setDate( ui->deDispDay->date().addDays(1) );
        UpdateView();
    }
}

void DayViewDlg::on_PrevDay_clicked(){

    if( WriteViewToDB() == true ){
        ui->deDispDay->setDate( ui->deDispDay->date().addDays(-1) );
        UpdateView();
    }
}

void DayViewDlg::UpdateView( ){

    ui->twDayContent->blockSignals(true);
    ui->twDayContent->clear();
    day.setDate( -1, -1 ,-1 );

    if( pData ){
        QTreeWidgetItem* pTreeItem;
        QTreeWidgetItem* pTreeSubItem;
        QVector<tlData::worktime_t>::iterator i;
        QVector<tlData::worktask_t>::iterator ii;

        day = ui->deDispDay->date();
        QVector<tlData::worktime_t> workday = pData->GetWorktimesOfDay( day );

        if( !workday.empty() ){

            for( i = workday.begin(); i < workday.end(); i++ ){
                pTreeItem = new QTreeWidgetItem;

                pTreeItem->setData( 0, Qt::UserRole, QVariant( i->id ) );
                pTreeItem->setText( 0, "" );
                pTreeItem->setText( 1, "" );

                pTreeItem->setText( 3, i->timeStart.toString() );
                if( i->timeStop != tlData::invalidTime ){
                    pTreeItem->setText( 4, i->timeStop.toString() );
                    pTreeItem->setText( 2, tlTools::TimesToSpanString( i->timeStart, i->timeStop ) );
                }
                else{
                    pTreeItem->setText( 4, "--" );
                    pTreeItem->setText( 2, "--" );
                }

                for( ii = i->tasks.begin(); ii < i->tasks.end(); ii++ ){
                    pTreeSubItem = new QTreeWidgetItem;

                    pTreeSubItem->setData( 0, Qt::UserRole, QVariant( ii->id ) );
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
                ui->twDayContent->expandAll();
            }
        }

//        QVector<tlData::project_t> projecsts = pData->GetProjectList();
//        for( size_t i = 0; i < projecsts.size(); i++ ){
//            ui->cbAddProj->addItem( projecsts[i].Name );
//        }
    }

    ui->twDayContent->blockSignals(false);
}

void DayViewDlg::ShowContextMenu( const QPoint &pos )
{
    // for most widgets
    // QPoint globalPos = myWidget->mapToGlobal(pos);
    // for QAbstractScrollArea and derived classes you would use:
    QPoint globalPos = ui->twDayContent->viewport()->mapToGlobal(pos);

    QModelIndex index = ui->twDayContent->indexAt(pos);
    if( index.isValid() ){
        cmaDayContentEdit->setDisabled( false );
    }
    else{
        cmaDayContentEdit->setDisabled( true );
    }
    cmDayContent->exec( globalPos );
}

void DayViewDlg::EditData(  ){
    QPoint pos = ui->twDayContent->viewport()->mapFromGlobal( cmDayContent->pos() );
    ui->twDayContent->blockSignals(true);
    QTreeWidgetItem* pTreeItem =  ui->twDayContent->itemAt( pos );
    pTreeItem->setFlags( pTreeItem->flags() | Qt::ItemIsEditable);
    ui->twDayContent->editItem( pTreeItem, ui->twDayContent->columnAt( pos.x() ) );
    ui->twDayContent->blockSignals(false);
}

void DayViewDlg::AddData(  ){

    ui->twDayContent->blockSignals(true);
    if( ui->twDayContent->topLevelItemCount() <= 0 ){
        QTreeWidgetItem* pTreeItem = new QTreeWidgetItem;
        pTreeItem->setData( 0, Qt::UserRole, QVariant( 0 ) );
        pTreeItem->setText( 0, "" );
        pTreeItem->setText( 1, "" );
        pTreeItem->setText( 2, tlTools::formatWorkTime( 0 ) );
        pTreeItem->setText( 3, tlTools::formatWorkTime( 0 )  );
        pTreeItem->setText( 4, tlTools::formatWorkTime( 0 ) );
        pTreeItem->setFlags( pTreeItem->flags() | Qt::ItemIsEditable);
        ui->twDayContent->addTopLevelItem( pTreeItem );
        ui->twDayContent->editItem( pTreeItem, 3 );
    }
    else{
        QTreeWidgetItem* pTreeSubItem = new QTreeWidgetItem;
         pTreeSubItem->setData( 0, Qt::UserRole, QVariant( 0 ) );
        pTreeSubItem->setText( 0, "Project" );
        pTreeSubItem->setText( 1, "" );
        pTreeSubItem->setText( 2, tlTools::formatWorkTime( 0 )  );
        pTreeSubItem->setText( 3, "--" );
        pTreeSubItem->setText( 4, "--" );
        pTreeSubItem->setFlags( pTreeSubItem->flags() | Qt::ItemIsEditable);
        ui->twDayContent->topLevelItem( 0 )->addChild( pTreeSubItem );
        ui->twDayContent->editItem( pTreeSubItem, 0 );
    }
    ui->twDayContent->blockSignals(false);
}

void DayViewDlg::on_twDayContent_itemChanged( QTreeWidgetItem *item, int column )
{
    if( column == 2 || column == 3 || column == 4 ){

        QString strT = "--";
        QTime timeT;

        if( !item->text(column).isEmpty() ){
            timeT = tlTools::StringToTime( item->text(column) );
            if( timeT.isValid() ){
                strT = timeT.toString();
            }
        }
        ui->twDayContent->blockSignals(true);
        item->setText( column, strT );
        ui->twDayContent->blockSignals(false);
    }
}
