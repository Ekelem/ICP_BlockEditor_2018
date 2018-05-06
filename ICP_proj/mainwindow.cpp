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

    for (unsigned int i = 0; i < actual_project_m->get_type_lib().get_type_count(); i++)
    {
        ui->comboBoxTypes->addItem(QString::fromStdString(actual_project_m->get_type_lib().get_type_name(i)));
    }

    scene_m->addItem(new Start_Graphics(nullptr, actual_project_m->get_start_addr()));

    ui->Frame_BlockPalette->layout()->setAlignment(Qt::AlignTop);
    ui->canvas->setScene(scene_m);
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

void MainWindow::on_actionRun_program_triggered()
{
    try
    {
        actual_project_m->run();
        QMessageBox msgBox;
        msgBox.setText("Scheme finnished succesfuly!");
        msgBox.exec();
    }
    catch(exceptions_enum e)
    {
        QMessageBox msgBox;
        switch (e) {
        case cycle:
            msgBox.setText("Cycle detected!");
            msgBox.exec();
            break;
        case unconnected_in:
            msgBox.setText("Unconnected in port detected!");
            msgBox.exec();
            break;
        default:
            break;
        }
    }
}

void MainWindow::on_constantButton_clicked()
{
    //Constant_Graphics()
    bool number = false;
    std::cerr << ui->constantValue->toPlainText().toStdString() << std::endl;
    double value = ui->constantValue->toPlainText().toDouble(&number);
    if (!number)
        throw exceptions_enum::not_a_number;
    scene_m->addItem(new Constant_Graphics(nullptr, value, ui->comboBoxTypes->currentIndex(), ui->comboBoxTypes->currentText().toStdString()));
}
