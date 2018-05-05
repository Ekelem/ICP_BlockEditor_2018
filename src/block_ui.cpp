#include "block_ui.h"
#include "canvas_ui.h"
#include "mainwindow.h"

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

    /*QPainter painter(this);
    QPainterPath path;
    path.moveTo(0, 0);
    path.cubicTo(200, 80, 320, 80, 480, 320);

    painter.setPen(QPen(Qt::black, 8));
    painter.drawPath(path);*/


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
            new Node_UI((QWidget*)this->parent()->parent(), recieved, this);
        }
    }
}

Node_UI::Node_UI(QWidget *parent, In_Port_UI *reference_in, Out_Port_UI *reference_out) : QWidget(parent)
{
    reference_in_m = reference_in;
    reference_out_m = reference_out;

    //this->adjustSize();
    this->setFixedSize(1000, 1000);
    this->show();
}

void Node_UI::paintEvent(QPaintEvent *)
{
    //this->adjustSize();
    QPainter painter(this);
    QPainterPath path;
    //path.moveTo(0, 0);
    QWidget * in_block =(QWidget*)reference_in_m->parent();
    path.moveTo(reference_in_m->pos()+ in_block->pos());
    path.cubicTo(200, 80, 320, 80, 480, 320);
    //path.cubicTo(-30, 0, reference_out_m->pos().x() + 30, reference_out_m->pos().y(), reference_out_m->pos().x(), reference_out_m->pos().y());

    painter.setPen(QPen(Qt::black, 8));
    painter.drawPath(path);
    //this->adjustSize();
    //this->updateGeometry();
}

















Block_Graphics::Block_Graphics(QGraphicsItem *parent, block *reference, QString name) : QGraphicsWidget(parent)
{
    height_m = UI_BLOCK_HEIGHT_BASE;
    name_m = name;
    reference_m = reference;
    this->resize(UI_BLOCK_WIDTH_BASE, UI_BLOCK_HEIGHT_BASE);
    setup_block();
}

QString Block_Graphics::get_name_m() {
    return name_m;
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
            new In_Port_Graphics(this, reference_m->get_in_port(i), i);

        for (unsigned int i = 0; i < reference_m->get_out_size(); i++)   //visualize out_ports
            new Out_Port_Graphics(this, reference_m->get_out_port(i), i);
    }
}

In_Port_Graphics::In_Port_Graphics(QGraphicsItem *parent, in_port *reference, unsigned int index) : QGraphicsWidget(parent)
{
    reference_m = reference;
    this->resize(UI_BLOCK_HEIGHT_BASE, UI_BLOCK_HEIGHT_BASE);
    this->setPos(-10, UI_BLOCK_HEIGHT_BASE * (index+1));
    setAcceptDrops(true);
}

void In_Port_Graphics::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setBrush(Qt::gray);
    painter->drawEllipse({16, 16}, 16, 16);
}


void In_Port_Graphics::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        offset = event->pos();

        Scene_Graphics *sc = (Scene_Graphics *)(this->scene());
        sc->temporary_in = this;

    }
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
    if (event->mimeData()->hasFormat("text/plain"))
    {
        event->acceptProposedAction();
    }
}

void In_Port_Graphics::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    if (event->mimeData()->text() == "Connect from out")
    {
        reference_m->attach(*(*reinterpret_cast<Out_Port_Graphics**>(event->mimeData()->data("out_port").data()))->get_reference());    //in mime_data is stored pointer(8 bytes) on out_port, so pointer on data is pointer on pointer..
        //TODO draw cubic bezier
    }
}



Out_Port_Graphics::Out_Port_Graphics(QGraphicsItem *parent, out_port *reference, unsigned int index) : QGraphicsWidget(parent)
{
    reference_m = reference;
    this->resize(UI_BLOCK_HEIGHT_BASE, UI_BLOCK_HEIGHT_BASE);
    this->setPos(UI_BLOCK_WIDTH_BASE - 24, UI_BLOCK_HEIGHT_BASE * (index+1));
    setAcceptDrops(true);
}

void Out_Port_Graphics::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setBrush(Qt::gray);
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
            Scene_Graphics *sc = (Scene_Graphics *)(this->scene());
            sc->temporary_in->out_port_pointer = this;
            this->in_port_pointers.push_back(sc->temporary_in);

            sc->temporary_in = nullptr;

            //recieved->access_backend();
            //recieved->access_backend()->attach(*reference_m);    //in mime_data is stored pointer(8 bytes) on out_port, so pointer on data is pointer on pointer..
            //TODO draw cubic bezier
            QPainterPath path = QPainterPath();
            QPoint start = QPoint(recieved->scenePos().x() + 16, recieved->scenePos().y() + 16);
            path.moveTo(start);
            path.cubicTo(recieved->scenePos().x() - 200, recieved->scenePos().y(), this->scenePos().x() + 200, this->scenePos().y(), this->scenePos().x() + 16, this->scenePos().y() + 16);
            QGraphicsPathItem * connection = this->scene()->addPath(path, QPen(Qt::green, 3, Qt::SolidLine));
        }
    }
}


Start_Graphics::Start_Graphics(QGraphicsItem *parent, block * reference) {
    height_m = UI_BLOCK_HEIGHT_BASE;
    reference_m = reference;
    this->resize(UI_BLOCK_WIDTH_BASE, UI_BLOCK_HEIGHT_BASE);
    this->setFlag(QGraphicsWidget::ItemIsMovable);
    this->setPos(100,300);
    setup_block();
}

void Start_Graphics::setup_block()
{
    if (reference_m != nullptr) {
        height_m = (reference_m->get_max_size()+1) * UI_BLOCK_HEIGHT_BASE;
        this->resize(UI_BLOCK_WIDTH_BASE, height_m);
        new Out_Port_Graphics(this, reference_m->get_out_port(0), 0);
    }
}

void Start_Graphics::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRect size = QRect(0 , 0 , UI_BLOCK_WIDTH_BASE, height_m);

    QLinearGradient linearGrad(0 , 0 , UI_BLOCK_WIDTH_BASE*2, height_m*2);
        linearGrad.setColorAt(0, Qt::white);
        linearGrad.setColorAt(0.5, Qt::gray);
        linearGrad.setColorAt(1, Qt::black);

    painter->setBrush(linearGrad);
    painter->drawRoundedRect(size, 10.0, 10.0);
    painter->setPen(Qt::black);
    QFont sansFont("Helvetica [Cronyx]", 12);
    painter->setFont(sansFont);
    painter->drawText(size, Qt::AlignHCenter, "START");
    painter->setPen(QPen(Qt::gray, 3, Qt::DashDotLine, Qt::RoundCap));
    painter->drawLine(QPoint(0, UI_BLOCK_HEADER_LINE_OFFSET), QPoint(UI_BLOCK_WIDTH_BASE, UI_BLOCK_HEADER_LINE_OFFSET));
}




End_Graphics::End_Graphics(QGraphicsItem *parent, block * reference) {
    height_m = UI_BLOCK_HEIGHT_BASE;
    reference_m = reference;
    this->resize(UI_BLOCK_WIDTH_BASE, UI_BLOCK_HEIGHT_BASE);
    this->setFlag(QGraphicsWidget::ItemIsMovable);
    this->setPos(1220,300);
    setup_block();
}

void End_Graphics::setup_block()
{
    if (reference_m != nullptr) {
        height_m = (reference_m->get_max_size()+1) * UI_BLOCK_HEIGHT_BASE;
        this->resize(UI_BLOCK_WIDTH_BASE, height_m);
        new In_Port_Graphics(this, reference_m->get_in_port(0), 0);
    }
}

void End_Graphics::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRect size = QRect(0 , 0 , UI_BLOCK_WIDTH_BASE, height_m);

    QLinearGradient linearGrad(0 , 0 , UI_BLOCK_WIDTH_BASE*2, height_m*2);
        linearGrad.setColorAt(0, Qt::white);
        linearGrad.setColorAt(0.5, Qt::gray);
        linearGrad.setColorAt(1, Qt::black);

    painter->setBrush(linearGrad);
    painter->drawRoundedRect(size, 10.0, 10.0);
    painter->setPen(Qt::black);
    QFont sansFont("Helvetica [Cronyx]", 12);
    painter->setFont(sansFont);
    painter->drawText(size, Qt::AlignHCenter, "END");
    painter->setPen(QPen(Qt::gray, 3, Qt::DashDotLine, Qt::RoundCap));
    painter->drawLine(QPoint(0, UI_BLOCK_HEADER_LINE_OFFSET), QPoint(UI_BLOCK_WIDTH_BASE, UI_BLOCK_HEADER_LINE_OFFSET));
}



















Value_Graphics::Value_Graphics(QGraphicsItem *parent, block *reference, QString name, QString value) : QGraphicsWidget(parent)
{
    height_m = UI_BLOCK_HEIGHT_BASE;
    name_m = name;
    value_m = value;
    reference_m = reference;
    this->resize(UI_BLOCK_WIDTH_BASE, UI_BLOCK_HEIGHT_BASE);
    setup_block();
}

QString Value_Graphics::get_name_m() {
    return name_m;
}


void Value_Graphics::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
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
    painter->drawText(size, Qt::AlignCenter, value_m);
    painter->setPen(QPen(Qt::gray, 3, Qt::DashDotLine, Qt::RoundCap));
    painter->drawLine(QPoint(0, UI_BLOCK_HEADER_LINE_OFFSET), QPoint(UI_BLOCK_WIDTH_BASE, UI_BLOCK_HEADER_LINE_OFFSET));
}

void Value_Graphics::setup_block()
{
    if (reference_m != nullptr)
    {
        height_m = (reference_m->get_max_size()+1) * UI_BLOCK_HEIGHT_BASE;
        this->resize(UI_BLOCK_WIDTH_BASE, height_m);
        new In_Port_Graphics(this, reference_m->get_in_port(0), 0);
        new Out_Port_Graphics(this, reference_m->get_out_port(0), 0);
    }
}
