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

#include "block_ui.h"
#include "mainwindow.h"

class Canvas_UI : public QWidget
{
    Q_OBJECT
public:
    explicit Canvas_UI(QWidget *parent = nullptr);
protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
private:
    QPoint offset;
    QList<Block_UI *> blocks;

signals:

public slots:
};

#endif // CANVAS_UI_H
