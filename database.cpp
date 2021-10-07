#include "database.h"

DataBase::DataBase(QObject *parent) : QObject(parent) { }

DataBase::~DataBase() {delete this; }


void DataBase::connectToDataBase()                                          //  метод подключения к БД.   Если нашей бд не существовало, она будет создана и подключена.
{
         QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");              //  драйвер
         db.setUserName("postgres");
         db.setPassword("hhhesoyam");
         db.setHostName("127.0.0.1");              // или  localhost
         db.setPort(5432);
         db.setDatabaseName("template1");       //  сперва подключаемся к встроенной бд, что-бы иметь возможность создать нашу.
         if (db.open())  {
               if(tryCreateNewDataBase()){
                      QMessageBox::information(0, "Ок", "База данных " DB_NAME " создана. Подключение прошло успешно.");              //  пытаемся создать нашу БД, если она уже существует, то не будет создана.
               }
               db.setDatabaseName(DB_NAME);                                                                                                                                   //  переподключаемся к ней.
               if (!db.open()) { QMessageBox::critical(0, "Ошибка", "Не удалось подключиться к базе данных " DB_NAME); }
               else { QMessageBox::information(0, "Ок", "Успешное подключение к базе данных " DB_NAME); }
         }
         else { QMessageBox::critical(0, "Ошибка", "Не удалось подключиться к серверу."); }

      createTables();
}

void DataBase::createTables()                       // Метод  создания таблиц
{
    QSqlQuery query;
    if(!query.exec( "CREATE TABLE IF NOT EXISTS " TABLE1 " ("                       //   создаем qual_table  ,  если она не создана.
                            "p00    CHARACTER VARYING(2) UNIQUE, "
                            "p01    TEXT  "
                        "  )"  )) {
        qDebug() << "DataBase: error of create " << TABLE1;                     //  вывод возможной ошибки при создании.
        qDebug() << query.lastError().text();
    }

    if(!query.exec( "CREATE TABLE IF NOT EXISTS " TABLE2 " ("                                              //  создаем main_table
                            "i    int4 PRIMARY KEY, "
                            "r   CHARACTER VARYING(2) REFERENCES " TABLE1 " (p00)  ON UPDATE CASCADE , "
                            "t    TEXT  "
                        "  )"  )) {
        qDebug() << "DataBase: error of create " << TABLE2;
        qDebug() << query.lastError().text();
    }
}

bool DataBase::inserIntoQualTable(const QVariantList  &data)                  //   метод вставки строки в таблицу qual_table ,  принимает QVariantList  т.к.  значения в таблицах могут быть разных типов.
{
    QSqlQuery query;

    query.prepare( "INSERT INTO "  TABLE1 " ( p00,  p01 ) "            //   готовим запрос
                                           "VALUES  (?,?)");
    query.addBindValue(data[0].toString());
    query.addBindValue(data[1].toString());

    if(!query.exec()){                                                                       //  выполняем запрос методом exec()
        qDebug() << "error insert into " << TABLE1;
        qDebug() << query.lastError().text();                                       //  в этом методе предусмотрен вывод информации об ошибках.
        return false;
    } else {
        return true;
    }
}

bool DataBase::inserIntoMainTable(const QVariantList  &data)                  //   метод вставки строки в таблицу main_table
{
    QSqlQuery query;

    query.prepare( "INSERT INTO "  TABLE2 " ( i, r, t ) "
                                           "VALUES  (?,?,?)");
    query.addBindValue(data[0].toInt());
    query.addBindValue(data[1].toInt());
    query.addBindValue(data[2].toString());

    if(!query.exec()){
        qDebug() << "error insert into " << TABLE2;
        qDebug() << query.lastError().text();
        return false;
    } else {
        return true;
    }
}

bool DataBase::tryCreateNewDataBase()
{
    QSqlQuery *query = new QSqlQuery(db);
    if(!query->exec( "CREATE DATABASE " DB_NAME ";")) {
        qDebug() << "DataBase: error of create database " << DB_NAME;                     //  если такая БД уже существует, эта информация будет выведена в отладке.
        qDebug() << query->lastError().text();
        return false;
    }
    else return true;
}

void DataBase::closeDataBase()  {  db.close();  }
