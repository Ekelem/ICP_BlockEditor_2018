#include "typemanager.h"



type_mgr::type_mgr()
{

}

void type_mgr::create_type(std::string type_name)
{
    //TODO
    if (!type_already_exists(type_name))
        type_lib.push_back(type_name);
    else
        throw 25;
}

void type_mgr::insert_type(std::string type_name, type_id_t id)
{
    if (!type_already_exists(type_name))
        type_lib.insert(type_lib.begin() + id, type_name);
}

type_id_t type_mgr::get_type_id(std::string type_name)
{
    for (auto i = type_lib.begin(); i != type_lib.end(); i++)
        if (*i == type_name)
            return i - type_lib.begin();

    throw 25;
}

bool type_mgr::type_already_exists(std::string type_name)
{
    for (auto i = type_lib.begin(); i != type_lib.end(); i++)
        if (*i == type_name)
            return true;

    return false;
}
