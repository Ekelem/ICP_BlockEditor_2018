#include "mainwindow.h"
#include "project.h"
#include "typemanager.h"
#include "block.h"
#include "block_templates.h"
#include <QApplication>
#include <iostream>
#include <list>

proj_settings * editor_general;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    std::list<project> opened_projects;

    //EXAMPLE CREATE TWO EMPTY PROJECTS

    opened_projects.push_back(project());   //create new project
    opened_projects.push_back(project());   //create new project

    //-----------------------------------------------------------

    //EXAMPLE EXECUTE ONE BLOCK

    editor_general->type_lib.create_type("kg"); //create type in second project
    editor_general->type_lib.create_type("brambora"); //create type in second project

    b_add_kg_to_kg block_test = b_add_kg_to_kg();   //create block local
    editor_general->block_lib.push_back(block_test);  //block in second project

    constant c_a = constant(editor_general->type_lib.get_type_id("kg"), 5.0f);  //constant 5 kg
    constant c_b = constant(editor_general->type_lib.get_type_id("kg"), 3.0f);  //constant 3 kg

    block_test.set_in_port(0, c_a);
    block_test.set_in_port(1, c_b);

    block_test.run();

    std::cout << "out port 0 = " << block_test.get_out_port(0) << std::endl;    //should be 5 + 3 = 8

    //-----------------------------------------------------------------------------------------------



    return a.exec();
}
