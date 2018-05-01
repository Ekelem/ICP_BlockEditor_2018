/*!
 * @file
 * @brief This file contains the root window
 *
 * @author Attila Lakatos, xlakat01@stud.fit.vutbr.cz
 */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "project.h"

enum loadMode { ADD_BLOCKS, ADD_CONNECTIONS };

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionNew_File_triggered();
    void on_actionLoad_File_triggered();
    void on_actionSave_File_triggered();
    void on_actionQuit_triggered();
    void keyPressEvent(QKeyEvent *event);

private:
    Ui::MainWindow *ui;
    project * actual_project_m;
};

#endif // MAINWINDOW_H
