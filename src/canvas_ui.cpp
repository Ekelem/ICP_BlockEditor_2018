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

/*void Canvas_UI::mousePressEvent(QMouseEvent *event)
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
}*/

void Canvas_UI::dragEnterEvent(QDragEnterEvent *event)
{
    std::cerr << "canvas dragenter" << std::endl;
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

Canvas_SCENE::Canvas_SCENE(QObject *parent) : QGraphicsScene(parent)
{

}

/*void Canvas_SCENE::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
    std::cerr << "scene dragenter" << std::endl;
    if (event->mimeData()->hasFormat("text/plain"))
    {
        event->setAccepted(true);
        event->acceptProposedAction();
    }
}

void Canvas_SCENE::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    std::cerr << "scene drag!!!" << std::endl;
    //((QWidget*)focusItem())->dropEvent(event);

}

void Canvas_SCENE::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{
    //event->acceptProposedAction();
}*/

Canvas_Graphics::Canvas_Graphics(QGraphicsWidget *parent, project *reference) : QGraphicsWidget(parent)
{
    resize(2000, 2000);
    setAcceptDrops(true);
    actual_project_m = reference;
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
        QString name = event->mimeData()->data("block");
        name += QString::number(block_id[event->mimeData()->data("block")]++);
        block * added = new b_add_kg_to_kg(actual_project_m->get_type_lib());
        Block_Graphics * new_block = new Block_Graphics(this, added, name);
        actual_project_m->get_block_lib().push_back(added);
        //new_block->move(event->pos());
        new_block->setPos(event->pos());
        new_block->show();
        new_block->setFlag(QGraphicsWidget::ItemIsMovable);
    }
}

void Canvas_Graphics::mark_project(project *actual_project)
{
    if (actual_project == nullptr)
        throw 25; //TODO

    actual_project_m = actual_project;
}


void Canvas_Graphics::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRect size = QRect(0, 0, this->geometry().width(), this->geometry().height());
    QBrush canvas(Qt::gray, Qt::CrossPattern);
    canvas.setMatrix(QMatrix().scale(2.0, 2.0));
    painter->setBrush(canvas);
    painter->drawRect(size);
}


void Canvas_Graphics::setBlock_id(std::map<QString, int> new_block_id) {
    for (std::map<QString, int>::iterator it=new_block_id.begin(); it!=new_block_id.end(); ++it) {
        block_id[it->first] = it->second;
    }
}

void Canvas_Graphics::clearBlock_id() {
    for (std::map<QString, int>::iterator it=block_id.begin(); it!=block_id.end(); ++it) {
        block_id[it->first] = 0;
    }
}

std::map<QString, int> Canvas_Graphics::get_block_id() {
    return block_id;
}
