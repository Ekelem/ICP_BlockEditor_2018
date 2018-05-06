/*!
 * @file
 * @brief This file contains the basic canvas used by the editor
 *
 * @author Erik Kelemen, xkelem01@stud.fit.vutbr.cz
 */


#include "canvas_ui.h"

Canvas_UI::Canvas_UI(QWidget *parent) : QWidget(parent)
{
    this->setFixedSize(2000, 2000);
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
        block * added = actual_project_m->add_block(event->mimeData()->data("block").data());
        Block_UI * new_block = new Block_UI(this, added, event->mimeData()->data("block"));
        actual_project_m->get_block_lib().push_back(added);
        new_block->move(event->pos());
        new_block->show();
    }
}


Canvas_Graphics::Canvas_Graphics(QGraphicsItem *parent, project *reference) : QGraphicsWidget(parent)
{
    resize(2000, 2000);
    setAcceptDrops(true);
    actual_project_m = reference;
}

Canvas_Graphics::~Canvas_Graphics()
{
    for (auto itr = blocks_ui.begin(); itr != blocks_ui.end(); itr++)
        delete (*itr);
}

void Canvas_Graphics::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
    if (event->mimeData()->hasFormat("text/plain"))
    {
        event->setAccepted(true);
        event->acceptProposedAction();
    }
}

void Canvas_Graphics::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    if (event->mimeData()->text() == "Create Block")
    {
        block * added = actual_project_m->add_block(event->mimeData()->data("block").data());
        Block_Graphics * new_block = new Block_Graphics(this, added, event->mimeData()->data("block"));
        actual_project_m->get_block_lib().push_back(added);
        blocks_ui.push_back(new_block);
        new_block->setPos(event->pos());
        new_block->show();
        new_block->setFlag(QGraphicsWidget::ItemIsMovable);
        new_block->setFlag(QGraphicsWidget::ItemIsSelectable);
        new_block->setFlag(QGraphicsWidget::ItemIsFocusable);
        new_block->setFocus();
        this->scene()->setFocusItem(new_block);
    }
}

void Canvas_Graphics::mark_project(project *actual_project)
{
    if (actual_project == nullptr)
        throw 25; //TODO

    actual_project_m = actual_project;
}

void Canvas_Graphics::update()
{
    for (auto itr = blocks_ui.begin(); itr != blocks_ui.end(); itr++)
        (*itr)->update();
}

void Canvas_Graphics::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRect size = QRect(0, 0, this->geometry().width(), this->geometry().height());
    QBrush canvas(Qt::gray, Qt::CrossPattern);
    canvas.setMatrix(QMatrix().scale(2.0, 2.0));
    painter->setBrush(canvas);
    painter->drawRect(size);

    (void)option;   //dont really need it
    (void)widget;   //dont really need it
}
