/*!
 * @file
 * @brief Blocz types are defined here
 *
 * @author Erik Kelemen, xkelem01@stud.fit.vutbr.cz
 */


#ifndef BLOCK_TEMPLATES_H
#define BLOCK_TEMPLATES_H 42

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
    }                                                                               \

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

#define MAC_BLOCK_MUL(name, type)                                                   \
    class name : public block                                                       \
    {                                                                               \
    public:                                                                         \
        name(type_mgr type_lib)                                                     \
        {                                                                           \
            in_ports_m.push_back(in_port(type_lib.get_type_id(type)));              \
            in_ports_m.push_back(in_port(type_lib.get_type_id("typeless")));        \
            out_ports_m.push_back(out_port(type_lib.get_type_id(type)));            \
        }                                                                           \
        void run()                                                                  \
        {                                                                           \
            out_ports_m[0].value = in_ports_m[0].value() * in_ports_m[1].value();   \
        }                                                                           \
    }

#define MAC_CONVERT(name, from, to, multiplier)                                     \
    class name : public block                                                       \
    {                                                                               \
    public:                                                                         \
        name(type_mgr type_lib)                                                     \
        {                                                                           \
            in_ports_m.push_back(in_port(type_lib.get_type_id(from)));              \
            out_ports_m.push_back(out_port(type_lib.get_type_id(to)));              \
        }                                                                           \
        void run()                                                                  \
        {                                                                           \
            out_ports_m[0].value = in_ports_m[0].value() * multiplier;              \
        }                                                                           \
    }

                                  \
    class b_output : public block
    {
    public:
        b_output(type_mgr type_lib)
        {
            in_ports_m.push_back(in_port(type_lib.get_type_id("typeless")));
        }
        void run()
        {
            std::cout << in_ports_m[0].value() << std::endl;
        }
    };

MAC_BLOCK_ADD_SAME(b_add_kg_to_kg, "kilogram");
MAC_BLOCK_ADD_SAME(b_add_g_to_g, "gram");
MAC_BLOCK_ADD_SAME(b_add_mg_to_mg, "miligram");
MAC_BLOCK_ADD_SAME(b_add_t_to_t, "ton");
MAC_BLOCK_ADD_SAME(b_add_lb_to_lb, "pound");

MAC_BLOCK_SUB_SAME(b_sub_kg_from_kg, "kilogram");
MAC_BLOCK_SUB_SAME(b_sub_g_from_g, "gram");
MAC_BLOCK_SUB_SAME(b_sub_mg_from_mg, "miligram");
MAC_BLOCK_SUB_SAME(b_sub_t_from_t, "ton");
MAC_BLOCK_SUB_SAME(b_sub_lb_from_lb, "pound");

MAC_BLOCK_MUL(b_mul_kg, "kilogram");
MAC_BLOCK_MUL(b_mul_g, "gram");
MAC_BLOCK_MUL(b_mul_mg, "miligram");
MAC_BLOCK_MUL(b_mul_t, "ton");
MAC_BLOCK_MUL(b_mul_lb, "pound");

MAC_CONVERT(b_conv_kg_to_g, "kilogram", "gram", 1000.0);
MAC_CONVERT(b_conv_kg_to_mg, "kilogram", "miligram", 1000000.0);
MAC_CONVERT(b_conv_kg_to_t, "kilogram", "ton", 0.001);
MAC_CONVERT(b_conv_kg_to_lb, "kilogram", "pound", 2.204623);

MAC_CONVERT(b_conv_g_to_kg, "gram", "kilogram", 0.001);
MAC_CONVERT(b_conv_mg_to_kg, "miligram", "kilogram", 0.000001);
MAC_CONVERT(b_conv_t_to_kg, "ton", "kilogram", 1000.0);
MAC_CONVERT(b_conv_lb_to_kg, "pound", "kilogram", 0.4535924);

MAC_CONVERT(b_conv_kg_to_nthg, "kilogram", "typeless", 1.0);
MAC_CONVERT(b_conv_g_to_nthg, "gram", "typeless", 1.0);
MAC_CONVERT(b_conv_mg_to_nthg, "miligram", "typeless", 1.0);
MAC_CONVERT(b_conv_t_to_nthg, "ton", "typeless", 1.0);
MAC_CONVERT(b_conv_lb_to_nthg, "pound", "typeless", 1.0);


#endif // BLOCK_TEMPLATES_H
