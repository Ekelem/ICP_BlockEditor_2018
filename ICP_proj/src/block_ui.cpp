#include "block_ui.h"
#include "canvas_ui.h"

#include <iostream>
#include <string>
#include <QDebug>

Block_UI::Block_UI(QWidget *parent, QString name) : QWidget(parent)
{
    name_m = name;
    this->setFixedSize(UI_BLOCK_WIDTH_BASE, UI_BLOCK_HEIGHT_BASE);
}

QString Block_UI::get_name_m() {
    return name_m;
}


void Block_UI::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QRect size = QRect(0 , 0 , UI_BLOCK_WIDTH_BASE, UI_BLOCK_HEIGHT_BASE);

    QLinearGradient linearGrad(0 , 0 , UI_BLOCK_WIDTH_BASE*2, UI_BLOCK_HEIGHT_BASE*2);
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
    if (event->button() == Qt::LeftButton) {
        offset = event->pos();
    }
    QWidget::mousePressEvent(event);
}

void Block_UI::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton)
    {
            this->move(mapToParent(event->pos() - offset));
    }
}

void Block_UI::keyPressEvent(QKeyEvent *event){
    qDebug() << "asd";
    if(event->key() == Qt::Key_Delete) {
        delete this;
    }
}
