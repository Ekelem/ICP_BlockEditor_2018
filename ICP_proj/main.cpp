#include "mainwindow.h"
#include "project.h"
#include "typemanager.h"
#include "block.h"
#include "block_templates.h"
#include <QApplication>
#include <iostream>
#include <list>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    std::list<project> opened_projects;

    project test_00 = project();
    opened_projects.push_back(test_00);   //create new project
    type_mgr & type_lib = test_00.get_type_lib();
    std::list<block> & block_lib = test_00.get_block_lib();

    //-----------------------------------------------------------

    //EXAMPLE EXECUTE ONE BLOCK

    type_lib.create_type("brambora"); //create type

    b_add_kg_to_kg block_test = b_add_kg_to_kg(type_lib);   //create block local
    block_lib.push_back(block_test);  //block in second project

    constant c_a = constant(type_lib.get_type_id("kilogram"), 5.0f);  //constant 5 kg
    constant c_b = constant(type_lib.get_type_id("kilogram"), 3.0f);  //constant 3 kg

    block_test.set_in_port(0, c_a);
    block_test.set_in_port(1, c_b);

    test_00.set_start(&block_test);
    test_00.run();

    std::cout << "out port 0 = " << block_test.get_out_port(0) << std::endl;    //should be 5 + 3 = 8

    //-----------------------------------------------------------------------------------------------



    return a.exec();
}
