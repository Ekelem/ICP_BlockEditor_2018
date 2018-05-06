#ifndef PROJECT_H
#define PROJECT_H

#include "typemanager.h"
#include "block.h"
#include "block_templates.h"
#include "exceptions.h"

#include <list>
#include <iostream>
#include <map>

#define MAC_ADD_TO_PALETTE(name, func) block_palette.insert(std::pair<std::string, block * (*)(type_mgr)>(name, [](type_mgr type_lib) -> block * {return new func(type_lib);}))

class project
{
public:
    project();
    ~project();
    void setup_palette();
    block * add_block(std::string name);
    void run();
    void set_start(block * first);
    block ** get_start_addr();
    type_mgr & get_type_lib();
    std::list<block *> &get_block_lib();
private:
    type_mgr type_lib;
    std::list<block*> block_lib;
    std::map<std::string, block * (*)(type_mgr)> block_palette;
    block * start;

    bool check_cycles();
    bool check_inputs();
};

#endif // PROJECT_H
