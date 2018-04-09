#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>

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

/*
 * Event for New File
 * Question: Should ask for saving the actual state?
 */
void MainWindow::on_actionNew_File_triggered()
{

}


/*
 * Event for Load File
 * Question: Should ask for saving the actual state?
 * Question: Where should save all the data from the file?
 */
void MainWindow::on_actionLoad_File_triggered()
{
    QString filename=QFileDialog::getOpenFileName(this, tr("Open File"), "", "All files (*.*);;");
    if (filename != "")
        QMessageBox::information(this, tr("File Name"), filename);
}


/*
 * Event for Save File
 */
void MainWindow::on_actionSave_File_triggered()
{

}


/*
 * Quit application
 */
void MainWindow::on_actionQuit_triggered()
{
    QMessageBox::StandardButton reply;

    reply = QMessageBox::question(this, "This file has changes, do you want to save them?", "Your changes will be lost if you close this item without saving.",
        QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

    if (reply == QMessageBox::Save) {
        QMessageBox::information(this, tr("Saved"), "");
        close();
        // TODO - Call NewFile trigger
    }
    if (reply == QMessageBox::Discard)
    {
        close();
    }
    if(reply == QMessageBox::Cancel)
    {
        return;
    }
}
