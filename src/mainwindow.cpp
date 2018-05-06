#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    actual_project_m = new project();
    scene_m = new Scene_Graphics(this);
    scene_m->addItem(new Canvas_Graphics(nullptr, actual_project_m));
    ui->setupUi(this);
    setWindowState(Qt::WindowMaximized);
    setWindowIcon(QIcon(":/icons/icon-c++.svg"));

    ui->Frame_BlockPalette->layout()->setAlignment(Qt::AlignTop);
    ui->canvas->setScene(scene_m);
    ui->canvas->verticalScrollBar()->setSliderPosition(1);
    ui->canvas->horizontalScrollBar()->setSliderDown(1);

    block *ptr_end   = new b_add_kg_to_kg(actual_project_m->get_type_lib());
    scene_m->end = new End_Graphics(nullptr, ptr_end);
    scene_m->addItem(scene_m->end);

}

MainWindow::~MainWindow()
{
    delete ui;
    delete actual_project_m;
}


void MainWindow::on_actionNew_File_triggered()
{
    QList<QGraphicsItem *> items = scene_m->items();
    Canvas_Graphics *graphics = (Canvas_Graphics *)items.last();
    graphics->clearBlock_id();
    items.removeLast();

    foreach( QGraphicsItem *item, items ) {
        delete item;
    }

    actual_project_m = new project();

    block *ptr_end   = new b_add_kg_to_kg(actual_project_m->get_type_lib());
    scene_m->end = new End_Graphics(nullptr, ptr_end);
    scene_m->addItem(scene_m->end);
}


void MainWindow::on_actionSave_File_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this,
        tr("Save Address Book"), "",
        tr("Block Editor files (*.be2018);;All Files (*)"));

    if (fileName.isEmpty()) {
        return;
    }

    std::string extension = fileName.toStdString().substr(fileName.length()-7);
    fileName = extension == ".be2018" ? fileName : fileName+".be2018";

    QFile outputFile(fileName);
    outputFile.open(QIODevice::WriteOnly);

    if(!outputFile.isOpen()){
        qDebug() << "- Error, unable to open" << fileName << "for output";
    }

    QTextStream outStream(&outputFile);
    QList<QGraphicsItem *> items_qlist = scene_m->items();
    items_qlist.removeLast();
    std::list<QGraphicsItem *>items = items_qlist.toStdList();
    items.reverse();

    outStream << "# BLOCKS\n";
    for (std::list<QGraphicsItem *>::iterator item = items.begin(); item != items.end(); ++item ) {
        Block_Graphics *as_block = dynamic_cast<Block_Graphics *>(*item);
        if (as_block) {
            QPointF pos = (*item)->pos();
            QString name = as_block->get_name_m();
            outStream << pos.x() << " " << pos.y() << " " << name << "\n";
        }
    }


    outStream << "# VALUES\n";
    for (std::list<QGraphicsItem *>::iterator item = items.begin(); item != items.end(); ++item ) {
        Value_Graphics *as_value = dynamic_cast<Value_Graphics *>(*item);
        if (as_value) {
            QPointF pos = (*item)->pos();
            QString name = as_value->get_name_m();
            QString value = as_value->value_m;
            outStream << pos.x() << " " << pos.y() << " " << name << " " << value << "\n";
        }
    }

    for (std::list<QGraphicsItem *>::iterator item = items.begin(); item != items.end(); ++item ) {
        End_Graphics *as_end = dynamic_cast<End_Graphics *>(*item);
        if (as_end) {
            QPointF pos = (*item)->pos();
            outStream << "# END_BLOCK\n" << pos.x() << " " << pos.y() << "\n";
        }
    }

    outStream << "# CONNECTIONS\n";
    for (std::list<QGraphicsItem *>::iterator item = items.begin(); item != items.end(); ++item ) {
        Value_Graphics *as_value = dynamic_cast<Value_Graphics *>(*item);
        Block_Graphics *as_arit = dynamic_cast<Block_Graphics *>(*item);
        End_Graphics *as_end = dynamic_cast<End_Graphics *>(*item);
        if (as_value) {
            QString name = as_value->get_name_m();
            Out_Port_Graphics *out = ((Out_Port_Graphics *)as_value->childItems().last());

            for(std::list<In_Port_Graphics *>::iterator iter = out->in_port_pointers.begin(); iter != out->in_port_pointers.end(); ++iter) {
                In_Port_Graphics *actual = *iter;
                QString port = actual->pos() == QPointF(-10,40) ? "1" : "2";
                QString name2;
                if (dynamic_cast<Block_Graphics *>(actual->parentItem())) {
                    name2 = (dynamic_cast<Block_Graphics *>(actual->parentItem()))->get_name_m();
                }
                else if (dynamic_cast<Value_Graphics *>(actual->parentItem())) {
                    name2 = (dynamic_cast<Value_Graphics *>(actual->parentItem()))->get_name_m();
                }
                else if (dynamic_cast<End_Graphics *>(actual->parentItem())) {
                    name2 = "END";
                }
                outStream << name << " OUT 1 " << name2 << " IN " << port << "\n";
            }
        }
        else if (as_arit) {
            QString name = as_arit->get_name_m();

            In_Port_Graphics *in1 = ((In_Port_Graphics *)as_arit->childItems().first());
            In_Port_Graphics *in2 = ((In_Port_Graphics *)as_arit->childItems().at(1));
            Out_Port_Graphics *out = ((Out_Port_Graphics *)as_arit->childItems().last());

            if (in1->out_port_pointer) {
            }

            if (in2->out_port_pointer) {
            }

            for(std::list<In_Port_Graphics *>::iterator iter = out->in_port_pointers.begin(); iter != out->in_port_pointers.end(); ++iter) {
                In_Port_Graphics *actual = *iter;
                QString port = actual->pos() == QPointF(-10,40) ? "1" : "2";
                QString name2;
                if (dynamic_cast<Block_Graphics *>(actual->parentItem())) {
                    name2 = (dynamic_cast<Block_Graphics *>(actual->parentItem()))->get_name_m();
                }
                else if (dynamic_cast<Value_Graphics *>(actual->parentItem())) {
                    name2 = (dynamic_cast<Value_Graphics *>(actual->parentItem()))->get_name_m();
                }
                else if (dynamic_cast<End_Graphics *>(actual->parentItem())) {
                    name2 = "END";
                }
                outStream << name << " OUT 1 " << name2 << " IN " << port << "\n";
            }
        }
    }


    outputFile.close();
}


void MainWindow::on_actionQuit_triggered()
{
    QMessageBox::StandardButton reply;

    reply = QMessageBox::question(this, "This file may contain changes, do you want to save them?", "Your changes will be lost if you quit without saving.",
        QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

    if (reply == QMessageBox::Save) {
        emit on_actionSave_File_triggered();
        close();
    }
    if (reply == QMessageBox::Discard)
    {
        close();
    }
    if(reply == QMessageBox::Cancel)
    {
        return;
    }
}

double MainWindow::go_through_tree(In_Port_Graphics *pointer_to_curr_in) {
    double value = 0;
        Out_Port_Graphics *out_port = pointer_to_curr_in->out_port_pointer;
        if (!out_port) {
            qDebug() << "Not connected properly";
            return 0;
        }
        Value_Graphics *as_value = dynamic_cast<Value_Graphics*>(out_port->parentItem());
        Block_Graphics *as_block = dynamic_cast<Block_Graphics*>(out_port->parentItem());
        if (as_value) {
            value = as_value->value_m.toDouble();
        }
        else if (as_block) {
            In_Port_Graphics *act_prev1 = dynamic_cast<In_Port_Graphics *>(as_block->childItems().at(0));
            In_Port_Graphics *act_prev2 = dynamic_cast<In_Port_Graphics *>(as_block->childItems().at(1));
            value = go_through_tree(act_prev1) + go_through_tree(act_prev2);;
        }
    return value;
}

void MainWindow::on_actionRun_program_triggered()
{

    In_Port_Graphics *end_data = (In_Port_Graphics *)scene_m->end->childItems().first();

    double result =  go_through_tree(end_data);
    qDebug() << result;
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    QList<QLineEdit *>list = this->findChildren<QLineEdit *>();
    QString value = list.first()->objectName() == "value" ? list.first()->text() : list.last()->text();
    //value += list.last()->objectName() == "type" ? list.last()->text() : list.first()->text();

    if (event->key() == Qt::Key_Return) {
        Canvas_Graphics *view = (Canvas_Graphics *)scene_m->items().last();

        QString name = "VALUE" + QString::number(view->get_block_id()["VALUE"]);
        view->block_id["VALUE"]++;

        block * added = new b_add_kg_to_kg(actual_project_m->get_type_lib());
        Value_Graphics * new_block = new Value_Graphics(view, added, name, value);
        actual_project_m->get_block_lib().push_back(added);
        new_block->show();
        new_block->setFlag(QGraphicsWidget::ItemIsMovable);
        new_block->setFlag(QGraphicsWidget::ItemIsSelectable);
    }
    QMainWindow::keyPressEvent(event);
}


void MainWindow::on_actionLoad_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Address Book"), "", tr("Block Editor (*.be2018);;All Files (*)"));
    if (fileName.isEmpty())
        return;

    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "error", file.errorString());
        return;
    }

    emit on_actionNew_File_triggered();

    QTextStream in(&file);
    loadMode mode = ADD_BLOCKS;
    std::map<QString, int> max_values = { };
    while(!in.atEnd()) {
        QString line = in.readLine();
        if (line == "# BLOCKS") {
            mode = ADD_BLOCKS;
        }
        else if (line == "# CONNECTIONS") {
            mode = ADD_CONNECTIONS;
        }
        else if (line == "# END_BLOCK") {
            mode = ADD_END;
        }
        else if (line == "# VALUES") {
            mode = ADD_VALUES;
        }
        else {
            if (mode == ADD_BLOCKS) {
                QStringList lineList = line.split(" ");
                QPoint pos = QPoint();
                Block_Graphics *new_block;
                if (lineList.count() != 3)
                    return;
                pos.setX(lineList.at(0).toInt());
                pos.setY(lineList.at(1).toInt());

                block * added = new b_add_kg_to_kg(actual_project_m->get_type_lib());
                new_block = new Block_Graphics((Canvas_Graphics *)scene_m->items().last() , added, lineList.at(2));
                actual_project_m->get_block_lib().push_back(added);
                new_block->setFlag(QGraphicsWidget::ItemIsMovable);
                new_block->setFlag(QGraphicsWidget::ItemIsSelectable);
                new_block->show();
                new_block->setPos(pos);

                // To ensure that each block has an unique ID after opening a saved file
                QString key = lineList.at(2).left(3);
                int value = lineList.at(2).mid(3).toInt();
                if (value >= max_values[key])
                    max_values[key] = value+1;
            }
            else if (mode == ADD_CONNECTIONS) {
                QStringList lineList = line.split(" ");
                if (lineList.count() != 6) {
                    qDebug() << "Parse error in load";
                    return;
                }
                QString name1 = lineList.at(0);
                QString type1 = lineList.at(1);
                int port1 = lineList.at(2).toInt();
                QString name2 = lineList.at(3);
                QString type2 = lineList.at(4);
                int port2 = lineList.at(5).toInt();

                QList<QGraphicsItem *> items_qlist = scene_m->items();
                items_qlist.removeLast();
                std::list<QGraphicsItem *>items = items_qlist.toStdList();
                items.reverse();
                In_Port_Graphics *inport;
                Out_Port_Graphics *outport;

                for (std::list<QGraphicsItem *>::iterator item = items.begin(); item != items.end(); ++item ) {
                    Block_Graphics *as_arit = dynamic_cast<Block_Graphics *>(*item);
                    End_Graphics *as_end = dynamic_cast<End_Graphics *>(*item);
                    Value_Graphics *as_value = dynamic_cast<Value_Graphics *>(*item);
                    if (as_arit) {
                            if (as_arit->get_name_m() == name1) {
                                if (type1 == "IN")
                                    inport = as_arit->in_ports_m.at(port1-1);
                                else
                                    outport = as_arit->out_ports_m.at(port1-1);
                            }
                            else if (as_arit->get_name_m() == name2){
                                if (type2 == "IN")
                                    inport = as_arit->in_ports_m.at(port2-1);
                                else
                                    outport = as_arit->out_ports_m.at(port2-1);
                            }
                    }
                    else if (as_end) {
                        if (as_end->name_m == name1)
                            inport = as_end->in_ports_m.at(port1-1);

                        else if (as_end->name_m == name2){
                            inport = as_end->in_ports_m.at(port2-1);
                        }
                    }
                    else if (as_value) {
                        if (as_value->get_name_m() == name1) {
                            outport = as_value->out_ports_m.at(port1-1);
                        }
                        else if (as_value->get_name_m() == name2){
                            outport = as_value->out_ports_m.at(port2-1);
                        }
                    }
                }
                inport->reference_m->attach(*(outport->get_reference()));
                Node_Graphics *node = new Node_Graphics(inport->scene(), inport->scenePos(), outport->scenePos());
                inport->attach(node);
                outport->attach(node);

                inport->out_port_pointer = outport;
                outport->in_port_pointers.push_back(inport);
            }

            else if (mode == ADD_END) {
                QStringList lineList = line.split(" ");
                QPoint pos = QPoint();
                if (lineList.count() != 2)
                    return;
                pos.setX(lineList.at(0).toInt());
                pos.setY(lineList.at(1).toInt());

                scene_m->end->setPos(pos);
            }
            else if (mode == ADD_VALUES) {
                QStringList lineList = line.split(" ");
                QPoint pos = QPoint();
                Value_Graphics *new_block;
                if (lineList.count() != 4)
                    return;
                pos.setX(lineList.at(0).toInt());
                pos.setY(lineList.at(1).toInt());

                block * added = new b_add_kg_to_kg(actual_project_m->get_type_lib());
                new_block = new Value_Graphics((Canvas_Graphics *)scene_m->items().last() , added, lineList.at(2), lineList.at(3));
                actual_project_m->get_block_lib().push_back(added);
                new_block->setFlag(QGraphicsWidget::ItemIsMovable);
                new_block->show();
                new_block->setPos(pos);

                // To ensure that each block has an unique ID after opening a saved file
                QString key = lineList.at(2).left(3);
                int value = lineList.at(2).mid(3).toInt();
                if (value >= max_values[key])
                    max_values[key] = value+1;
            }
            else {
                qDebug() << "Load file interesing item detected";
            }
        }
    }
    ((Canvas_Graphics*)(scene_m->items().last()))->setBlock_id(max_values);


    file.close();
}


// SCENE ------------------------------------
Scene_Graphics::Scene_Graphics(QObject *parent) : QGraphicsScene(parent) {

}

void Scene_Graphics::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    QGraphicsScene::mousePressEvent(event);
    if (event->button() == Qt::RightButton) {
        QGraphicsItem *item = itemAt(event->scenePos(), QTransform());
        if ((dynamic_cast<End_Graphics *>(item)) ) {
            return;
        }
        if ((dynamic_cast<End_Graphics *>(item->parentItem()))) {
            return;
        }
        if (!(dynamic_cast<In_Port_Graphics *>(item)) && !(dynamic_cast<Out_Port_Graphics *>(item)) && !(dynamic_cast<Block_Graphics *>(item)) && !(dynamic_cast<QGraphicsPathItem *>(item)) && !(dynamic_cast<Value_Graphics *>(item))) {
            return;
        }

        if (item) {
            if ((dynamic_cast<In_Port_Graphics *>(item)) || (dynamic_cast<Out_Port_Graphics *>(item))) {
                item = item->parentItem();
            }
            foreach(QGraphicsItem *tmp, item->childItems()) {
                tmp->~QGraphicsItem();
            }
            item->~QGraphicsItem();
        }
    }
}
