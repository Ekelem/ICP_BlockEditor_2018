#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    actual_project_m = new project();
    scene_m = new QGraphicsScene(this);
    scene_m->addItem(new Canvas_Graphics(nullptr, actual_project_m));
    ui->setupUi(this);
    setWindowState(Qt::WindowMaximized);
    setWindowIcon(QIcon(":/icons/icon-c++.svg"));

    ui->Frame_BlockPalette->layout()->setAlignment(Qt::AlignTop);
    ui->canvas->setScene(scene_m);

    //block * ptr = new b_add_kg_to_kg(actual_project_m->get_type_lib());
    //scene_m->addWidget(new Block_UI(nullptr, ptr, "abc"));
    QPainterPath path = QPainterPath();
    path.cubicTo(20, 0, 30, 50, 50, 50);
    QGraphicsPathItem * ptr = scene_m->addPath(path, QPen(Qt::green, 3, Qt::SolidLine), QBrush(Qt::black));
    //ptr->setPath();


    //ui->main_field->mark_project(actual_project_m);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete actual_project_m;
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
