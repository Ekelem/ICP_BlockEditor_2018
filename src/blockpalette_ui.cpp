#include "blockpalette_ui.h"
#include "mainwindow.h"

BlockPalette_UI::BlockPalette_UI(QWidget *parent) : QTreeWidget(parent)
{
    create_default();
}

void BlockPalette_UI::create_unit(QTreeWidgetItem * parent, QString name)
{
    QTreeWidgetItem * NEW_UNIT = new QTreeWidgetItem(parent);
    QLabel * text = new BlockName_UI(this, name);
    this->setItemWidget(NEW_UNIT, 0, text);
}

void BlockPalette_UI::create_default()
{
    QTreeWidgetItem *aritmetic = new QTreeWidgetItem(this);
    aritmetic->setText(0, tr("Aritmetic"));

    create_default_aritmetic(aritmetic);

    QTreeWidgetItem *conversion = new QTreeWidgetItem(this);
    conversion->setText(0, tr("Conversion"));


    QHBoxLayout *mid = new QHBoxLayout(this);
    QLabel *label1 = new QLabel();
    label1->setMaximumSize(70,30);
    label1->setText("Value:");
    QLabel *label2 = new QLabel();
    label2->setText("in");
    label2->setMaximumSize(70,30);
    QLineEdit *value1 = new QLineEdit();
    value1->setObjectName("value");
    value1->setMaximumWidth(80);
    QLineEdit *value2 = new QLineEdit();
    value2->setMaximumWidth(80);
    value2->setObjectName("type");

    mid->addWidget(label1);
    mid->addWidget(value1);
    mid->addWidget(label2);
    mid->addWidget(value2);
}

void BlockPalette_UI::create_default_aritmetic(QTreeWidgetItem *aritmetic)
{
    create_unit(aritmetic, "b_add_kg_to_kg");
    create_unit(aritmetic, "b_add_g_to_g");
    create_unit(aritmetic, "b_add_mg_to_mg");
}

BlockName_UI::BlockName_UI(QWidget *parent, QString name) : QLabel(name, parent)
{

}

void BlockName_UI::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
        dragStartPosition = event->pos();
}

void BlockName_UI::mouseMoveEvent(QMouseEvent *event)
{
    if (!(event->buttons() & Qt::LeftButton))
            return;
        if ((event->pos() - dragStartPosition).manhattanLength()
             < QApplication::startDragDistance())
            return;

        QDrag *drag = new QDrag((QWidget*)this);
        QMimeData *mimeData = new QMimeData;

        QByteArray data = QByteArray(this->text().toLatin1());

        mimeData->setText("Create Block");
        mimeData->setData("block", data);
        drag->setMimeData(mimeData);

        Qt::DropAction dropAction = drag->exec(Qt::CopyAction | Qt::MoveAction);
}

