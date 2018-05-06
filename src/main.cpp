/*!
 * @file
 * @brief Basic class of a bunch of blokes-frontend
 *
 * @author Erik Kelemen, xkelem01@stud.fit.vutbr.cz
 * @author Attila Lakatos, xlakat01@stud.fit.vutbr.cz
 */

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
    return a.exec();
}



/*! \mainpage
 *
 * # Introduction
 * Block Editor is a graphical drag and drop code editor, where you can snap together blocks of code.
 *
 * Funcionality:
 *  * Fundemantal requirements:
 *      * With this application you are able to create/save/load blocks into/from block editor
 *      * Each block has a unique indentifier
 *      * Once you save a block schema you are able to open it
 *      * Every single schema can contain a bunch of blocks and connections between them
 *  * Blocks:
 *      * Each block has input and output ports
 *      * Each port represents a type such like name=>value(In our case value will always be double)
 *      * Connections can be establilished between in and output ports
 *      * If a certain block is executed the output is computed
 *  * Connection between blocks:
 *      * Application checks the type compatibility between blocks
 *      * Type is assigned according to the inputs
 *  * Calculation:
 *      * If the blocks are connected you can run the program
 *      * Block Editor supports cycles
 *      * If blocks are syntactically correct you can run the program
 *      * You can use step-by-step
 *
 *  * Additional upgrades:
 *      * The value appears while hovering over a connection between two blocks
 *      * Application contains various blocks which have not been specified in the specification
 *
 * ## This blockeditor also supports...
 * * Two or more programs inside the editor
 * * Remove connections and/or blocks without the need of creating new file
 *
 *
 * ## Installation
 *
 * `````
 * git clone https://github.com/Ekelem/ICP_BlockEditor_2018.git
 * sudo apt-get install gcc
 * sudo apt-get install qtcreator
 * sudo apt-get install doxygen
 * cd ICP_BlockEditor_2018
 * make
 * `````
 * Note that this repository will be public after 15.6.2018
 *
 * ## Run
 *
 * `````
 * ./blockeditor
 * `````
 *
 * ## Implementation
 *
 * The whole project can be divided into 2 parts(frontend/backend), so at the beginning we have created two branches, one for each part.
 * Since we did not have much experience with QtCreator, this was the hardest part of the project. The official Qt documentation provided us some information about the basic usage, it also contains examples which we were using in this project.
 *
 *
 *
 *
 *
 * ### References
 * * [Qt official documentation](http://doc.qt.io/qt-5/qtexamplesandtutorials.html)
 * * [Qt Drag and Drop event](http://doc.qt.io/qt-5/qtwidgets-draganddrop-draggableicons-example.html)
 * * [Stack overflow](https://stackoverflow.com/)
 */
