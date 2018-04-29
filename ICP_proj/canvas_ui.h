#ifndef CANVAS_UI_H
#define CANVAS_UI_H

#include <QPainter>
#include <QWidget>
#include <QMouseEvent>
#include <QScrollArea>
#include <QScrollBar>
#include <QApplication>
#include <iostream> //debug

class Canvas_UI : public QWidget
{
    Q_OBJECT
public:
    explicit Canvas_UI(QWidget *parent = nullptr);
    void set_scroll(QScrollArea * scroll_area);
protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
private:
    QPoint offset;

signals:

public slots:
};

#endif // CANVAS_UI_H
