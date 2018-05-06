/*!
 * @file
 * @brief This file contains the backend part of each block
 *
 * @author Erik Kelemen, xkelem01@stud.fit.vutbr.cz
 */

#include "block.h"

void block::run() {}

type_id_t value_i::get_type()
{
    return type_m;
}

void value_i::reset()
{
    value = 0.0;
}

out_port::out_port(const type_id_t type)
{
    value = 0.0;
    type_m = type;
}

constant::constant(const type_id_t type, value_t constant_val)
{
    value = constant_val;
    type_m = type;
}

in_port::in_port(const type_id_t type)
{
    type_m = type;
    value_m = nullptr;
}

value_t in_port::value()
{
    //TODO: throw ex
    return value_m->value;
}

type_id_t in_port::get_type()
{
    return type_m;
}

bool in_port::is_free()
{
    return value_m == nullptr ? true : false;
}

void in_port::attach(value_i & out)
{
    if (out.get_type() == type_m)
        ;   //TODO throw ex

    value_m = &out;
}

void block::set_in_port(unsigned int index, value_i & val)
{
    if (index > in_ports_m.size())
        ;   //TODO: throw ex
    in_ports_m[index].attach(val);
}

out_port *block::get_out_port(unsigned int index)
{
    //TODO throw
    return &out_ports_m.at(index);
}

in_port *block::get_in_port(unsigned int index)
{
    //TODO throw
    return &in_ports_m.at(index);
}

unsigned int block::get_out_size()
{
    return out_ports_m.size();
}

unsigned int block::get_in_size()
{
    return in_ports_m.size();
}

unsigned int block::get_max_size()
{
    return (get_in_size() > get_out_size()) ? get_in_size() : get_out_size();
}

value_t block::get_out_port_value(unsigned int index)
{
    if (index > out_ports_m.size())
        ;   //TODO: throw ex

    return out_ports_m[index].value;
}

block * block::get_sequence_succ()
{
    return call_next;
}

block **block::get_ptr_sequence_succ()
{
    return &call_next;
}

size_t block::get_id()
{
    return instance_id;
}

void block::instantiate()
{
    static size_t id = 0;
    size_t old = id;
    instance_id = id++;
    if (instance_id < old)
        throw 1;    //overflow
}
