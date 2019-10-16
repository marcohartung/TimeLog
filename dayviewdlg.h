/* TimeLog - Copyright (C) 2019  Marco Hartung
 *
 * This file is part of TimeLog.
 *
 * TimeLog is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * TimeLog is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with TimeLog.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef DAYVIEWDLG_H
#define DAYVIEWDLG_H

#include <QDialog>
#include <QMenu>
#include "tlData.h"

class QTreeWidgetItem;

namespace Ui {
class DayViewDlg;
}

class DayViewDlg : public QDialog
{
    Q_OBJECT

public:
    explicit DayViewDlg(QWidget *parent = 0);
    ~DayViewDlg();

    void SetData( tlData* pd );

private:

    bool WriteViewToDB( void );

    Ui::DayViewDlg *ui;
    QMenu* cmDayContent;
    QAction* cmaDayContentEdit;
    QAction* cmaDayContentAdd;

    tlData* pData;
    QDate day;

 private slots:
    void accept();
    void rejected();
    void on_NextDay_clicked();
    void on_PrevDay_clicked();
    void UpdateView( );
    void on_twDayContent_itemChanged( QTreeWidgetItem *item, int column );
    void ShowContextMenu( const QPoint &pos );
    void AddData( );
    void EditData( );

};

#endif // DAYVIEWDLG_H
