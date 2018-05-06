/*!
 * @file
 * @brief Canvas which contains the BlockEditor2018 scene
 *
 * @author Erik Kelemen, xkelem01@stud.fit.vutbr.cz
 * @author Attila Lakatos, xlakat01@stud.fit.vutbr.cz
 */

#ifndef CANVAS_UI_H
#define CANVAS_UI_H

#include <QPainter>
#include <QWidget>
#include <QMouseEvent>
#include <QScrollArea>
#include <QScrollBar>
#include <QApplication>
#include <QList>
#include <QMainWindow>
#include <QMimeData>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <iostream> //debug

#include "block_ui.h"
//#include "mainwindow.h"
#include "project.h"
#include "block_templates.h"
#include <QGraphicsItem>

#include <QGraphicsSceneDragDropEvent>

class Canvas_UI : public QWidget
{
    Q_OBJECT
public:
    explicit Canvas_UI(QWidget *parent = nullptr);
    void mark_project(project * actual_project);
protected:
    void paintEvent(QPaintEvent *);
    /*void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);*/

    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);

private:
    QPoint offset;
    project * actual_project_m;

signals:

public slots:
};

class Canvas_Graphics : public QGraphicsWidget
{
    Q_OBJECT
public:
    explicit Canvas_Graphics(QGraphicsItem *parent = nullptr, project * reference = nullptr);
    ~Canvas_Graphics();
    void mark_project(project * actual_project);
    void update();
protected:

    void dragEnterEvent(QGraphicsSceneDragDropEvent *event);
    void dropEvent(QGraphicsSceneDragDropEvent *event);
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option , QWidget *widget);

private:
    QPoint offset;
    project * actual_project_m;
    std::list<Block_Graphics *> blocks_ui;

signals:

public slots:
};

#endif // CANVAS_UI_H
