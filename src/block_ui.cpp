/*!
 * @file
 * @brief This file contains the UI of each block
 *
 * @author Erik Kelemen, xkelem01@stud.fit.vutbr.cz
 * @author Attila Lakatos, xlakat01@stud.fit.vutbr.cz
 */

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

value_i *Out_Port_Graphics::get_reference()
{
    return reference_m;
}

std::list<Node_Graphics *> *Out_Port_Graphics::get_connect_list()
{
    return &connections_m;
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

    QLinearGradient linearGrad(0 , 0 , UI_BLOCK_WIDTH_BASE, height_m);
    if (this->isSelected())
    {
        linearGrad.setColorAt(0, Qt::white);
        linearGrad.setColorAt(0.5, Qt::green);
        linearGrad.setColorAt(1, Qt::gray);
    }
    else
    {
        linearGrad.setColorAt(0, Qt::white);
        //linearGrad.setColorAt(0.5, Qt::gray);
        linearGrad.setColorAt(1, Qt::gray);
    }

    painter->setBrush(linearGrad);
    painter->drawRoundedRect(size, 10.0, 10.0);

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

        out_exec_m = new Out_Exec_Graphics(this, reference_m);
        in_exec_m = new In_Exec_Graphics(this, reference_m->get_ptr_sequence_succ());
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

    out_exec_m->moved();
    in_exec_m->moved();
}

void Block_Graphics::keyPressEvent(QKeyEvent *event)
{
    std::cerr << "NOW" << std::endl;
    if (event->key() == Qt::Key_Delete)
    {
        this->scene()->removeItem(this);
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

void In_Port_Graphics::free()
{
    if (connection_m != nullptr)
        connection_m->~Node_Graphics();
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

Node_Graphics **In_Port_Graphics::get_connection()
{
    return &connection_m;
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
        if (this->reference_m->get_type() !=  recieved->get_reference()->get_type())
        {
            QMessageBox msgBox;
            msgBox.setText("Type mistmatch!");
            msgBox.exec();
        }
        else
        {
            if (!this->is_free())
            {
                this->free();   //destroy previous connection
            }
            reference_m->attach(*(recieved->get_reference()));
            Node_Graphics * node = new Node_Graphics(this->scene(), this->scenePos(), recieved->scenePos(), &this->connection_m, recieved->get_connect_list());
            this->attach(node);
            recieved->attach(node);
        }
        //reference_m->attach(*(*reinterpret_cast<Out_Port_Graphics**>(event->mimeData()->data("out_port").data()))->get_reference());    //in mime_data is stored pointer(8 bytes) on out_port, so pointer on data is pointer on pointer..
        //TODO draw cubic bezier
    }
}

void In_Port_Graphics::compute_color()
{
    QCryptographicHash hasher(QCryptographicHash::Sha1);
    type_id_t id = reference_m->get_type();
    hasher.addData(QByteArray((const char*)&id, sizeof(type_id_t)));
    QByteArray res = hasher.result();
    color_m = QColor(abs(static_cast<int>(*(res.data()))), abs(static_cast<int>(*(res.data()+8))), abs(static_cast<int>(*(res.data()+16))));
}



Out_Port_Graphics::Out_Port_Graphics(QGraphicsItem *parent, value_i *reference, unsigned int index) : QGraphicsWidget(parent)
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
            throw 15;
        }
        else
        {
            if (recieved->access_backend()->get_type() != this->reference_m->get_type())
            {
                QMessageBox msgBox;
                msgBox.setText("Type mistmatch!");
                msgBox.exec();
            }
            else
            {
                if (!recieved->is_free())
                {
                    recieved->free();
                }
                recieved->access_backend()->attach(*reference_m);
                Node_Graphics * node = new Node_Graphics(this->scene(), recieved->scenePos(), this->scenePos(), recieved->get_connection(), this->get_connect_list());
                recieved->attach(node);
                connections_m.push_back(node);
            }
        }
    }
}

void Out_Port_Graphics::compute_color()
{
    QCryptographicHash hasher(QCryptographicHash::Sha1);
    type_id_t id = reference_m->get_type();
    hasher.addData(QByteArray((const char*)&id, sizeof(type_id_t)));
    QByteArray res = hasher.result();
    color_m = QColor(abs(static_cast<int>(*(res.data()))), abs(static_cast<int>(*(res.data()+8))), abs(static_cast<int>(*(res.data()+16))));
}


Node_Graphics::Node_Graphics(QGraphicsScene *parent, QPointF start, QPointF end, Node_Graphics **reference_in, std::list<Node_Graphics *> *reference_out)
{
    path_m = new QPainterPath();
    alter_path(start, end);
    reference_m = parent->addPath(*path_m, QPen(Qt::green, 3, Qt::SolidLine));
    reference_node_m = reference_in;
    reference_out_m = reference_out;
}

Node_Graphics::~Node_Graphics()
{
    delete path_m;
    reference_m->scene()->removeItem(reference_m);
    reference_node_m = nullptr;
    reference_out_m->remove(this);
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

Exec_Node_Graphics::Exec_Node_Graphics(QGraphicsScene *parent, QPointF start, QPointF end, Exec_Node_Graphics **reference_in, std::list<Exec_Node_Graphics *> *reference_out)
{
    path_m = new QPainterPath();
    alter_path(start, end);
    reference_m = parent->addPath(*path_m, QPen(Qt::red, 3, Qt::SolidLine));
    reference_node_m = reference_in;
    reference_out_m = reference_out;
}

Exec_Node_Graphics::~Exec_Node_Graphics()
{
    delete path_m;
    reference_m->scene()->removeItem(reference_m);
    reference_node_m = nullptr;
    reference_out_m->remove(this);
}

void Exec_Node_Graphics::alter_path(QPointF start, QPointF end)
{
    start_m = QPoint(start.x() + 16, start.y() + 16);   // in port + circle radius
    end_m = QPoint(end.x() + 16, end.y() + 16); //out_port + circle radius
    path_m->moveTo(start_m);
    int divider = start_m.x() < end_m.x() ? 2 : 1;
    int length = (start_m - end_m).manhattanLength() / divider;
    path_m->cubicTo(start_m.x() + length, start_m.y(), end_m.x() - length, end_m.y(), end_m.x(), end_m.y());
}

void Exec_Node_Graphics::alter_path(QPointF start)
{
    start_m = QPoint(start.x() + 16, start.y() + 16);   // in port + circle radius
    *path_m = QPainterPath();
    path_m->moveTo(start_m);
    int divider = start_m.x() < end_m.x() ? 2 : 1;
    int length = (start_m - end_m).manhattanLength() / divider;
    path_m->cubicTo(start_m.x() + length, start_m.y(), end_m.x() - length, end_m.y(), end_m.x(), end_m.y());
    reference_m->setPath(*path_m);
}

void Exec_Node_Graphics::alter_path(int padding, QPointF end)
{
    end_m = QPoint(end.x() + 16, end.y() + 16); //out_port + circle radius
    *path_m = QPainterPath();
    path_m->moveTo(start_m);
    int divider = start_m.x() < end_m.x() ? 2 : 1;
    int length = (start_m - end_m).manhattanLength() / divider;
    path_m->cubicTo(start_m.x() + length, start_m.y(), end_m.x() - length, end_m.y(), end_m.x(), end_m.y());
    reference_m->setPath(*path_m);
}

In_Exec_Graphics::In_Exec_Graphics(QGraphicsItem *parent, block **reference) : QGraphicsWidget(parent)
{
    reference_m = reference;
    connection_m = nullptr;
    this->resize(UI_BLOCK_HEIGHT_BASE, UI_BLOCK_HEIGHT_BASE);
    this->setPos(UI_BLOCK_WIDTH_BASE -20, -20);
    setAcceptDrops(true);
}

block **In_Exec_Graphics::access_backend()
{
    return reference_m;
}

void In_Exec_Graphics::free()
{
    if (connection_m != nullptr)
        connection_m->~Exec_Node_Graphics();
}

bool In_Exec_Graphics::is_free()
{
    return connection_m == nullptr ? true : false;
}

void In_Exec_Graphics::attach(Exec_Node_Graphics *node)
{
    connection_m = node;
}

void In_Exec_Graphics::moved()
{
    if (connection_m != nullptr)
        connection_m->alter_path(this->scenePos());
}

Exec_Node_Graphics **In_Exec_Graphics::get_connection()
{
    return &connection_m;
}

void In_Exec_Graphics::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setBrush(Qt::red);
    QRectF size(10.0, 10.0, 20.0, 20.0);
    painter->drawRect(size);
}

void In_Exec_Graphics::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
        offset = event->pos();
}

void In_Exec_Graphics::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
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

        mimeData->setText("Exec from in");
        mimeData->setData("in_exec", prep_data);
        drag->setMimeData(mimeData);

        Qt::DropAction dropAction = drag->exec(Qt::CopyAction | Qt::MoveAction);
}

void In_Exec_Graphics::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
    if (event->mimeData()->hasFormat("text/plain"))
    {
        event->acceptProposedAction();
    }
}


void In_Exec_Graphics::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    if (event->mimeData()->text() == "Exec from out")
    {
        Out_Exec_Graphics * recieved = (*reinterpret_cast<Out_Exec_Graphics**>(event->mimeData()->data("out_exec").data()));
        if (recieved == nullptr)
            throw 1;
        else
        {
            if (!this->is_free())
            {
                this->free();   //destroy previous connection
            }
            *reference_m = recieved->get_reference();
            //reference_m->attach(*(recieved->get_reference()));
            Exec_Node_Graphics * node = new Exec_Node_Graphics(this->scene(), this->scenePos(), recieved->scenePos(), &this->connection_m, recieved->get_connect_list());
            this->attach(node);
            recieved->attach(node);
        }
        //reference_m->attach(*(*reinterpret_cast<Out_Port_Graphics**>(event->mimeData()->data("out_port").data()))->get_reference());    //in mime_data is stored pointer(8 bytes) on out_port, so pointer on data is pointer on pointer..
        //TODO draw cubic bezier
    }
}

Out_Exec_Graphics::Out_Exec_Graphics(QGraphicsItem *parent, block *reference) : QGraphicsWidget(parent)
{
    reference_m = reference;
    this->resize(UI_BLOCK_HEIGHT_BASE, UI_BLOCK_HEIGHT_BASE);
    this->setPos(-20 , -20);
    setAcceptDrops(true);
}

void Out_Exec_Graphics::attach(Exec_Node_Graphics *node)
{
    connections_m.push_back(node);
}

block *Out_Exec_Graphics::get_reference()
{
    return reference_m;
}

std::list<Exec_Node_Graphics *> *Out_Exec_Graphics::get_connect_list()
{
    return &connections_m;
}

void Out_Exec_Graphics::moved()
{
    for (auto itr = connections_m.begin(); itr != connections_m.end(); itr++)
    {
        (*itr)->alter_path(0, this->scenePos());
    }
}

void Out_Exec_Graphics::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setBrush(Qt::red);
    QRectF size(10.0, 10.0, 20.0, 20.0);
    painter->drawRect(size);
}

void Out_Exec_Graphics::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
        offset = event->pos();
}

void Out_Exec_Graphics::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
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

        mimeData->setText("Exec from out");
        mimeData->setData("out_exec", prep_data);
        drag->setMimeData(mimeData);

        Qt::DropAction dropAction = drag->exec(Qt::CopyAction | Qt::MoveAction);
}

void Out_Exec_Graphics::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
    if (event->mimeData()->hasFormat("text/plain"))
    {
        event->acceptProposedAction();
    }
}

void Out_Exec_Graphics::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    if (event->mimeData()->text() == "Exec from in")
    {
        In_Exec_Graphics * recieved = (*reinterpret_cast<In_Exec_Graphics**>(event->mimeData()->data("in_exec").data()));
        if (recieved == nullptr)
        {
            throw 15;
        }
        else
        {
            if (!recieved->is_free())
            {
                recieved->free();
            }
            *recieved->access_backend()=this->reference_m;
            Exec_Node_Graphics * node = new Exec_Node_Graphics(this->scene(), recieved->scenePos(), this->scenePos(), recieved->get_connection(), this->get_connect_list());
            recieved->attach(node);
            connections_m.push_back(node);
        }
    }
}

Start_Graphics::Start_Graphics(QGraphicsItem *parent, block **reference)
{
    this->resize(UI_BLOCK_WIDTH_BASE, UI_BLOCK_HEIGHT_BASE);
    this->setPos(500, 1000);
    reference_m = reference;
    in_exec_m = new In_Exec_Graphics(this, reference);
    in_exec_m->setPos(UI_BLOCK_WIDTH_BASE/2 - 20, 0);
}

void Start_Graphics::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRect size = QRect(0 , 0 , UI_BLOCK_WIDTH_BASE/2, UI_BLOCK_HEIGHT_BASE);

    QLinearGradient linearGrad(0 , 0 , UI_BLOCK_WIDTH_BASE/2, UI_BLOCK_HEIGHT_BASE);
    if (this->isSelected())
    {
        linearGrad.setColorAt(0, Qt::white);
        linearGrad.setColorAt(0.5, Qt::green);
        linearGrad.setColorAt(1, Qt::gray);
    }
    else
    {
        linearGrad.setColorAt(0, Qt::white);
        //linearGrad.setColorAt(0.5, Qt::gray);
        linearGrad.setColorAt(1, Qt::gray);
    }

    painter->setBrush(linearGrad);
    painter->drawRoundedRect(size, 10.0, 10.0);

    painter->setPen(Qt::black);
    QFont sansFont("Helvetica [Cronyx]", 12);
    painter->setFont(sansFont);
    painter->drawText(size, Qt::AlignHCenter, "START");
    painter->setPen(QPen(Qt::gray, 3, Qt::DashDotLine, Qt::RoundCap));
    //painter->drawLine(QPoint(0, UI_BLOCK_HEADER_LINE_OFFSET), QPoint(UI_BLOCK_WIDTH_BASE, UI_BLOCK_HEADER_LINE_OFFSET));
}



Constant_Graphics::Constant_Graphics(QGraphicsItem *parent, value_t value, type_id_t type_id, std::string name) : QGraphicsWidget(parent)
{
    reference_m = new constant(type_id, value);
    name_m = (QString::number(value) + " " + QString::fromStdString(name)).toStdString();
    this->resize(UI_BLOCK_WIDTH_BASE, UI_BLOCK_HEIGHT_BASE*2);
    this->setFlag(QGraphicsWidget::ItemIsMovable);
    this->setFlag(QGraphicsWidget::ItemIsSelectable);
    this->setFlag(QGraphicsWidget::ItemIsFocusable);
    out_port_m = new Out_Port_Graphics(this, reference_m, 0);
}

Constant_Graphics::~Constant_Graphics()
{
    delete reference_m;
}

void Constant_Graphics::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRect size = QRect(0 , 0 , UI_BLOCK_WIDTH_BASE, UI_BLOCK_HEIGHT_BASE*2);

    QLinearGradient linearGrad(0 , 0 , UI_BLOCK_WIDTH_BASE, UI_BLOCK_HEIGHT_BASE*2);
    if (this->isSelected())
    {
        linearGrad.setColorAt(0, Qt::white);
        linearGrad.setColorAt(0.5, Qt::green);
        linearGrad.setColorAt(1, Qt::gray);
    }
    else
    {
        linearGrad.setColorAt(0, Qt::white);
        linearGrad.setColorAt(1, Qt::gray);
    }

    painter->setBrush(linearGrad);
    painter->drawRoundedRect(size, 10.0, 10.0);

    painter->setPen(Qt::black);
    QFont sansFont("Helvetica [Cronyx]", 12);
    painter->setFont(sansFont);
    painter->drawText(size, Qt::AlignHCenter, QString::fromStdString(name_m));
    painter->setPen(QPen(Qt::gray, 3, Qt::DashDotLine, Qt::RoundCap));
    painter->drawLine(QPoint(0, UI_BLOCK_HEADER_LINE_OFFSET), QPoint(UI_BLOCK_WIDTH_BASE, UI_BLOCK_HEADER_LINE_OFFSET));
}

void Constant_Graphics::moveEvent(QGraphicsSceneMoveEvent *event)
{
    out_port_m->moved();
}
