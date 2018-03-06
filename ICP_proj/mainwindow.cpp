#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowState(Qt::WindowMaximized);
    setWindowIcon(QIcon(":/icons/icon-c++.svg"));

    ui->Frame_BlockPalette->layout()->setAlignment(Qt::AlignTop);

    QTreeWidgetItem *aritmetic = new QTreeWidgetItem(ui->Palette);
    aritmetic->setText(0, tr("Aritmetic"));

    QTreeWidgetItem *ADD = new QTreeWidgetItem(aritmetic);
        ADD->setText(0, tr("ADD"));
    QTreeWidgetItem *SUB = new QTreeWidgetItem(aritmetic);
        SUB->setText(0, tr("SUB"));
}

MainWindow::~MainWindow()
{
    delete ui;
}
