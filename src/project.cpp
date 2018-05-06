/*!
 * @file
 * @brief Project class merges backend with frontend
 *
 * @author Erik Kelemen, xkelem01@stud.fit.vutbr.cz
 */

#include "project.h"

project::project()
{
    start = nullptr;
    type_lib = type_mgr();
    setup_palette();
    running_m = false;
    break_block = nullptr;
}

project::~project()
{
    for (auto it = block_lib.begin(); it != block_lib.end(); it++)
    {
        delete *it;
    }
}

void project::setup_palette()
{
    block_palette.insert(std::pair<std::string, block * (*)(type_mgr)>("b_add_kg_to_kg", [](type_mgr type_lib) -> block * {return new b_add_kg_to_kg(type_lib);}));
    block_palette.insert(std::pair<std::string, block * (*)(type_mgr)>("b_add_g_to_g", [](type_mgr type_lib) -> block * {return new b_add_g_to_g(type_lib);}));
    block_palette.insert(std::pair<std::string, block * (*)(type_mgr)>("b_add_mg_to_mg", [](type_mgr type_lib) -> block * {return new b_add_mg_to_mg(type_lib);}));
    MAC_ADD_TO_PALETTE("b_add_t_to_t", b_add_t_to_t);
    MAC_ADD_TO_PALETTE("b_add_lb_to_lb", b_add_lb_to_lb);

    MAC_ADD_TO_PALETTE("b_sub_kg_from_kg", b_sub_kg_from_kg);
    MAC_ADD_TO_PALETTE("b_sub_g_from_g", b_sub_g_from_g);
    MAC_ADD_TO_PALETTE("b_sub_mg_from_mg", b_sub_mg_from_mg);
    MAC_ADD_TO_PALETTE("b_sub_t_from_t", b_sub_t_from_t);
    MAC_ADD_TO_PALETTE("b_sub_lb_from_lb", b_sub_lb_from_lb);

    MAC_ADD_TO_PALETTE("b_mul_kg", b_mul_kg);
    MAC_ADD_TO_PALETTE("b_mul_g", b_mul_g);
    MAC_ADD_TO_PALETTE("b_mul_mg", b_mul_mg);
    MAC_ADD_TO_PALETTE("b_mul_t", b_mul_t);
    MAC_ADD_TO_PALETTE("b_mul_lb", b_mul_lb);

    MAC_ADD_TO_PALETTE("b_conv_kg_to_g", b_conv_kg_to_g);
    MAC_ADD_TO_PALETTE("b_conv_kg_to_mg", b_conv_kg_to_mg);
    MAC_ADD_TO_PALETTE("b_conv_kg_to_t", b_conv_kg_to_t);
    MAC_ADD_TO_PALETTE("b_conv_kg_to_lb", b_conv_kg_to_lb);

    MAC_ADD_TO_PALETTE("b_conv_g_to_kg", b_conv_g_to_kg);
    MAC_ADD_TO_PALETTE("b_conv_mg_to_kg", b_conv_mg_to_kg);
    MAC_ADD_TO_PALETTE("b_conv_t_to_kg", b_conv_t_to_kg);
    MAC_ADD_TO_PALETTE("b_conv_lb_to_kg", b_conv_lb_to_kg);

    MAC_ADD_TO_PALETTE("b_conv_kg_to_nthg", b_conv_kg_to_nthg);
    MAC_ADD_TO_PALETTE("b_conv_g_to_nthg", b_conv_g_to_nthg);
    MAC_ADD_TO_PALETTE("b_conv_mg_to_nthg", b_conv_mg_to_nthg);
    MAC_ADD_TO_PALETTE("b_conv_t_to_nthg", b_conv_t_to_nthg);
    MAC_ADD_TO_PALETTE("b_conv_lb_to_nthg", b_conv_lb_to_nthg);

    MAC_ADD_TO_PALETTE("b_output", b_output);


}

block *project::add_block(std::string name)
{
    if ( block_palette.find(name) == block_palette.end() )
    {
        throw exceptions_enum::block_not_exists;
    }
    return block_palette.at(name)(type_lib);
}

void project::force_stop()
{
    running_m = false;
}

type_mgr & project::get_type_lib()
{
    return type_lib;
}

std::list<block*> & project::get_block_lib()
{
    return block_lib;
}

void project::reset()
{
    for (auto itr = block_lib.begin(); itr != block_lib.end(); itr++)
    {
        for (unsigned int i = 0; i < (*itr)->get_out_size(); i++)
            (*itr)->get_out_port(i)->reset();
    }
}

void project::run()
{
    block * next;
    if (!running_m)
    {
        next = start;
        running_m = true;
        reset();
        if (check_cycles())
            throw exceptions_enum::cycle;
        if (check_inputs())
            throw exceptions_enum::unconnected_in;
    }
    else
    {
        next = break_block;
    }

    while (next != nullptr)
    {
        next->run();
        next = next->get_sequence_succ();
    }
    running_m = false;
}

void project::step()
{
    if (running_m)
    {
        block * next = break_block;
        if (next != nullptr)
        {
            next->run();
            break_block = next->get_sequence_succ();
        }
        else
            running_m = false;
    }
    else
    {
        block * next = start;
        if (next != nullptr)
        {
            running_m = true;
            next->run();
            break_block = next->get_sequence_succ();
        }
        else
            running_m = false;
    }
}

bool project::check_cycles()
{
    std::vector<size_t> ids;

    block * next;
    next = start;
    while (next != nullptr)
    {
        for (auto i = ids.begin(); i != ids.end(); i++)
        {
            if (*i == next->get_id())
                return true;
        }
        ids.push_back(next->get_id());
        next = next->get_sequence_succ();
    }
    return false;
}

bool project::check_inputs()
{
    block * next = start;
    while (next != nullptr)
    {
        for (unsigned int i = 0; i < next->get_in_size(); i++)
        {
            if (next->get_in_port(i)->is_free())
                return true;
        }
        next = next->get_sequence_succ();
    }
    return false;
}

void project::set_start(block * first)
{
    start = first;
}

block **project::get_start_addr()
{
    return &start;
}
