#include "canvas_ui.h"

Canvas_UI::Canvas_UI(QWidget *parent) : QWidget(parent)
{
    setAcceptDrops(true);
}

void Canvas_UI::mark_project(project *actual_project)
{
    if (actual_project == nullptr)
        throw 25; //TODO

    actual_project_m = actual_project;
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
        //int old = this->pos().x();
        this->move(diff);
    }
}

void Canvas_UI::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("text/plain"))
    {
        event->acceptProposedAction();
    }
}

void Canvas_UI::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->text() == "Create Block")
    {
        block * added = new b_add_kg_to_kg(actual_project_m->get_type_lib());
        Block_UI * new_block = new Block_UI(this, added, event->mimeData()->data("block"));
        actual_project_m->get_block_lib().push_back(added);
        new_block->move(event->pos());
        new_block->show();
    }
}
