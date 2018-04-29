#ifndef BLOCK_UI_H
#define BLOCK_UI_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QApplication>

#define UI_BLOCK_HEADER_LINE_OFFSET 30
#define UI_BLOCK_WIDTH_BASE 120
#define UI_BLOCK_HEIGHT_BASE 60

class Block_UI : public QWidget
{
    Q_OBJECT
public:
    explicit Block_UI(QWidget *parent = nullptr);
protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
private:
    QPoint offset;
signals:

public slots:
};

#endif // BLOCK_UI_H
