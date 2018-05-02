#ifndef BLOCK_UI_H
#define BLOCK_UI_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QApplication>
#include <QMimeData>
#include <QDrag>

#include "block.h"

#define UI_BLOCK_HEADER_LINE_OFFSET 30
#define UI_BLOCK_WIDTH_BASE 120
#define UI_BLOCK_HEIGHT_BASE 40

class Block_UI : public QWidget
{
    Q_OBJECT
public:
    explicit Block_UI(QWidget *parent = nullptr, block * reference = nullptr, QString name = "Name");
protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
private:

    void setup_block();
    QPoint offset;
    QString name_m;
    block * reference_m;
    unsigned int height_m;
signals:

public slots:
};

class In_Port_UI : public QWidget
{
    Q_OBJECT
public:
    explicit In_Port_UI(QWidget *parent = nullptr, in_port * reference = nullptr, unsigned int index = 0);
protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
private:
    QPoint offset;
    in_port * reference_m;
signals:

public slots:
};

class Out_Port_UI : public QWidget
{
    Q_OBJECT
public:
    explicit Out_Port_UI(QWidget *parent = nullptr, out_port * reference = nullptr, unsigned int index = 0);
protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
private:
    QPoint offset;
    out_port * reference_m;
signals:

public slots:
};

#endif // BLOCK_UI_H
