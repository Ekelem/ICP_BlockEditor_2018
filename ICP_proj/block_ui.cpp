#include "block_ui.h"

#include <iostream>
#include <string>

Block_UI::Block_UI(QWidget *parent, block *reference, QString name) : QWidget(parent)
{
    height_m = UI_BLOCK_HEIGHT_BASE;
    name_m = name;
    reference_m = reference;
    this->setFixedSize(UI_BLOCK_WIDTH_BASE, UI_BLOCK_HEIGHT_BASE);
    setup_block();
}

void Block_UI::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QRect size = QRect(0 , 0 , UI_BLOCK_WIDTH_BASE, height_m);

    QLinearGradient linearGrad(0 , 0 , UI_BLOCK_WIDTH_BASE*2, height_m*2);
        linearGrad.setColorAt(0, Qt::white);
        linearGrad.setColorAt(0.5, Qt::gray);
        linearGrad.setColorAt(1, Qt::black);

    painter.setBrush(linearGrad);
    painter.drawRoundedRect(size, 10.0, 10.0);

    //QPen pen(Qt::black, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    painter.setPen(Qt::black);
    QFont sansFont("Helvetica [Cronyx]", 12);
    painter.setFont(sansFont);
    painter.drawText(size, Qt::AlignHCenter, name_m);
    painter.setPen(QPen(Qt::gray, 3, Qt::DashDotLine, Qt::RoundCap));
    painter.drawLine(QPoint(0, UI_BLOCK_HEADER_LINE_OFFSET), QPoint(UI_BLOCK_WIDTH_BASE, UI_BLOCK_HEADER_LINE_OFFSET));

}

void Block_UI::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
        offset = event->pos();
}

void Block_UI::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton)
    {
            this->move(mapToParent(event->pos() - offset));
    }
}

void Block_UI::setup_block()
{
    if (reference_m != nullptr)
    {
        height_m = (reference_m->get_max_size()+1) * UI_BLOCK_HEIGHT_BASE;
        this->setFixedSize(UI_BLOCK_WIDTH_BASE, height_m);

        for (unsigned int i = 0; i < reference_m->get_in_size(); i++)   //visualize in_ports
            new In_Port_UI(this, reference_m->get_in_port(i), i);

        for (unsigned int i = 0; i < reference_m->get_out_size(); i++)   //visualize out_ports
            new Out_Port_UI(this, reference_m->get_out_port(i), i);
    }
}

In_Port_UI::In_Port_UI(QWidget *parent, in_port *reference, unsigned int index) : QWidget(parent)
{
    reference_m = reference;
    this->setFixedSize(UI_BLOCK_HEIGHT_BASE, UI_BLOCK_HEIGHT_BASE);
    this->move(-10, UI_BLOCK_HEIGHT_BASE * (index+1));
    setAcceptDrops(true);
}

in_port *In_Port_UI::access_backend()
{
    return reference_m;
}

void In_Port_UI::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setBrush(Qt::gray);
    painter.drawEllipse({16, 16}, 16, 16);
}

void In_Port_UI::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
        offset = event->pos();
}

void In_Port_UI::mouseMoveEvent(QMouseEvent *event)
{
    if (!(event->buttons() & Qt::LeftButton))
            return;
        if ((event->pos() - offset).manhattanLength()
             < QApplication::startDragDistance())
            return;

        QDrag *drag = new QDrag((QWidget*)this);
        QMimeData *mimeData = new QMimeData;

        QByteArray prep_data = QByteArray();
        u_int64_t ptr = reinterpret_cast<u_int64_t>(this);
        prep_data.setRawData(reinterpret_cast<char*>(&ptr), 8);     //We assume only 32 bit architecture (4 bytes long pointers) or 64 (8 bytes long pointers)

        mimeData->setText("Connect from in");
        mimeData->setData("in_port", prep_data);
        drag->setMimeData(mimeData);

        Qt::DropAction dropAction = drag->exec(Qt::CopyAction | Qt::MoveAction);
}

void In_Port_UI::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("text/plain"))
    {
        event->acceptProposedAction();
    }
}

void In_Port_UI::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->text() == "Connect from out")
    {
        reference_m->attach(**reinterpret_cast<out_port**>(event->mimeData()->data("out_port").data()));    //in mime_data is stored pointer(8 bytes) on out_port, so pointer on data is pointer on pointer..
        //TODO draw cubic bezier
    }
}

Out_Port_UI::Out_Port_UI(QWidget *parent, out_port *reference, unsigned int index) : QWidget(parent)
{
    reference_m = reference;
    this->setFixedSize(UI_BLOCK_HEIGHT_BASE, UI_BLOCK_HEIGHT_BASE);
    this->move(UI_BLOCK_WIDTH_BASE - 24, UI_BLOCK_HEIGHT_BASE * (index+1));
    setAcceptDrops(true);
}

out_port *Out_Port_Graphics::get_reference()
{
    return reference_m;
}

void Out_Port_Graphics::moved()
{
    for (auto itr = connections_m.begin(); itr != connections_m.end(); itr++)
    {
        (*itr)->alter_path(0, this->scenePos());
    }
}

void Out_Port_UI::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setBrush(Qt::gray);
    painter.drawEllipse({16, 16}, 16, 16);
}

void Out_Port_UI::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
        offset = event->pos();
}

void Out_Port_UI::mouseMoveEvent(QMouseEvent *event)
{
    if (!(event->buttons() & Qt::LeftButton))
            return;
        if ((event->pos() - offset).manhattanLength()
             < QApplication::startDragDistance())
            return;

        QDrag *drag = new QDrag((QWidget*)this);
        QMimeData *mimeData = new QMimeData;

        QByteArray prep_data = QByteArray();
        u_int64_t ptr = reinterpret_cast<u_int64_t>(this);
        prep_data.setRawData(reinterpret_cast<char*>(&ptr), 8);     //We assume only 32 bit architecture (4 bytes long pointers) or 64 (8 bytes long pointers)

        mimeData->setText("Connect from out");
        mimeData->setData("out_port", prep_data);
        drag->setMimeData(mimeData);

        Qt::DropAction dropAction = drag->exec(Qt::CopyAction | Qt::MoveAction);
}

void Out_Port_UI::dragEnterEvent(QDragEnterEvent *event)
{
    std::cerr << "out dragenter" << std::endl;
    if (event->mimeData()->hasFormat("text/plain"))
    {
        event->acceptProposedAction();
    }
}

void Out_Port_UI::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->text() == "Connect from in")
    {
        In_Port_UI * recieved = (*reinterpret_cast<In_Port_UI**>(event->mimeData()->data("in_port").data()));
        if (recieved == nullptr)
        {
            //TODO throw
        }
        else
        {
            recieved->access_backend();
            recieved->access_backend()->attach(*reference_m);    //in mime_data is stored pointer(8 bytes) on out_port, so pointer on data is pointer on pointer..
            //TODO draw cubic bezier
            //new Node_UI((QWidget*)this->parent()->parent(), recieved, this);
        }
    }
}


Block_Graphics::Block_Graphics(QGraphicsItem *parent, block *reference, QString name) : QGraphicsWidget(parent)
{
    height_m = UI_BLOCK_HEIGHT_BASE;
    name_m = name;
    reference_m = reference;
    this->resize(UI_BLOCK_WIDTH_BASE, UI_BLOCK_HEIGHT_BASE);
    setup_block();
}

void Block_Graphics::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRect size = QRect(0 , 0 , UI_BLOCK_WIDTH_BASE, height_m);

    QLinearGradient linearGrad(0 , 0 , UI_BLOCK_WIDTH_BASE*2, height_m*2);
        linearGrad.setColorAt(0, Qt::white);
        linearGrad.setColorAt(0.5, Qt::gray);
        linearGrad.setColorAt(1, Qt::black);

    painter->setBrush(linearGrad);
    painter->drawRoundedRect(size, 10.0, 10.0);

    //QPen pen(Qt::black, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    painter->setPen(Qt::black);
    QFont sansFont("Helvetica [Cronyx]", 12);
    painter->setFont(sansFont);
    painter->drawText(size, Qt::AlignHCenter, name_m);
    painter->setPen(QPen(Qt::gray, 3, Qt::DashDotLine, Qt::RoundCap));
    painter->drawLine(QPoint(0, UI_BLOCK_HEADER_LINE_OFFSET), QPoint(UI_BLOCK_WIDTH_BASE, UI_BLOCK_HEADER_LINE_OFFSET));
}

void Block_Graphics::setup_block()
{
    if (reference_m != nullptr)
    {
        height_m = (reference_m->get_max_size()+1) * UI_BLOCK_HEIGHT_BASE;
        this->resize(UI_BLOCK_WIDTH_BASE, height_m);

        for (unsigned int i = 0; i < reference_m->get_in_size(); i++)   //visualize in_ports
            in_ports_m.push_back(new In_Port_Graphics(this, reference_m->get_in_port(i), i));

        for (unsigned int i = 0; i < reference_m->get_out_size(); i++)   //visualize out_ports
            out_ports_m.push_back(new Out_Port_Graphics(this, reference_m->get_out_port(i), i));
    }
}


void Block_Graphics::moveEvent(QGraphicsSceneMoveEvent *event)
{
    for (unsigned int i = 0; i < reference_m->get_in_size(); i++)
    {
        if (in_ports_m.size() > i)      //just for sure
            in_ports_m[i]->moved();
    }

    for (unsigned int i = 0; i < reference_m->get_out_size(); i++)
    {
        if (out_ports_m.size() > i)     //just for sure
            out_ports_m[i]->moved();
    }
}

In_Port_Graphics::In_Port_Graphics(QGraphicsItem *parent, in_port *reference, unsigned int index) : QGraphicsWidget(parent)
{
    reference_m = reference;
    connection_m = nullptr;
    this->compute_color();
    this->resize(UI_BLOCK_HEIGHT_BASE, UI_BLOCK_HEIGHT_BASE);
    this->setPos(-10, UI_BLOCK_HEIGHT_BASE * (index+1));
    setAcceptDrops(true);
}

in_port *In_Port_Graphics::access_backend()
{
    return reference_m;
}

bool In_Port_Graphics::is_free()
{
    return connection_m == nullptr ? true : false;
}

void In_Port_Graphics::attach(Node_Graphics *node)
{
    connection_m = node;
}

void In_Port_Graphics::moved()
{
    if (connection_m != nullptr)
        connection_m->alter_path(this->scenePos());
}

void In_Port_Graphics::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setBrush(color_m);
    painter->drawEllipse({16, 16}, 16, 16);
}

void In_Port_Graphics::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
        offset = event->pos();
}

void In_Port_Graphics::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (!(event->buttons() & Qt::LeftButton))
            return;
        if ((event->pos() - offset).manhattanLength()
             < QApplication::startDragDistance())
            return;

        QDrag *drag = new QDrag((QWidget*)this);
        QMimeData *mimeData = new QMimeData;

        QByteArray prep_data = QByteArray();
        u_int64_t ptr = reinterpret_cast<u_int64_t>(this);
        prep_data.setRawData(reinterpret_cast<char*>(&ptr), 8);     //We assume only 32 bit architecture (4 bytes long pointers) or 64 (8 bytes long pointers)

        mimeData->setText("Connect from in");
        mimeData->setData("in_port", prep_data);
        drag->setMimeData(mimeData);

        Qt::DropAction dropAction = drag->exec(Qt::CopyAction | Qt::MoveAction);
}

void In_Port_Graphics::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
    std::cerr << "in_port dragenter" << std::endl;
    if (event->mimeData()->hasFormat("text/plain"))
    {
        event->acceptProposedAction();
    }
}


void In_Port_Graphics::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    if (event->mimeData()->text() == "Connect from out")
    {
        Out_Port_Graphics * recieved = (*reinterpret_cast<Out_Port_Graphics**>(event->mimeData()->data("out_port").data()));
        if (recieved == nullptr)
            throw 1;
        if (!this->is_free())
        {
            //TODO
        }
        else
        {
            reference_m->attach(*(recieved->get_reference()));
            Node_Graphics * node = new Node_Graphics(this->scene(), this->scenePos(), recieved->scenePos());
            this->attach(node);
            recieved->attach(node);
        }
        //reference_m->attach(*(*reinterpret_cast<Out_Port_Graphics**>(event->mimeData()->data("out_port").data()))->get_reference());    //in mime_data is stored pointer(8 bytes) on out_port, so pointer on data is pointer on pointer..
        //TODO draw cubic bezier
    }
}

void In_Port_Graphics::compute_color()
{
    QCryptographicHash hasher(QCryptographicHash::Sha1);//QColor(reference_m->get_type())
    type_id_t id = reference_m->get_type();
    hasher.addData(QByteArray((const char*)&id, sizeof(type_id_t)));
    QByteArray res = hasher.result();
    color_m = QColor(abs(static_cast<int>(*(res.data()))), abs(static_cast<int>(*(res.data()+8))), abs(static_cast<int>(*(res.data()+16))));
}



Out_Port_Graphics::Out_Port_Graphics(QGraphicsItem *parent, out_port *reference, unsigned int index) : QGraphicsWidget(parent)
{
    reference_m = reference;
    compute_color();
    this->resize(UI_BLOCK_HEIGHT_BASE, UI_BLOCK_HEIGHT_BASE);
    this->setPos(UI_BLOCK_WIDTH_BASE - 24, UI_BLOCK_HEIGHT_BASE * (index+1));
    setAcceptDrops(true);
}

void Out_Port_Graphics::attach(Node_Graphics *node)
{
    connections_m.push_back(node);
    //&(*(--connections_m.end()))
}

void Out_Port_Graphics::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setBrush(color_m);
    painter->drawEllipse({16, 16}, 16, 16);
}

void Out_Port_Graphics::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
        offset = event->pos();
}

void Out_Port_Graphics::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (!(event->buttons() & Qt::LeftButton))
            return;
        if ((event->pos() - offset).manhattanLength()
             < QApplication::startDragDistance())
            return;

        QDrag *drag = new QDrag((QWidget*)this);
        QMimeData *mimeData = new QMimeData;

        QByteArray prep_data = QByteArray();
        u_int64_t ptr = reinterpret_cast<u_int64_t>(this);
        prep_data.setRawData(reinterpret_cast<char*>(&ptr), 8);     //We assume only 32 bit architecture (4 bytes long pointers) or 64 (8 bytes long pointers)

        mimeData->setText("Connect from out");
        mimeData->setData("out_port", prep_data);
        drag->setMimeData(mimeData);

        Qt::DropAction dropAction = drag->exec(Qt::CopyAction | Qt::MoveAction);
}

void Out_Port_Graphics::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
    std::cerr << "out_port dragenter" << std::endl;
    if (event->mimeData()->hasFormat("text/plain"))
    {
        event->acceptProposedAction();
    }
}

void Out_Port_Graphics::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    if (event->mimeData()->text() == "Connect from in")
    {
        In_Port_Graphics * recieved = (*reinterpret_cast<In_Port_Graphics**>(event->mimeData()->data("in_port").data()));
        if (recieved == nullptr)
        {
            //TODO throw
        }
        else
        {
            if (!recieved->is_free())
            {
                //TODO
            }
            else
            {
                recieved->access_backend()->attach(*reference_m);
                Node_Graphics * node = new Node_Graphics(this->scene(), recieved->scenePos(), this->scenePos());
                recieved->attach(node);
                connections_m.push_back(node);
            }
            /*QPainterPath path = QPainterPath();
            QPoint start = QPoint(recieved->scenePos().x() + 16, recieved->scenePos().y() + 16);
            path.moveTo(start);
            path.cubicTo(recieved->scenePos().x() - 200, recieved->scenePos().y(), this->scenePos().x() + 200, this->scenePos().y(), this->scenePos().x() + 16, this->scenePos().y() + 16);
            QGraphicsPathItem * connection = this->scene()->addPath(path, QPen(Qt::green, 3, Qt::SolidLine));*/
        }
    }
}

void Out_Port_Graphics::compute_color()
{
    QCryptographicHash hasher(QCryptographicHash::Sha1);//QColor(reference_m->get_type())
    type_id_t id = reference_m->get_type();
    hasher.addData(QByteArray((const char*)&id, sizeof(type_id_t)));
    QByteArray res = hasher.result();
    color_m = QColor(abs(static_cast<int>(*(res.data()))), abs(static_cast<int>(*(res.data()+8))), abs(static_cast<int>(*(res.data()+16))));
}


Node_Graphics::Node_Graphics(QGraphicsScene *parent, QPointF start, QPointF end, Node_Graphics **reference_in, Node_Graphics **reference_out)
{
    path_m = new QPainterPath();
    alter_path(start, end);
    reference_m = parent->addPath(*path_m, QPen(Qt::green, 3, Qt::SolidLine));
}

void Node_Graphics::alter_path(QPointF start, QPointF end)
{
    start_m = QPoint(start.x() + 16, start.y() + 16);   // in port + circle radius
    end_m = QPoint(end.x() + 16, end.y() + 16); //out_port + circle radius
    path_m->moveTo(start_m);
    int divider = start_m.x() > end_m.x() ? 2 : 1;
    int length = (start_m - end_m).manhattanLength() / divider;
    path_m->cubicTo(start_m.x() - length, start_m.y(), end_m.x() + length, end_m.y(), end_m.x(), end_m.y());
}

void Node_Graphics::alter_path(QPointF start)
{
    start_m = QPoint(start.x() + 16, start.y() + 16);   // in port + circle radius
    *path_m = QPainterPath();
    path_m->moveTo(start_m);
    int divider = start_m.x() > end_m.x() ? 2 : 1;
    int length = (start_m - end_m).manhattanLength() / divider;
    path_m->cubicTo(start_m.x() - length, start_m.y(), end_m.x() + length, end_m.y(), end_m.x(), end_m.y());
    reference_m->setPath(*path_m);
}

void Node_Graphics::alter_path(int padding, QPointF end)
{
    end_m = QPoint(end.x() + 16, end.y() + 16); //out_port + circle radius
    *path_m = QPainterPath();
    path_m->moveTo(start_m);
    int divider = start_m.x() > end_m.x() ? 2 : 1;
    int length = (start_m - end_m).manhattanLength() / divider;
    path_m->cubicTo(start_m.x() - length, start_m.y(), end_m.x() + length, end_m.y(), end_m.x(), end_m.y());
    reference_m->setPath(*path_m);
}
