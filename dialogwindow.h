#ifndef DIALOGWINDOW_H
#define DIALOGWINDOW_H

#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QLayout>
#include <QSpacerItem>

class DialogWindow : public QWidget
{
    Q_OBJECT
public:
    explicit DialogWindow(QWidget *parent = nullptr);

    friend class MainWindow;             //  для доступа из MainWindow к закрытым полям

private:
    QPushButton *buttonSave;
    QLineEdit* lineedit1;
    QLineEdit* lineedit2;
    QLineEdit* lineedit3;

};

#endif // DIALOGWINDOW_H
