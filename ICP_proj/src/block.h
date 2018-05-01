#ifndef BLOCK_H
#define BLOCK_H

#include <vector>
#include "typemanager.h"
#include "block_ui.h"

typedef double value_t;   //value has always type double

class value_i   //interface for value
{
public:
    value_i() {};    //implicit constructor
    ~value_i() {};  //implicit destructor
    value_t value;
    type_id_t get_type();
protected:
    type_id_t type_m;
};

class out_port : public value_i
{
public:
    out_port(const type_id_t type);
};

class constant : public value_i
{
public:
    constant(const type_id_t type, value_t constant_val);
};

class in_port
{
public:
    in_port(const type_id_t type);
    value_t value();
    void attach(value_i & out);
private:
    type_id_t type_m;
    value_i* value_m;    //out_port or constatnt
};

class block
{
public:
    block() {instantiate(); call_next = nullptr;}
    ~block() {};
    void set_in_port(unsigned int index, value_i & val);
    void set_ui(Block_UI * new_ui);
    value_t get_out_port(unsigned int index);
    block * get_sequence_succ();
    size_t get_id();
    virtual void run();
protected:
    block* call_next;
    std::vector<in_port> in_ports_m;
    std::vector<out_port> out_ports_m;
private:
    size_t instance_id;
    Block_UI * ui;
    void instantiate();
};

#endif // BLOCK_H
