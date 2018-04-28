#include "myarea.h"
#include "mainwindow.h"
#include <QtWidgets>

MyArea::MyArea(QWidget *parent, QMainWindow *s) : QFrame(parent){
    Q_UNUSED(s);
    setFrameStyle(QFrame::Sunken | QFrame::StyledPanel);
    setAcceptDrops(true);
    setMouseTracking(true);

    layout = new QVBoxLayout();
    setLayout(layout);
    drawFrom = QPoint();
    drawTo = QPoint();

    scribbling = false;
    myPenWidth = 2;
    myPenColor = Qt::black;
}


MyArea::~MyArea(){}

void MyArea::setActualBlock(int type) {
    actualBlockType = type;
}


void MyArea::setPenColor(const QColor &newColor)
{
    myPenColor = newColor;
}

void MyArea::setPenWidth(int newWidth)
{
    myPenWidth = newWidth;
}

void MyArea::clearImage()
{
    image.fill(qRgb(255, 255, 255));
    modified = true;
    update();
}


void MyArea::resizeEvent(QResizeEvent *event)
{
    if (width() > image.width() || height() > image.height()) {
        int newWidth = qMax(width() + 128, image.width());
        int newHeight = qMax(height() + 128, image.height());
        resizeImage(&image, QSize(newWidth, newHeight));
        update();
    }
    QWidget::resizeEvent(event);
}

void MyArea::drawLineTo(const QPoint &endPoint)
{
    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::HighQualityAntialiasing);
    painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));
    painter.drawLine(drawFrom, endPoint);
    modified = true;

    int rad = (myPenWidth / 2) + 2;
    update(QRect(drawFrom, endPoint).normalized()
                                     .adjusted(-rad, -rad, +rad, +rad));
    drawFrom = endPoint;
}

void MyArea::resizeImage(QImage *image, const QSize &newSize)
{
    if (image->size() == newSize)
        return;

    QImage newImage(newSize, QImage::Format_RGB32);
    newImage.fill(qRgb(255, 255, 255));
    QPainter painter(&newImage);
    painter.drawImage(QPoint(0, 0), *image);
    *image = newImage;
}

void MyArea::create_block() {
    QLabel *blockIcon = new QLabel(this);
    blockIcon->setScaledContents(true);
    blockIcon->setFixedSize(70,40);
    blockIcon->show();
    blockIcon->setEnabled(true);
    blockIcon->setAttribute(Qt::WA_DeleteOnClose);



    switch (actualBlockType) {
        case B_ADD:
            blockIcon->setPixmap(QPixmap(":/add.png"));
            break;
        case B_SUB:
            blockIcon->setPixmap(QPixmap(":/minus.png"));
            break;
        case B_VALUE:
            QPixmap l(":/white.jpg");
            QPainter p(&l);
            p.setFont(QFont("times",14));
            blockIcon->setFixedSize(70,40);
            p.drawText(0,30,QString::number(blockValue));
            blockIcon->setPixmap(l);
            break;
    }
}


void MyArea::initialize() {
    createObjects();
}

void MyArea::createObjects() {
    menu = new QHBoxLayout();
    layout->addLayout(menu);

}


void MyArea::setObjectsToMoveable(bool isMoveAble) {
    if (isMoveAble)
        mode = OBJECTS_ARE_DRAGGABLE;
    else
        mode = OBJECTS_ARE_CONNECTABLE;
}



void MyArea::dragEnterEvent(QDragEnterEvent *event)
{
    if (mode != OBJECTS_ARE_DRAGGABLE)
        return;

    if (event->mimeData()->hasFormat("application/x-dnditemdata")) {
        if (event->source() == this) {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        } else {
            event->acceptProposedAction();
        }
    } else {
        event->ignore();
    }
}

void MyArea::dragMoveEvent(QDragMoveEvent *event)
{
    if (mode != OBJECTS_ARE_DRAGGABLE)
        return;
    if (event->mimeData()->hasFormat("application/x-dnditemdata")) {
        if (event->source() == this) {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        } else {
            event->acceptProposedAction();
        }
    } else {
        event->ignore();
    }
}

void MyArea::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasFormat("application/x-dnditemdata")) {
        drawTo = event->pos();

        QByteArray itemData = event->mimeData()->data("application/x-dnditemdata");
        QDataStream dataStream(&itemData, QIODevice::ReadOnly);

        QPixmap pixmap;
        QPoint offset;
        dataStream >> pixmap >> offset;

        QLabel *newIcon = new QLabel(this);
        newIcon->setPixmap(pixmap);
        newIcon->move(event->pos() - offset);
        newIcon->show();
        newIcon->setScaledContents(true);
        newIcon->setFixedSize(70,40);
        newIcon->setAttribute(Qt::WA_DeleteOnClose);
//        newIcon->setStyleSheet("border: 1px solid black");

        if (event->source() == this) {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        } else {
            event->acceptProposedAction();
        }
    } else {
        event->ignore();
    }
}

void MyArea::mouseMoveEvent(QMouseEvent *event) {
    QLineF *child = (QLineF*)(childAt(event->pos()));
    if (!child)
        return;
}


void MyArea::mousePressEvent(QMouseEvent *event)
{
    QLabel *child = static_cast<QLabel*>(childAt(event->pos()));
    if (!child)
        return;

    //only labels are movable
    if (strcmp(child->metaObject()->className(), "QLabel") != 0)
        return;

    source = child;
    if (mode == OBJECTS_ARE_CONNECTABLE) {
        if (event->button() == Qt::LeftButton) {
            drawFrom = event->pos();
            scribbling = true;
            return;
        }
    }

    drawFrom = event->pos();

    QPixmap pixmap = *child->pixmap();


    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);
    dataStream << pixmap << QPoint(event->pos() - child->pos());

    QMimeData *mimeData = new QMimeData;
    mimeData->setData("application/x-dnditemdata", itemData);

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->setPixmap(pixmap);
    drag->setHotSpot(event->pos() - child->pos());

    QPixmap tempPixmap = pixmap;
    QPainter painter;
    painter.begin(&tempPixmap);
    painter.fillRect(pixmap.rect(), QColor(127, 127, 127, 127));
    painter.end();

    child->setPixmap(tempPixmap);

    if (drag->exec(Qt::CopyAction | Qt::MoveAction, Qt::CopyAction) == Qt::MoveAction) {
        child->close();
    } else {
        child->show();
        child->setPixmap(pixmap);
    }
}

void MyArea::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::HighQualityAntialiasing);
    QRect dirtyRect = event->rect();
    painter.drawImage(dirtyRect, image, dirtyRect);
}


void MyArea::mouseReleaseEvent(QMouseEvent *event) {
    QLabel *child = static_cast<QLabel*>(childAt(event->pos()));
    if (!child)
        return;

    if (strcmp(child->metaObject()->className(), "QLabel") != 0)
        return;

    destination = child;

    if (source == destination)
        return;

    if (event->button() == Qt::LeftButton && scribbling && mode == OBJECTS_ARE_CONNECTABLE) {
        drawLineTo(event->pos());
        scribbling = false;
    }
}

bool MyArea::openImage(const QString &fileName) {
    QImage loadedImage;
    if (!loadedImage.load(fileName))
        return false;

    QSize newSize = loadedImage.size().expandedTo(size());
    resizeImage(&loadedImage, newSize);
    image = loadedImage;
    modified = false;
    update();
    return true;
}

bool MyArea::saveImage(const QString &fileName, const char *fileFormat) {
    QImage visibleImage = image;
    resizeImage(&visibleImage, size());

    if (visibleImage.save(fileName, fileFormat)) {
        modified = false;
        return true;
    } else {
        return false;
    }
}
