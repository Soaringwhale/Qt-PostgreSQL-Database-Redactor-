#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QDebug>
#include <QFile>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>


#define TABLE1      "qual_table"           //  наши таблицы.    main_table  будет зависить от qual_table.
#define TABLE2      "main_table"
#define DB_NAME   "mypgdb2"


class DataBase : public QObject
{
    Q_OBJECT
public:
    DataBase(QObject *parent = 0);

    void connectToDataBase();                                      //  публичный метод - соединиться с бд
    bool inserIntoQualTable(const QVariantList &data);          // публичный метод - вставить в таблицу
    bool inserIntoMainTable(const QVariantList &data);

    ~DataBase();

private:
    QSqlDatabase   db;                         // Сам объект базы данных

    void closeDataBase();               //  закрыть бд
    void createTables();                  //   создать таблицу
    bool tryCreateNewDataBase();
};

#endif   // DATABASE_H
