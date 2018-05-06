/*!
 * @file
 * @brief This file contains the basic window definition
 *
 * @author Erik Kelemen, xkelem01@stud.fit.vutbr.cz
 * @author Attila Lakatos, xlakat01@stud.fit.vutbr.cz
 */

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
    canvas_m = new Canvas_Graphics(nullptr, actual_project_m);
    scene_m->addItem(canvas_m);
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
    delete canvas_m;
}

void MainWindow::on_actionNew_File_triggered()
{

}


void MainWindow::on_actionLoad_File_triggered()
{
    QString filename=QFileDialog::getOpenFileName(this, tr("Open File"), "", "All files (*.*);;");
    if (filename != "")
        QMessageBox::information(this, tr("File Name"), filename);
}


void MainWindow::on_actionSave_File_triggered()
{

}

/**
 * @brief MainWindow::on_actionQuit_triggered Application terminates
 */
void MainWindow::on_actionQuit_triggered()
{
    QMessageBox::StandardButton reply;

    reply = QMessageBox::question(this, "This file has changes, do you want to save them?", "Your changes will be lost if you close this item without saving.",
        QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

    if (reply == QMessageBox::Save) {
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

/**
 * @brief MainWindow::on_actionRun_program_triggered Exectutes the main program
 */
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
        actual_project_m->force_stop();
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
    canvas_m->update();
}

/**
 * @brief MainWindow::on_constantButton_clicked Create a constant
 *
 */
void MainWindow::on_constantButton_clicked()
{
    bool number = false;
    double value = ui->constantValue->toPlainText().toDouble(&number);
    if (!number)
    {
        QMessageBox msgBox;
        msgBox.setText("Constant Value must be number!");
        msgBox.exec();
    }
    else
    {
        QGraphicsItem * new_constant = new Constant_Graphics(nullptr, value, ui->comboBoxTypes->currentIndex(), ui->comboBoxTypes->currentText().toStdString());
        scene_m->addItem(new_constant);
        new_constant->setPos(mapToParent(QPoint(START_POINT_X, START_POINT_Y)));
    }
}

void MainWindow::on_actionNext_step_triggered()
{
    try
    {
        actual_project_m->step();
        QMessageBox msgBox;
        msgBox.setText("Step succesful!");
        msgBox.exec();
    }
    catch(exceptions_enum e)
    {
        actual_project_m->force_stop();
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
    canvas_m->update();
}

void MainWindow::on_actionRestart_program_triggered()
{
    actual_project_m->reset();
    canvas_m->update();

    QMessageBox msgBox;
    msgBox.setText("Reset succesful!");
    msgBox.exec();
}
