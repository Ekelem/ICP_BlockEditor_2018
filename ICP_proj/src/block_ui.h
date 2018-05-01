/**
 * @file
 * @brief Basic class of a block-frontend
 *
 * @author Erik Kelemen, xkelem01@stud.fit.vutbr.cz
 * @author Attila Lakatos, xlakat01@stud.fit.vutbr.cz
 */

#ifndef BLOCK_UI_H
#define BLOCK_UI_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QApplication>
#include <QMimeData>

#define UI_BLOCK_HEADER_LINE_OFFSET 30
#define UI_BLOCK_WIDTH_BASE 120
#define UI_BLOCK_HEIGHT_BASE 60

class Block_UI : public QWidget
{
    Q_OBJECT
public:
    explicit Block_UI(QWidget *parent = nullptr, QString name = "Name");
    QString get_name_m();
protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

    void keyPressEvent(QKeyEvent *event);
private:
    QPoint offset;
    QString name_m;
signals:

public slots:
};

#endif // BLOCK_UI_H
