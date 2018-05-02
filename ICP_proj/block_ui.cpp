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
        prep_data.setRawData(reinterpret_cast<char*>(&reference_m), 8);     //We assume only 32 bit architecture (4 bytes long pointers) or 64 (8 bytes long pointers)

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
        prep_data.setRawData(reinterpret_cast<char*>(&reference_m), 8);     //We assume only 32 bit architecture (4 bytes long pointers) or 64 (8 bytes long pointers)

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
        (*reinterpret_cast<in_port**>(event->mimeData()->data("out_port").data()))->attach(*reference_m);    //in mime_data is stored pointer(8 bytes) on out_port, so pointer on data is pointer on pointer..
        //TODO draw cubic bezier
    }
}
