#include "canvas_ui.h"

Canvas_UI::Canvas_UI(QWidget *parent) : QWidget(parent)
{

}

void Canvas_UI::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QRect size = QRect(0, 0, this->geometry().width(), this->geometry().height());
    QBrush canvas(Qt::gray, Qt::CrossPattern);
    canvas.setMatrix(QMatrix().scale(2.0, 2.0));
    painter.setBrush(canvas);
    painter.drawRect(size);
}

void Canvas_UI::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
        //offset = mapToParent(event->pos());
        offset = event->pos();
}

void Canvas_UI::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton)
    {
        QPoint diff = mapToParent(event->pos() - offset);
        //std::cerr << diff.x() << std::endl;
        int old = this->pos().x();
        this->move(diff);
        //scroll->horizontalScrollBar()->setValue(scroll->horizontalScrollBar()->value() - diff.x());
        //scroll->verticalScrollBar()->setValue(scroll->verticalScrollBar()->value() - diff.y());
    }
}
