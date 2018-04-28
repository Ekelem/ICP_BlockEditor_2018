#ifndef MYLINES_H
#define MYLINES_H

#include <QGraphicsItem>
#include <QList>
#include <QMainWindow>

class Edge;
class GraphWidget;
class QGraphicsSceneMouseEvent;

class Node : public QGraphicsItem
{
    Q_OBJECT
public:
    Node(GraphWidget *graphWidget,QWidget *parent = nullptr, QMainWindow *s = nullptr);


    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

protected:

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

};

#endif // MYLINES_H

