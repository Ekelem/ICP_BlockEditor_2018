#include <QApplication>
#include <QPushButton>
#include <QHBoxLayout>
#include <unistd.h>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QWidget mainWidget;
    new MainWindow(&mainWidget);

    return a.exec();
}
