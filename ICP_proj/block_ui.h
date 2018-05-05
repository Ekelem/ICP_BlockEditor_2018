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
#include <QCryptographicHash>
#include <QByteArray>

#include "block.h"

#define UI_BLOCK_HEADER_LINE_OFFSET 30
#define UI_BLOCK_WIDTH_BASE 120
#define UI_BLOCK_HEIGHT_BASE 40

class Block_UI : public QWidget
{
    Q_OBJECT
public:
    explicit Block_UI(QWidget *parent = nullptr, block * reference = nullptr, QString name = "Name");
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
signals:

public slots:
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

class Node_Graphics
{
public:
    Node_Graphics(QGraphicsScene *parent = nullptr, QPointF start = {0, 0}, QPointF end = {0, 0}, Node_Graphics **reference_in = nullptr, Node_Graphics **reference_out = nullptr);
    void alter_path(QPointF start, QPointF end);
    void alter_path(QPointF start);
    void alter_path(int padding, QPointF end);
protected:

private:
    QPoint start_m;
    QPoint end_m;
    QPainterPath * path_m;
    QGraphicsPathItem * reference_m;
    in_port * reference_in_m;
signals:

public slots:
};

class In_Port_Graphics : public QGraphicsWidget
{
    Q_OBJECT
public:
    explicit In_Port_Graphics(QGraphicsItem *parent = nullptr, in_port * reference = nullptr, unsigned int index = 0);
    in_port * access_backend();
    bool is_free();
    void attach(Node_Graphics * node);
    void moved();
protected:
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option , QWidget *widget);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

    virtual void dragEnterEvent(QGraphicsSceneDragDropEvent *event);
    virtual void dropEvent(QGraphicsSceneDragDropEvent *event);
private:
    void compute_color();

    QColor color_m;
    QPointF offset;
    in_port * reference_m;
    Node_Graphics * connection_m;
signals:

public slots:
};

class Out_Port_Graphics : public QGraphicsWidget
{
    Q_OBJECT
public:
    explicit Out_Port_Graphics(QGraphicsItem *parent = nullptr, out_port * reference = nullptr, unsigned int index = 0);
    void attach(Node_Graphics * node);
    out_port *get_reference();
    void moved();
protected:
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option , QWidget *widget);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

    virtual void dragEnterEvent(QGraphicsSceneDragDropEvent *event);
    virtual void dropEvent(QGraphicsSceneDragDropEvent *event);
private:
    void compute_color();
    QColor color_m;
    std::list<Node_Graphics *> connections_m;
    QPointF offset;
    out_port * reference_m;
signals:

public slots:
};

class Block_Graphics : public QGraphicsWidget
{
    Q_OBJECT
public:
    explicit Block_Graphics(QGraphicsItem *parent = nullptr, block * reference = nullptr, QString name = "Name");
protected:
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option , QWidget *widget);
    virtual void moveEvent(QGraphicsSceneMoveEvent * event);
    //void mouseMoveEvent(QMouseEvent *event);
private:
    std::vector<In_Port_Graphics *> in_ports_m;
    std::vector<Out_Port_Graphics *> out_ports_m;
    void setup_block();
    QPoint offset;
    QString name_m;
    block * reference_m;
    unsigned int height_m;
signals:

public slots:
};

#endif // BLOCK_UI_H
