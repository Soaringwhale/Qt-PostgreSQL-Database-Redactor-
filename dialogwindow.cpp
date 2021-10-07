#include "dialogwindow.h"

DialogWindow::DialogWindow(QWidget *parent) : QWidget(parent)
{
        setFixedSize(400,200);

        QLabel *label1 = new QLabel(this);
        label1->setText("Поле main_table.i");

        QLabel *label2 = new QLabel(this);
        label2->setText("Поле qual_table.p01 связанное через main_table.r с qual_table.p00");

        QLabel *label3 = new QLabel(this);
        label3->setText("Поле main_table.t");

       lineedit1 = new QLineEdit(this);
       lineedit2 = new QLineEdit(this);
       lineedit3 = new QLineEdit(this);

        buttonSave = new QPushButton("Сохранить",this);
        QPushButton *buttonCancel = new QPushButton("Отмена",this);

        QHBoxLayout *hlayout = new QHBoxLayout();
        hlayout->addSpacing(200);
        hlayout->addWidget(buttonSave);
        hlayout->addWidget(buttonCancel);

        QVBoxLayout *vlayout = new QVBoxLayout(this);

        vlayout->addWidget(label1);
        vlayout->addWidget(lineedit1);
        vlayout->addWidget(label2);
        vlayout->addWidget(lineedit2);
        vlayout->addWidget(label3);
        vlayout->addWidget(lineedit3);
        vlayout->addLayout(hlayout);

      connect(buttonCancel, SIGNAL(clicked()), this, SLOT(close()));

}
