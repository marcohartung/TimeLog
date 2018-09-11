#include "edittaskdlg.h"
#include "ui_edittaskdlg.h"

EditTaskDlg::EditTaskDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditTaskDlg)
{
    ui->setupUi(this);
}

EditTaskDlg::~EditTaskDlg()
{
    delete ui;
}

 void EditTaskDlg::SetData( const tlData::worktask_t& defvalues )
 {

     if( !defvalues.TaskName.isEmpty() ){
        ui->cbProj->addItem( defvalues.TaskName );
     }
     if( !defvalues.TaskSubName.isEmpty() ){
        ui->cbSubProj->addItem( defvalues.TaskSubName );
     }
    // TODO
//     if( !defvalues.timeSpan != tlData::invalidTime ){
//        ui->teSpan->addText( defvalues.timeSpan );
//     }
//     if( !defvalues.timeStart != tlData::invalidTime ){
//        ui->teStart->setTime( defvalues.timeStart );
//     }
//     if( !defvalues.timeStop != tlData::invalidTime ){
//        ui->teStop->setTime( defvalues.timeStop );
//     }
 }
