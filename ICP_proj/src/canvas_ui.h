/*!
 * @file
 * @brief Basic class of a block-backend
 *
 * @author Erik Kelemen, xkelem01@stud.fit.vutbr.cz
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
#include <iostream> //debug
#include <map>
#include <QDebug> //debug

#include "block_ui.h"
#include "mainwindow.h"
#include "project.h"
#include "block_templates.h"

class Canvas_UI : public QWidget
{
    Q_OBJECT
public:
    explicit Canvas_UI(QWidget *parent = nullptr);
    void mark_project(project * actual_project);
    QPoint blockRemovePos = QPoint();
    void setBlock_id(std::map<QString, int> new_block_id);
    void clearBlock_id();
protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
private:
    QPoint offset;
    project * actual_project_m;
    std::map<QString, int> block_id = { {"ADD" , 0}, {"SUB", 0} };

signals:

public slots:
};

#endif // CANVAS_UI_H
