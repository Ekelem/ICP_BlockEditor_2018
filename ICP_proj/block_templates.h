#ifndef BLOCK_TEMPLATES_H
#define BLOCK_TEMPLATES_H

#include "block.h"

#include <iostream>

extern proj_settings * editor_general;

class b_add_kg_to_kg : public block
{
public:
    b_add_kg_to_kg()
    {
        in_ports_m.push_back(in_port(editor_general->type_lib.get_type_id("kg")));  //first argument
        in_ports_m.push_back(in_port(editor_general->type_lib.get_type_id("kg")));  //second argument
        out_ports_m.push_back(out_port(editor_general->type_lib.get_type_id("kg")));    //out port
    }

    void run()
    {
        out_ports_m[0].value = in_ports_m[0].value() + in_ports_m[1].value();
    }
};

#endif // BLOCK_TEMPLATES_H
