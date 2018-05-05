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

    block *ptr_start = new b_add_kg_to_kg(actual_project_m->get_type_lib());
    scene_m->start = new Start_Graphics(nullptr, ptr_start);
    scene_m->addItem(scene_m->start);

    block *ptr_end   = new b_add_kg_to_kg(actual_project_m->get_type_lib());
    scene_m->end = new End_Graphics(nullptr, ptr_end);
    scene_m->addItem(scene_m->end);


    //block * ptr = new b_add_kg_to_kg(actual_project_m->get_type_lib());
    //scene_m->addWidget(new Block_UI(nullptr, ptr, "abc"));
//    QPainterPath path = QPainterPath();
//    path.cubicTo(20, 0, 30, 50, 50, 50);
//    QGraphicsPathItem * ptr = scene_m->addPath(path, QPen(Qt::green, 3, Qt::SolidLine), QBrush(Qt::black));
    //ptr->setPath();


    //ui->main_field->mark_project(actual_project_m);
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
    block *ptr_start = new b_add_kg_to_kg(actual_project_m->get_type_lib());
    scene_m->start = new Start_Graphics(nullptr, ptr_start);
    scene_m->addItem(scene_m->start);

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


    for (std::list<QGraphicsItem *>::iterator item = items.begin(); item != items.end(); ++item ) {
        Start_Graphics *as_start = dynamic_cast<Start_Graphics *>(*item);
        End_Graphics *as_end = dynamic_cast<End_Graphics *>(*item);
        if (as_start) {
            QPointF pos = (*item)->pos();
            qDebug() << pos;
            outStream << "# START_BLOCK\n" << pos.x() << " " << pos.y() << "\n";
        }
        else if (as_end) {
            QPointF pos = (*item)->pos();
            outStream << "# END_BLOCK\n" << pos.x() << " " << pos.y() << "\n";
        }
    }

    outStream << "# CONNECTIONS\n";
    for (std::list<QGraphicsItem *>::iterator item = items.begin(); item != items.end(); ++item ) {
        QGraphicsPathItem *as_connection = dynamic_cast<QGraphicsPathItem *>(*item);
        if (as_connection) {
            for (int i = 0; i < as_connection->path().elementCount(); i++) {
                outStream << as_connection->path().elementAt(i).x << " " << as_connection->path().elementAt(i).y << ((i == as_connection->path().elementCount()-1) ? "" : " ");
            }
            outStream << "\n";
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

bool MainWindow::isAtTheBeginning(In_Port_Graphics *pointer_to_curr_in, std::list<In_Port_Graphics *>first_items) {
    for(std::list<In_Port_Graphics *>::iterator it = first_items.begin(); it != first_items.end(); ++it) {
        if (*it == pointer_to_curr_in)
            return true;
    }
    return false;
}

double MainWindow::go_through_tree(In_Port_Graphics *pointer_to_curr_in, std::list<In_Port_Graphics *>first_items) {
    double value = 0;
    if (isAtTheBeginning(pointer_to_curr_in, first_items)) {
//        qDebug() << "start reached";
        value =  (dynamic_cast<Value_Graphics*>(pointer_to_curr_in->parentItem()))->value_m.toDouble();

    }
    else {
        Out_Port_Graphics *out_port = pointer_to_curr_in->out_port_pointer;
        if (!out_port) {
            qDebug() << "Not connected properly";
            return 0;
        }

        Value_Graphics *as_value = dynamic_cast<Value_Graphics*>(out_port->parentItem());
        Block_Graphics *as_block = dynamic_cast<Block_Graphics*>(out_port->parentItem());
        if (as_value) {
//            qDebug() << "Value detected";
            In_Port_Graphics *act_prev = (In_Port_Graphics *)as_value->childItems().first();
            value = go_through_tree(act_prev, first_items);
        }
        else if (as_block) {
//            qDebug() << "Block detected";
            In_Port_Graphics *act_prev1 = (In_Port_Graphics *)as_block->childItems().at(0);
            In_Port_Graphics *act_prev2 = (In_Port_Graphics *)as_block->childItems().at(1);
            value = go_through_tree(act_prev1, first_items) + go_through_tree(act_prev2, first_items);;
        }
        else {
//            qDebug() << "nothing";
        }
    }

    return value;
}

void MainWindow::on_actionRun_program_triggered()
{

    In_Port_Graphics *end_data = (In_Port_Graphics *)scene_m->end->childItems().first();
    std::list<In_Port_Graphics *>first_items = ((Out_Port_Graphics *)scene_m->start->childItems().first())->in_port_pointers;

    double result =  go_through_tree(end_data, first_items);
    qDebug() << result;


//    In_Port_Graphics *tmp_in = ((Out_Port_Graphics *)scene_m->start->childItems().first())->in_port_pointer;
//    In_Port_Graphics *end_data = (In_Port_Graphics *)scene_m->end->childItems().first();
//    qDebug() << ((Out_Port_Graphics *)scene_m->start->childItems().first())->in_port_pointers


//    qDebug() << "run";
//    while(tmp_in !=  end_data) { // end->pointer

//        Block_Graphics *block = dynamic_cast<Block_Graphics*>(tmp_in->parentItem());
//        Value_Graphics *value = dynamic_cast<Value_Graphics*>(tmp_in->parentItem());
//        End_Graphics *end = dynamic_cast<End_Graphics*>(tmp_in->parentItem());

//        if (block) {
//            QGraphicsItem *tmpItem = block->childItems().last();
//            Out_Port_Graphics *tmp_out = dynamic_cast<Out_Port_Graphics *>(tmpItem);
//            tmp_in = tmp_out->in_port_pointer;
//            qDebug() << block->get_name_m();
//        }
//        else if(value) {
//            QGraphicsItem *tmpItem = value->childItems().last();
//            Out_Port_Graphics *tmp_out = dynamic_cast<Out_Port_Graphics *>(tmpItem);
//            tmp_in = tmp_out->in_port_pointer;
//            qDebug() << value->get_name_m();
//        }
//        else if (end) {
//            QGraphicsItem *tmpItem = end->childItems().last();
//            Out_Port_Graphics *tmp_out = dynamic_cast<Out_Port_Graphics *>(tmpItem);
//            tmp_in = tmp_out->in_port_pointer;
//            qDebug() << "END";
//        }
//        if (tmp_in == nullptr) {
//            qDebug() << "NOOOt conenetion";
//            break;
//        }

//    }
//    qDebug() << "END";

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
    }
    QMainWindow::keyPressEvent(event);
}











// SCENE ------------------------------------
Scene_Graphics::Scene_Graphics(QObject *parent) : QGraphicsScene(parent) {

}

void Scene_Graphics::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    QGraphicsScene::mousePressEvent(event);
    if (event->button() == Qt::RightButton) {
        QGraphicsItem *item = itemAt(event->scenePos(), QTransform());
        if ((dynamic_cast<Start_Graphics *>(item)) || (dynamic_cast<End_Graphics *>(item)) ) {
            return;
        }
        if ((dynamic_cast<Start_Graphics *>(item->parentItem())) || (dynamic_cast<End_Graphics *>(item->parentItem()))) {
            return;
        }
        if (!(dynamic_cast<In_Port_Graphics *>(item)) && !(dynamic_cast<Out_Port_Graphics *>(item)) && !(dynamic_cast<Block_Graphics *>(item)) && !(dynamic_cast<QGraphicsPathItem *>(item))) {
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


void MainWindow::on_actionLoad_triggered()
{
    qDebug() <<" ASD";
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
        else if (line == "# START_BLOCK") {
            mode = ADD_START;
        }
        else if (line == "# END_BLOCK") {
            mode = ADD_END;
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
                if (lineList.count() != 8) {
                    qDebug() << "Parse error in load";
                    return;
                }
                QPoint point1 = QPoint(lineList.at(0).toInt(), lineList.at(1).toInt());
                QPoint point2 = QPoint(lineList.at(2).toInt(), lineList.at(3).toInt());
                QPoint point3 = QPoint(lineList.at(4).toInt(), lineList.at(5).toInt());
                QPoint point4 = QPoint(lineList.at(6).toInt(), lineList.at(7).toInt());

                QPainterPath path = QPainterPath();
                path.moveTo(point1);
                path.cubicTo(point2.x(), point2.y(), point3.x(), point3.y(), point4.x(), point4.y());
                scene_m->addPath(path, QPen(Qt::green, 3, Qt::SolidLine));
            }
            else if (mode == ADD_START) {
                QStringList lineList = line.split(" ");
                QPoint pos = QPoint();
                if (lineList.count() != 2)
                    return;
                pos.setX(lineList.at(0).toInt());
                pos.setY(lineList.at(1).toInt());

                scene_m->start->setPos(pos);
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
            else {
                qDebug() << "Load file interesing item detected";
            }
        }
    }
    ((Canvas_Graphics*)(scene_m->items().last()))->setBlock_id(max_values);


    file.close();
}
