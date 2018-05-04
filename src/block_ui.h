#ifndef BLOCK_UI_H
#define BLOCK_UI_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QApplication>
#include <QMimeData>
#include <QDrag>
#include <QFileDialog>
#include <QGraphicsWidget>
#include <QGraphicsScene>
#include <QGraphicsSceneEvent>

#include "block.h"

#define UI_BLOCK_HEADER_LINE_OFFSET 30
#define UI_BLOCK_WIDTH_BASE 120
#define UI_BLOCK_HEIGHT_BASE 40

#include <QDebug>

class Block_UI : public QWidget
{
    Q_OBJECT
public:
    explicit Block_UI(QWidget *parent = nullptr, block * reference = nullptr, QString name = "Name");
    QString get_name_m();
protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
private:
    void setup_block();
    QPoint offset;
    QString name_m;
    block * reference_m;
    unsigned int height_m;
};

class Block_Graphics : public QGraphicsWidget
{
    Q_OBJECT
public:
    explicit Block_Graphics(QGraphicsItem *parent = nullptr, block * reference = nullptr, QString name = "Name");
    QString get_name_m();
protected:
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option , QWidget *widget);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
private:

    void setup_block();
    QPoint offset;
    QString name_m;
    block * reference_m;
    unsigned int height_m;
};

class In_Port_UI : public QWidget
{
    Q_OBJECT
public:
    explicit In_Port_UI(QWidget *parent = nullptr, in_port * reference = nullptr, unsigned int index = 0);
    in_port * access_backend();
protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
private:
    QPoint offset;
    in_port * reference_m;
signals:

public slots:
};

class In_Port_Graphics : public QGraphicsWidget
{
    Q_OBJECT
public:
    explicit In_Port_Graphics(QGraphicsItem *parent = nullptr, in_port * reference = nullptr, unsigned int index = 0);
    in_port * access_backend();
protected:
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option , QWidget *widget);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

    virtual void dragEnterEvent(QGraphicsSceneDragDropEvent *event);
    virtual void dropEvent(QGraphicsSceneDragDropEvent *event);
private:
    QPointF offset;
    in_port * reference_m;
signals:

public slots:
};

class Out_Port_UI : public QWidget
{
    Q_OBJECT
public:
    explicit Out_Port_UI(QWidget *parent = nullptr, out_port * reference = nullptr, unsigned int index = 0);
protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
private:
    QPoint offset;
    out_port * reference_m;
signals:

public slots:
};

class Out_Port_Graphics : public QGraphicsWidget
{
    Q_OBJECT
public:
    explicit Out_Port_Graphics(QGraphicsItem *parent = nullptr, out_port * reference = nullptr, unsigned int index = 0);
    out_port *get_reference();
protected:
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option , QWidget *widget);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

    virtual void dragEnterEvent(QGraphicsSceneDragDropEvent *event);
    virtual void dropEvent(QGraphicsSceneDragDropEvent *event);
private:
    QPointF offset;
    out_port * reference_m;
signals:

public slots:
};

class Node_UI : public QWidget
{
    Q_OBJECT
public:
    explicit Node_UI(QWidget *parent = nullptr, In_Port_UI * reference_in = nullptr, Out_Port_UI * reference_out = nullptr);
protected:
    void paintEvent(QPaintEvent *);
    /*void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);*/
private:
    In_Port_UI * reference_in_m;
    Out_Port_UI * reference_out_m;
signals:

public slots:
};


class Start_Graphics : public QGraphicsWidget
{
    Q_OBJECT
public:
    explicit Start_Graphics(QGraphicsItem *parent = nullptr, block * reference = nullptr);
protected:
      virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option , QWidget *widget);
//    void mousePressEvent(QMouseEvent *event);
//    void mouseMoveEvent(QMouseEvent *event);
private:
    void setup_block();
    QPoint offset;
    QString name_m;
    block * reference_m;
    unsigned int height_m;
};

class End_Graphics : public QGraphicsWidget
{
    Q_OBJECT
public:
    explicit End_Graphics(QGraphicsItem *parent = nullptr, block * reference = nullptr);
protected:
      virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option , QWidget *widget);
//    void mousePressEvent(QMouseEvent *event);
//    void mouseMoveEvent(QMouseEvent *event);
private:
    void setup_block();
    QPoint offset;
    QString name_m;
    block * reference_m;
    unsigned int height_m;
};


#endif // BLOCK_UI_H
