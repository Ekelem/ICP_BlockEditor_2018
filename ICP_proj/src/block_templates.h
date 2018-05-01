/*!
 * @file
 * @brief Macro for creating different types
 *
 * @author Erik Kelemen, xkelem01@stud.fit.vutbr.cz
 */

#ifndef BLOCK_TEMPLATES_H
#define BLOCK_TEMPLATES_H

#include "block.h"

#include <iostream>

#define MAC_BLOCK_ADD_SAME(name, type)                                              \
    class name : public block                                                       \
    {                                                                               \
    public:                                                                         \
        name(type_mgr type_lib)                                                     \
        {                                                                           \
            in_ports_m.push_back(in_port(type_lib.get_type_id(type)));              \
            in_ports_m.push_back(in_port(type_lib.get_type_id(type)));              \
            out_ports_m.push_back(out_port(type_lib.get_type_id(type)));            \
        }                                                                           \
        void run()                                                                  \
        {                                                                           \
            out_ports_m[0].value = in_ports_m[0].value() + in_ports_m[1].value();   \
        }                                                                           \
    }

#define MAC_BLOCK_SUB_SAME(name, type)                                              \
    class name : public block                                                       \
    {                                                                               \
    public:                                                                         \
        name(type_mgr type_lib)                                                     \
        {                                                                           \
            in_ports_m.push_back(in_port(type_lib.get_type_id(type)));              \
            in_ports_m.push_back(in_port(type_lib.get_type_id(type)));              \
            out_ports_m.push_back(out_port(type_lib.get_type_id(type)));            \
        }                                                                           \
        void run()                                                                  \
        {                                                                           \
            out_ports_m[0].value = in_ports_m[0].value() - in_ports_m[1].value();   \
        }                                                                           \
    }

#define MAC_BLOCK_MUL_SAME(name, type)                                              \
    class name : public block                                                       \
    {                                                                               \
    public:                                                                         \
        name(type_mgr type_lib)                                                     \
        {                                                                           \
            in_ports_m.push_back(in_port(type_lib.get_type_id(type)));              \
            in_ports_m.push_back(in_port(type_lib.get_type_id(type)));              \
            out_ports_m.push_back(out_port(type_lib.get_type_id(type)));            \
        }                                                                           \
        void run()                                                                  \
        {                                                                           \
            out_ports_m[0].value = in_ports_m[0].value() * in_ports_m[1].value();   \
        }                                                                           \
    }


MAC_BLOCK_ADD_SAME(b_add_kg_to_kg, "kilogram");
MAC_BLOCK_ADD_SAME(b_add_g_to_g, "gram");
MAC_BLOCK_ADD_SAME(b_add_mg_to_mg, "miligram");
MAC_BLOCK_ADD_SAME(b_add_t_to_t, "ton");
MAC_BLOCK_ADD_SAME(b_add_lb_to_lb, "pound");

#endif // BLOCK_TEMPLATES_H
