#ifndef PROJECT_H
#define PROJECT_H

#include "typemanager.h"
#include "block.h"

#include <list>
#include <iostream>

class project
{
public:
    project();
    void run();
    void set_start(block * first);
    type_mgr & get_type_lib();
    std::list<block> & get_block_lib();
private:
    type_mgr type_lib;
    std::list<block> block_lib;
    block * start;

    bool check_cycles();
};

#endif // PROJECT_H
