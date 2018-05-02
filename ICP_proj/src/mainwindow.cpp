/*!
 * @file
 * @brief This file contains the root window
 *
 * @author Attila Lakatos, xlakat01@stud.fit.vutbr.cz
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QList>
#include <QDebug>

/**
 *  @brief MainWindow::MainWindow Constructor which creates main view
 */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    setMouseTracking(true);
    actual_project_m = new project();
    ui->setupUi(this);
    setWindowState(Qt::WindowMaximized);
    setWindowIcon(QIcon(":/icons/icon-c++.svg"));

    ui->Frame_BlockPalette->layout()->setAlignment(Qt::AlignTop);

    ui->main_field->mark_project(actual_project_m);
}

/**
 * @brief MainWindow::~MainWindow destructor
 */
MainWindow::~MainWindow()
{
    delete ui;
    delete actual_project_m;
}

/**
 * @brief MainWindow::on_actionNew_File_triggered Create a new file for editing
 */
void MainWindow::on_actionNew_File_triggered()
{
    QList<Block_UI *>list = ui->main_field->findChildren<Block_UI *>();

    foreach(Block_UI *elem, list) {
        delete elem;
    }

    ui->main_field->clearBlock_id();
}

/**
 * @brief MainWindow::on_actionLoad_File_triggered Opens an existing file for editing/executing
 * @note If the file is invalid(syntactically invalid) creates a new empty file
 */
void MainWindow::on_actionLoad_File_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Address Book"), "", tr("Block Editor (*.be2018);;All Files (*)"));
    if (fileName.isEmpty())
        return;

    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "error", file.errorString());
        return;
    }

    emit on_actionNew_File_triggered();

    QTextStream in(&file);
    loadMode mode = ADD_BLOCKS;
    std::map<QString, int> max_values = { {"ADD", 0}, {"SUB", 0} };

    while(!in.atEnd()) {
        QString line = in.readLine();
        if (line == "# BLOCKS") {
            mode = ADD_BLOCKS;
        }
        else if (line == "# CONNECTIONS") {
            mode = ADD_CONNECTIONS;
        }
        else {
            if (mode == ADD_BLOCKS) {
                QStringList lineList = line.split(" ");
                QPoint pos = QPoint();
                Block_UI *new_block;
                if (lineList.count() != 3)
                    return;
                pos.setX(lineList.at(0).toInt());
                pos.setY(lineList.at(1).toInt());
                new_block = new Block_UI(ui->main_field, lineList.at(2));
                new_block->move(pos);
                new_block->show();

                // To ensure that each block has an unique ID after opening a saved file
                QString key = lineList.at(2).left(3);
                int value = lineList.at(2).mid(3).toInt();
                if (value >= max_values[key])
                    max_values[key] = value+1;
            }
            else {
                //TODO - connections
            }
        }
    }

    ui->main_field->setBlock_id(max_values);
    file.close();
}


/**
 * @brief MainWindow::on_actionSave_File_triggered Saves the actual state of the editor
 */
void MainWindow::on_actionSave_File_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this,
        tr("Save Address Book"), "",
        tr("Block Editor files (*.be2018);;All Files (*)"));

    if (fileName.isEmpty()) {
        return;
    }

    std::string extension = fileName.toStdString().substr(fileName.length()-7);
    fileName = extension == ".be2018" ? fileName : fileName+".be2018";

    QFile outputFile(fileName);
    outputFile.open(QIODevice::WriteOnly);

    if(!outputFile.isOpen()){
        qDebug() << "- Error, unable to open" << fileName << "for output";
    }

    QTextStream outStream(&outputFile);
    outStream << "# BLOCKS\n";


    QList<Block_UI *>list = ui->main_field->findChildren<Block_UI *>();

    foreach(Block_UI *elem, list) {
        int x = elem->geometry().topLeft().x();
        int y = elem->geometry().topLeft().y();
        outStream << x << " " << y << " " << elem->get_name_m() << "\n";
    }
    //TODO - Connections between blocks

    outputFile.close();
}


/**
 * @brief MainWindow::on_actionQuit_triggered Exit
 *
 */
void MainWindow::on_actionQuit_triggered()
{
    QMessageBox::StandardButton reply;

    reply = QMessageBox::question(this, "This file may contain changes, do you want to save them?", "Your changes will be lost if you quit without saving.",
        QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

    if (reply == QMessageBox::Save) {
        emit on_actionSave_File_triggered();
        close();
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
 * @brief MainWindow::keyPressEvent Delete block/signal
 */
void MainWindow::keyPressEvent(QKeyEvent *event){
    if(event->key() == Qt::Key_Delete) {
        Block_UI *elem = (Block_UI *)ui->main_field->childAt(ui->main_field->blockRemovePos);
        delete elem;
    }
}
