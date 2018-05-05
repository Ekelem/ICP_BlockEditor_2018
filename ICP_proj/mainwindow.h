#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include "project.h"
#include "block.h"
#include "block_templates.h"
#include "canvas_ui.h"

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

private:
    Ui::MainWindow *ui;
    project * actual_project_m;
    QGraphicsScene * scene_m;
};

#endif // MAINWINDOW_H
