/*!
 * @file
 * @brief Contains definitions of class project which maintains the backend part
 *
 * @author Erik Kelemen, xkelem01@stud.fit.vutbr.cz
 *
 */


#include "project.h"

project::project()
{
    start = nullptr;
    type_lib = type_mgr();
}

type_mgr & project::get_type_lib()
{
    return type_lib;
}

std::list<block*> & project::get_block_lib()
{
    return block_lib;
}

void project::run()
{
    if (check_cycles())
        throw 3;

    block * next;
    next = start;
    while (next != nullptr)
    {
        next->run();
        next = next->get_sequence_succ();
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
            std::cerr << *i << std::endl;
            if (*i == next->get_id())
                return true;
        }
        ids.push_back(next->get_id());
        next = next->get_sequence_succ();
    }
    return false;
}

void project::set_start(block * first)
{
    start = first;
}
