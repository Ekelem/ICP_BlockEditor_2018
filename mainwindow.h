#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <QRadioButton>
#include "myarea.h"

enum blockTypes {
    B_ADD, B_SUB, B_VALUE
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void setup();

private:
    QWidget *p;
    MyArea *MainTable;
    QVBoxLayout *box;
    QHBoxLayout *main;
    QGroupBox *groupModeBox;
    QVBoxLayout *modeGroupVBox;
    QVBoxLayout *modeGroupVValue;
    QGroupBox *groupBlockBox;
    QLineEdit *edit;


    QPushButton *createBlock;
    QRadioButton *radio1;
    QRadioButton *radio2;

    QRadioButton *radioADD;
    QRadioButton *radioSUB;
    QRadioButton *radioValue;

    void createObjects();
    void connectSignals();


private slots:
    void create_block();
    void setObjectsToMoveable();
    void setObjectsToConnectable();
    void setActualBlock(int type);
    void valueChanged();

};

#endif // MAINWINDOW_H
