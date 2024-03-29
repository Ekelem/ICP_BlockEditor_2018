/*!
 * @file
 * @brief Basic palette at the side panel
 *
 * @author Erik Kelemen, xkelem01@stud.fit.vutbr.cz
 * @author Attila Lakatos, xlakat01@stud.fit.vutbr.cz
 */

#ifndef BLOCKPALETTE_UI_H
#define BLOCKPALETTE_UI_H

#include <QWidget>
#include <QTreeWidget>
#include <QMouseEvent>
#include <QApplication>
#include <QLabel>
#include <QMimeData>
#include <QString>
#include <QDrag>

#include <iostream>

class BlockPalette_UI : public QTreeWidget
{
    //Q_OBJECT
public:
    explicit BlockPalette_UI(QWidget *parent = nullptr);
private:
    void create_unit(QTreeWidgetItem * parent, QString name);
    void create_default();
    void create_default_aritmetic(QTreeWidgetItem * parent);
    void create_default_conversion(QTreeWidgetItem * parent);
    void create_default_extra(QTreeWidgetItem * parent);
};

class BlockName_UI : public QLabel
{
    //Q_OBJECT
public:
    explicit BlockName_UI(QWidget *parent = nullptr, QString name = "Name");
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
private:
    QPoint dragStartPosition;
};

#endif // BLOCKPALETTE_UI_H
