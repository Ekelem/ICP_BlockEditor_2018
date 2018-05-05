#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QDebug>
#include <QMessageBox>
#include <QMouseEvent>
#include <list>
#include <QGraphicsItem>
#include <QKeyEvent>
#include <QList>
#include <map>
#include <QString>

#include "project.h"
#include "block.h"
#include "block_templates.h"
#include "canvas_ui.h"
#include "block_ui.h"

enum loadMode { ADD_BLOCKS, ADD_CONNECTIONS, ADD_START, ADD_END };

class Scene_Graphics;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
protected:
    void keyPressEvent(QKeyEvent *);

private slots:
    void on_actionNew_File_triggered();
    void on_actionSave_File_triggered();
    void on_actionQuit_triggered();
    void on_actionRun_program_triggered();
    void on_actionLoad_triggered();

private:
    Ui::MainWindow *ui;
    project * actual_project_m;
    Scene_Graphics * scene_m;
    double go_through_tree(In_Port_Graphics *pointer_to_curr_in, std::list<In_Port_Graphics *>first_items);
    bool isAtTheBeginning(In_Port_Graphics *pointer_to_curr_in, std::list<In_Port_Graphics *>first_items);
};


class Scene_Graphics : public QGraphicsScene {
    Q_OBJECT
public:
    explicit Scene_Graphics(QObject *parent = 0);
    enum CanvasMode {NoMode, SelectObject};
    void setMode(CanvasMode mode);
    Start_Graphics *start;
    End_Graphics *end;
    In_Port_Graphics *temporary_in = nullptr;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
private:
    CanvasMode sceneMode;
    void makeItemsControllable(bool areControllable);
};

#endif // MAINWINDOW_H
