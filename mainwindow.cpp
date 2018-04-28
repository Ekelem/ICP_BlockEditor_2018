#include "mainwindow.h"
#include <QtWidgets>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    p = parent;
    p->setFixedSize(1400, 800);
    main = new QHBoxLayout(p);

    QGridLayout *horizontalLayout = new QGridLayout();
    main->addLayout(horizontalLayout);
    MainTable = new MyArea(parent, this);
    MainTable->initialize();
    MainTable->setMinimumSize(QSize(1170, 750));
    horizontalLayout->addWidget(MainTable);

    box = new QVBoxLayout();
    box->setAlignment(Qt::AlignTop);
    main->addLayout(box);

    createObjects();
    connectSignals();

    parent->setWindowTitle(QObject::tr("Block Editor"));
    parent->show();
}

MainWindow::~MainWindow()
{

}

void MainWindow::create_block() {
    MainTable->create_block();
}

void MainWindow::createObjects() {
    createBlock = new QPushButton("Create Block");
    box->addWidget(createBlock);

    groupModeBox = new QGroupBox(tr("Select design mode"));
//    groupModeBox->setStyleSheet("QGroupBox{border:2px solid gray;border-radius:5px;margin-top: 1ex;} QGroupBox::title{subcontrol-origin: margin;subcontrol-position:top center;padding:0 3px;}");
    radio1 = new QRadioButton(tr("Move blocks"));
    radio2 = new QRadioButton(tr("Connect blocks"));

    radio1->setChecked(true);
    modeGroupVBox = new QVBoxLayout;
    modeGroupVBox->addWidget(radio1);
    modeGroupVBox->addWidget(radio2);
    modeGroupVBox->addStretch(1);
    modeGroupVBox->setMargin(2);
    groupModeBox->setLayout(modeGroupVBox);
    box->addWidget(groupModeBox);

    groupBlockBox = new QGroupBox(tr("Select block type"));
    radioADD = new QRadioButton(tr("ADD"));
    radioADD->setChecked(true);
    radioSUB = new QRadioButton(tr("SUB"));
    radioValue = new QRadioButton(tr("Value"));
    modeGroupVValue = new QVBoxLayout;
    modeGroupVValue->addWidget(radioADD);
    modeGroupVValue->addWidget(radioSUB);
    modeGroupVValue->addStretch(1);
    modeGroupVValue->setMargin(2);
    QHBoxLayout *valuecontent = new QHBoxLayout();
    modeGroupVValue->addLayout(valuecontent);
    valuecontent->addWidget(radioValue);
    edit = new QLineEdit();

    QRegExp re("^[+-]?(?=[.]?[0-9])[0-9]*(?:[.][0-9]*)?(?:[Ee][+-]?[0-9]+)?$"); // source: https://stackoverflow.com/questions/9853950/this-regular-expression-fail-to-parse-all-valid-floating-numbers
    QRegExpValidator *validator = new QRegExpValidator(re, this);
    edit->setValidator(validator);

    valuecontent->addWidget(edit);
    groupBlockBox->setLayout(modeGroupVValue);
    box->addWidget(groupBlockBox, 0, Qt::AlignBottom);

}

void MainWindow::connectSignals() {
    connect(createBlock, SIGNAL (released()), this, SLOT (create_block()));
    connect(radio1, SIGNAL( clicked(bool)), this, SLOT(setObjectsToMoveable()));
    connect(radio2, SIGNAL( clicked(bool)), this, SLOT(setObjectsToConnectable()));

    QSignalMapper *signalMapper = new QSignalMapper(this);
    connect(radioADD, SIGNAL(clicked(bool)), signalMapper, SLOT(map()));
    connect(radioSUB, SIGNAL(clicked(bool)), signalMapper, SLOT(map()));
    connect(radioValue, SIGNAL(clicked(bool)), signalMapper, SLOT(map()));
    signalMapper->setMapping(radioADD,B_ADD);
    signalMapper->setMapping(radioSUB, B_SUB);
    signalMapper->setMapping(radioValue, B_VALUE);

    connect (signalMapper, SIGNAL(mapped(int)), this, SLOT(setActualBlock(int)));
    connect(edit, SIGNAL(textChanged(const QString &)), this, SLOT(valueChanged()));
}

void MainWindow::setObjectsToMoveable(){
    MainTable->setObjectsToMoveable(true);
}

void MainWindow::setObjectsToConnectable(){
    MainTable->setObjectsToMoveable(false);
}

void MainWindow::setActualBlock(int type) {
    MainTable->setActualBlock(type);
}

void MainWindow::valueChanged() {
    MainTable->blockValue = edit->text().toDouble();
}
