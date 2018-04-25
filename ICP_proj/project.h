#ifndef PROJECT_H
#define PROJECT_H

#include "typemanager.h"
#include "block.h"

#include <list>

typedef struct project_settings
{
    type_mgr type_lib;
    std::list<block> block_lib;
}proj_settings;

extern proj_settings * editor_general;

class project
{
public:
    project();
    void make_actual();
private:
    proj_settings main_parts;
};

#endif // PROJECT_H
