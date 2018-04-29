#ifndef BLOCKPALETTE_UI_H
#define BLOCKPALETTE_UI_H

#include <QWidget>
#include <QTreeWidget>

class BlockPalette_UI : public QTreeWidget
{
public:
    explicit BlockPalette_UI(QWidget *parent = nullptr);
};

#endif // BLOCKPALETTE_UI_H
