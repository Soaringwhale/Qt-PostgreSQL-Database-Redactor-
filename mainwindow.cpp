#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{

    setFixedSize(650,350);                        // фиксированный размер гл. окна

    db = new DataBase();                        //  объект, который будет использоваться для подключения и дальнейшей работы с БД
    db->connectToDataBase();                //  подключаемся/создаем бд

    for (int i = 0; i < 10; i++) {
         QVariantList data;
         data.append(i+1);                                      //     ЗАПОЛНЯЕМ СТРОКАМИ qual_table
         data.append(i+10);
         db->inserIntoQualTable(data);             //  каждый раз вызываем inserIntoTable()  с передачей массива данных
    }

    for (int i = 0; i < 5; i++) {
         QVariantList data;
         data.append(i+12);                                           //  ЗАПОЛНЯЕМ СТРОКАМИ main_table
         data.append(i+1);
         data.append(i+11);
         db->inserIntoMainTable(data);
    }

    this->setupSQLTableModel( TABLE2,   QStringList() <<"main_table.i"  << "qual_table.p01"   << "main_table.t"  );         // передаем имя таблицы main_table, и массив заголовков (который заполняем прямо здесь)

    tableView = new QTableView(this);
    this->setupTableView();                       //  инициализируем ВИД  (QTableView)

    QPushButton *btnInsert = new QPushButton("Добавить", this);
    QPushButton *btnDelete = new QPushButton("Удалить",this);
    QPushButton *btnUpdate = new QPushButton("Редактировать",this);

    QHBoxLayout *hlayot = new QHBoxLayout();     // горизонтальный слой с кнопками
    QVBoxLayout *vlayot = new QVBoxLayout();         // таблица + слой с кнопками

    hlayot->addWidget(btnInsert);
    hlayot->addWidget(btnDelete);          // горизонтальный слой кнопок
    hlayot->addWidget(btnUpdate);
    hlayot->addSpacing(300);

    vlayot->addLayout(hlayot);                    //  вертикальный слой,  кнопки + таблица
    vlayot->addWidget(tableView);

    QWidget *placeholderWidget = new QWidget;        //  стандартный приём, добавляем главный слой на простой виджет, и этот виджет делаем центральным. Т.к.  setCentralWidget принимает лишь виджет.
    placeholderWidget->setLayout(vlayot);
    setCentralWidget(placeholderWidget);


    connect(btnInsert, SIGNAL(clicked()), this, SLOT(onAddButtonClickedSlot()));
    connect(btnUpdate, SIGNAL(clicked()), this, SLOT(onEditButtonClickedSlot()));                //  при нажатии на кнопки
    connect(tableView, SIGNAL(doubleClicked(const QModelIndex)), this, SLOT(onEditButtonClickedSlot()));
    connect(btnDelete, SIGNAL(clicked()), this, SLOT(deleteRowSlot()));
    connect(dialog.buttonSave, SIGNAL(clicked()), this, SLOT(onSaveButtonClickedSlot()));

}


void MainWindow::setupSQLTableModel(QString tableName,   QStringList headers)          //    метод  настройки  QSqlTableModel ,   принимает имя таблицы и заголовки
{
    model = new QSqlTableModel(this);
    model->setTable(tableName);                       //  этот встроенный метод QSqlTableModel задает имя таблицы,  ведь QSqlTableModel работает только с одной таблицей.

    for(int i = 0;  i < model->columnCount();  i++){            //  Устанавливаем названия столбцов из принятого массива
        model->setHeaderData( i,  Qt::Horizontal,  headers[i]);
    }
    model->setSort(0, Qt::AscendingOrder);                    // Устанавливаем сортировку по возрастанию данных по нулевой колонке
}



void MainWindow::setupTableView()               //  метод настройки TableView
{
   tableView->setModel(model);                         // Устанавливаем модель  QSqlTableModel  на TableView

   tableView->setSelectionBehavior(QAbstractItemView::SelectRows);          // Разрешаем выделение строк
   tableView->setSelectionMode(QAbstractItemView::SingleSelection);          // Устанавливаем режим выделения лишь одной строки в таблице
   tableView->resizeColumnsToContents();                         // Устанавливаем размер колонок по содержимому

   tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
   tableView->horizontalHeader()->setStretchLastSection(true);                //  последний столбец растягивается на все оставшееся пространство

    model->select();                                 // делаем выборку данных из main_table и обновляемся, что-бы видеть изменения.
    tableView->setFixedSize(650,300);
}


void MainWindow::onEditButtonClickedSlot()
{
    if(tableView->currentIndex().isValid()) {                   //  проверка выделено ли что-нибудь.
      forEdit = true;
      dialog.buttonSave->setText("Сохранить");

      QString str1 = model->index(tableView->currentIndex().row(), 0, QModelIndex()).data().toString();                 //  получаем текст элементов
      QString str2 = model->index(tableView->currentIndex().row(), 1, QModelIndex()).data().toString();
      QString str3 = model->index(tableView->currentIndex().row(), 2, QModelIndex()).data().toString();

      dialog.show();                                                           //  открываем окно для редактирования.
      dialog.lineedit1->setText(str1);
      dialog.lineedit2->setText(str2);
      dialog.lineedit3->setText(str3);                         //   поля редактирования изначально будут заполнены текущими данными.
    }
}

void MainWindow::onAddButtonClickedSlot()
{
    forEdit = false;
    dialog.buttonSave->setText("Добавить строку");
    dialog.show();                                                                    //  открываем окно для добавления.
    dialog.lineedit1->clear();
    dialog.lineedit2->clear();
    dialog.lineedit3->clear();
}

void MainWindow::onSaveButtonClickedSlot()
{
    if(forEdit) { updateRow(); }                                       //  вызовет либо изменение либо добавление строки, в зависимости от ситуации.
    else insertRow();
}


void MainWindow::insertRow()
{
    QSqlQuery query;

    query.prepare( "INSERT INTO "  TABLE2 " ( i, r, t ) "
                                           "VALUES  (?,?,?)");
    query.addBindValue(dialog.lineedit1->text().toInt());
    query.addBindValue(dialog.lineedit2->text());
    query.addBindValue(dialog.lineedit3->text());

    if(!query.exec()){
         QMessageBox::critical(0, "Не удалось добавить строку", query.lastError().text());
    }
    else {
    model->select();          //  обновляем модель, что-бы увидеть изменения
    dialog.close();
    QMessageBox::information(0, "Ок", "Данные успешно добавлены в базу");
    }
}



void MainWindow::updateRow()
{
    QModelIndex ind = model->index(tableView->currentIndex().row(), 0, QModelIndex());          //  получаем индекс первого элемента выделенной строки (он уникальный, поэтому будем по нему искать)
    QString str = ind.data().toString();

    QSqlQuery query;

    query.prepare( "UPDATE "  TABLE2 " SET i = ?, r = ?, t = ?  WHERE i = " +str+";");
    query.addBindValue(dialog.lineedit1->text().toInt());
    query.addBindValue(dialog.lineedit2->text());
    query.addBindValue(dialog.lineedit3->text());

    if(!query.exec()){
        QMessageBox::critical(0, "Не удалось изменить строку", query.lastError().text());
    }
    else {
    model->select();                   //  обновляем модель, что-бы увидеть изменения
    dialog.close();
    QMessageBox::information(0, "Ок", "Данные успешно изменены");
    }
}


void MainWindow::deleteRowSlot(){

   QModelIndex ind = model->index(tableView->currentIndex().row(), 0, QModelIndex());          //  получаем индекс первого элемента выделенной строки (он уникальный, поэтому будем по нему искать)
   QString str = ind.data().toString();

    QSqlQuery query;
    if(!query.exec( "DELETE FROM " TABLE2 " WHERE i = " +str+";"))                      //  удаляем строку по уникальному индексу
    {
        QMessageBox::critical(0, "Не удалось удалить строку", query.lastError().text());
    }
    model->select();          //  обновляем модель, что-бы увидеть изменения
}


MainWindow::~MainWindow() { }
