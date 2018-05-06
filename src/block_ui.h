/*!
 * @file
 * @brief This file contains the UI of each block
 *
 * @author Erik Kelemen, xkelem01@stud.fit.vutbr.cz
 * @author Attila Lakatos, xlakat01@stud.fit.vutbr.cz
 */

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
#include <QMessageBox>

#include "block.h"

#define START_POINT_X 600
#define START_POINT_Y 800
#define UI_BLOCK_HEADER_LINE_OFFSET 30
#define UI_BLOCK_WIDTH_BASE 180
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
    Node_Graphics(QGraphicsScene *parent = nullptr, QPointF start = {0, 0}, QPointF end = {0, 0}, Node_Graphics **reference_in = nullptr, std::list<Node_Graphics *> *reference_out = nullptr);
    ~Node_Graphics();
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
    Node_Graphics **reference_node_m;
    std::list<Node_Graphics *> *reference_out_m;
signals:

public slots:
};

class In_Port_Graphics : public QGraphicsWidget
{
    Q_OBJECT
public:
    explicit In_Port_Graphics(QGraphicsItem *parent = nullptr, in_port * reference = nullptr, unsigned int index = 0);
    in_port * access_backend();
    void free();
    bool is_free();
    void attach(Node_Graphics * node);
    void moved();
    Node_Graphics **get_connection();
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
    explicit Out_Port_Graphics(QGraphicsItem *parent = nullptr, value_i * reference = nullptr, unsigned int index = 0);
    void attach(Node_Graphics * node);
    value_i *get_reference();
    std::list<Node_Graphics *> *get_connect_list();
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
    value_i * reference_m;
signals:

public slots:
};

class Exec_Node_Graphics
{
public:
    Exec_Node_Graphics(QGraphicsScene *parent = nullptr, QPointF start = {0, 0}, QPointF end = {0, 0}, Exec_Node_Graphics **reference_in = nullptr, std::list<Exec_Node_Graphics *> *reference_out = nullptr);
    ~Exec_Node_Graphics();
    void alter_path(QPointF start, QPointF end);
    void alter_path(QPointF start);
    void alter_path(int padding, QPointF end);
protected:

private:
    QPoint start_m;
    QPoint end_m;
    QPainterPath * path_m;
    QGraphicsPathItem * reference_m;
    block * reference_in_m;
    Exec_Node_Graphics **reference_node_m;
    std::list<Exec_Node_Graphics *> *reference_out_m;
signals:

public slots:
};

class In_Exec_Graphics : public QGraphicsWidget
{
    Q_OBJECT
public:
    explicit In_Exec_Graphics(QGraphicsItem *parent = nullptr, block ** reference = nullptr);
    block ** access_backend();
    void free();
    bool is_free();
    void attach(Exec_Node_Graphics * node);
    void moved();
    Exec_Node_Graphics **get_connection();
protected:
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option , QWidget *widget);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

    virtual void dragEnterEvent(QGraphicsSceneDragDropEvent *event);
    virtual void dropEvent(QGraphicsSceneDragDropEvent *event);
private:
    QPointF offset;
    block **reference_m;
    Exec_Node_Graphics * connection_m;
signals:

public slots:
};

class Out_Exec_Graphics : public QGraphicsWidget
{
    Q_OBJECT
public:
    explicit Out_Exec_Graphics(QGraphicsItem *parent = nullptr, block * reference = nullptr);
    void attach(Exec_Node_Graphics *node);
    block *get_reference();
    std::list<Exec_Node_Graphics *> *get_connect_list();
    void moved();
protected:
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option , QWidget *widget);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

    virtual void dragEnterEvent(QGraphicsSceneDragDropEvent *event);
    virtual void dropEvent(QGraphicsSceneDragDropEvent *event);
private:
    std::list<Exec_Node_Graphics *> connections_m;
    QPointF offset;
    block * reference_m;
signals:

public slots:
};

/*class Start_Graphics : public QGraphicsWidget
{
    Q_OBJECT
public:
    explicit Block_Graphics(QGraphicsItem *parent = nullptr, block * reference = nullptr, QString name = "Name");
protected:
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option , QWidget *widget);
    virtual void moveEvent(QGraphicsSceneMoveEvent * event);
    virtual void keyPressEvent(QKeyEvent *event);
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
};*/

class Block_Graphics : public QGraphicsWidget
{
    Q_OBJECT
public:
    explicit Block_Graphics(QGraphicsItem *parent = nullptr, block * reference = nullptr, QString name = "Name");
protected:
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option , QWidget *widget);
    virtual void moveEvent(QGraphicsSceneMoveEvent * event);
    virtual void keyPressEvent(QKeyEvent *event);
    //void mouseMoveEvent(QMouseEvent *event);
private:
    std::vector<In_Port_Graphics *> in_ports_m;
    std::vector<Out_Port_Graphics *> out_ports_m;
    Out_Exec_Graphics * out_exec_m;
    In_Exec_Graphics * in_exec_m;
    void setup_block();
    QPoint offset;
    QString name_m;
    block * reference_m;
    unsigned int height_m;
signals:

public slots:
};

class Start_Graphics : public QGraphicsWidget
{
    Q_OBJECT
public:
    explicit Start_Graphics(QGraphicsItem *parent = nullptr, block ** reference = nullptr);
protected:
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option , QWidget *widget);
    //void mouseMoveEvent(QMouseEvent *event);
private:
    In_Exec_Graphics * in_exec_m;
    void setup_block();
    block ** reference_m;
signals:

public slots:
};

class Constant_Graphics : public QGraphicsWidget
{
    Q_OBJECT
public:
    explicit Constant_Graphics(QGraphicsItem *parent = nullptr, value_t value = 0.0, type_id_t type_id = 0, std::string name = "typeless");
    ~Constant_Graphics();
protected:
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option , QWidget *widget);
    virtual void moveEvent(QGraphicsSceneMoveEvent * event);
private:
    Out_Port_Graphics * out_port_m;
    value_i * reference_m;
    std::string name_m;
signals:

public slots:
};

#endif // BLOCK_UI_H
