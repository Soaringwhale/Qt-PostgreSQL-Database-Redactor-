#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlTableModel>
#include <QTableView>
#include <QHeaderView>
#include "database.h"
#include "dialogwindow.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

 private:
    DataBase   *db;                    //  объект нашего класса для бд
    QTableView *tableView;         //  обычный вид  QTableView
    QSqlTableModel  *model;      //   модель  QSqlTableModel
    DialogWindow dialog;
    bool forEdit = false;

    void setupSQLTableModel(QString tableName,   QStringList headers);        //  методы настройки  SQLTableModel   и  TableView
    void setupTableView();

    void insertRow();              //  методы добавления и изменения строк в БД
    void updateRow();

private slots:
    void deleteRowSlot();
    void onEditButtonClickedSlot();
    void onAddButtonClickedSlot();
    void onSaveButtonClickedSlot();

};

#endif    // MAINWINDOW_H
