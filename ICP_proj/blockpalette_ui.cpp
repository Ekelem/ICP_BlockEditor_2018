#include "blockpalette_ui.h"

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

    create_default_conversion(conversion);

    QTreeWidgetItem *extra = new QTreeWidgetItem(this);
    extra->setText(0, tr("Extra"));

    create_default_extra(extra);
}

void BlockPalette_UI::create_default_aritmetic(QTreeWidgetItem *parent)
{
    create_unit(parent, "b_add_kg_to_kg");
    create_unit(parent, "b_add_g_to_g");
    create_unit(parent, "b_add_mg_to_mg");
    create_unit(parent, "b_add_t_to_t");
    create_unit(parent, "b_add_lb_to_lb");

    create_unit(parent, "b_sub_kg_from_kg");
    create_unit(parent, "b_sub_g_from_g");
    create_unit(parent, "b_sub_mg_from_mg");
    create_unit(parent, "b_sub_t_from_t");
    create_unit(parent, "b_sub_lb_from_lb");

    create_unit(parent, "b_mul_kg");
    create_unit(parent, "b_mul_g");
    create_unit(parent, "b_mul_mg");
    create_unit(parent, "b_mul_t");
    create_unit(parent, "b_mul_lb");
}

void BlockPalette_UI::create_default_conversion(QTreeWidgetItem *parent)
{
    create_unit(parent, "b_conv_kg_to_g");
    create_unit(parent, "b_conv_kg_to_mg");
    create_unit(parent, "b_conv_kg_to_t");
    create_unit(parent, "b_conv_kg_to_lb");

    create_unit(parent, "b_conv_g_to_kg");
    create_unit(parent, "b_conv_mg_to_kg");
    create_unit(parent, "b_conv_t_to_kg");
    create_unit(parent, "b_conv_lb_to_kg");

    create_unit(parent, "b_conv_kg_to_nthg");
    create_unit(parent, "b_conv_g_to_nthg");
    create_unit(parent, "b_conv_mg_to_nthg");
    create_unit(parent, "b_conv_t_to_nthg");
    create_unit(parent, "b_conv_lb_to_nthg");
}

void BlockPalette_UI::create_default_extra(QTreeWidgetItem *parent)
{
    create_unit(parent, "b_output");
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
