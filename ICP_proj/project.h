#ifndef PROJECT_H
#define PROJECT_H

#include "typemanager.h"
#include "block.h"
#include "block_templates.h"

#include <list>
#include <iostream>
#include <map>

class project
{
public:
    project();
    ~project();
    void setup_palette();
    block * add_block(std::string name);
    void run();
    void set_start(block * first);
    type_mgr & get_type_lib();
    std::list<block *> &get_block_lib();
private:
    type_mgr type_lib;
    std::list<block*> block_lib;
    std::map<std::string, block * (*)(type_mgr)> block_palette;
    block * start;

    bool check_cycles();
};

#endif // PROJECT_H
