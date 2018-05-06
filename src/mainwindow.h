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

enum loadMode { ADD_BLOCKS, ADD_CONNECTIONS, ADD_START, ADD_END, ADD_VALUES };

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
    QGraphicsItem *getParentByName(QString name);
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
    double go_through_tree(In_Port_Graphics *pointer_to_curr_in);
    bool isAtTheBeginning(In_Port_Graphics *pointer_to_curr_in);
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
    Out_Port_Graphics *temporary_out = nullptr;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
private:
    CanvasMode sceneMode;
    void makeItemsControllable(bool areControllable);
};

#endif // MAINWINDOW_H
