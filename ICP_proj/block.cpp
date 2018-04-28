#include "block.h"

void block::run() {}

type_id_t value_i::get_type()
{
    return type_m;
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

value_t block::get_out_port(unsigned int index)
{
    if (index > out_ports_m.size())
        ;   //TODO: throw ex

    return out_ports_m[index].value;
}

block * block::get_sequence_succ()
{
    return call_next;
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
