#include "project.h"

project::project()
{
    start = nullptr;
    type_lib = type_mgr();
    setup_palette();
}

project::~project()
{
    /*for (auto it = block_lib.begin(); it != block_lib.end(); it++)
    {
        delete *it;
    }*/
}

void project::setup_palette()
{
    auto ptr = [](type_mgr type_lib) -> block * {return new b_add_kg_to_kg(type_lib);};
    block_palette.insert(std::pair<std::string, block * (*)(type_mgr)>("b_add_kg_to_kg", ptr));
    block_palette.insert(std::pair<std::string, block * (*)(type_mgr)>("b_add_g_to_g", [](type_mgr type_lib) -> block * {return new b_add_g_to_g(type_lib);}));
    block_palette.insert(std::pair<std::string, block * (*)(type_mgr)>("b_add_mg_to_mg", [](type_mgr type_lib) -> block * {return new b_add_mg_to_mg(type_lib);}));
}

block *project::add_block(std::string name)
{
    if ( block_palette.find(name) == block_palette.end() )
    {
        std::cerr << "not found " << name << std::endl;
    }
    std::cerr << "found " << name << std::endl;
    return block_palette.at(name)(type_lib);
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
