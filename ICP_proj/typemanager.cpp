#include "typemanager.h"

#include <iostream>



type_mgr::type_mgr()
{
    default_types();
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

unsigned int type_mgr::get_type_count()
{
    return type_lib.size();
}

std::string type_mgr::get_type_name(unsigned int index)
{
    return type_lib.at(index);
}

bool type_mgr::type_already_exists(std::string type_name)
{
    for (auto i = type_lib.begin(); i != type_lib.end(); i++)
        if (*i == type_name)
            return true;

    return false;
}

void type_mgr::default_types()
{
    create_type("typeless");
    //weight
    create_type("kilogram");
    create_type("gram");
    create_type("miligram");
    create_type("ton");
    create_type("pound");

    //length
    create_type("meter");
    create_type("decimeter");
    create_type("centimeter");
    create_type("milimeter");
    create_type("kilometer");
    create_type("inch");
    create_type("foot");
    create_type("yard");
    create_type("mile");

    //liquid
    create_type("liter");
    create_type("deciliter");
    create_type("gallon");

    //time
    create_type("minute");
    create_type("second");
    create_type("hour");
    create_type("day");
    create_type("week");
    create_type("year");

    //debug();
}

void type_mgr::debug()
{
    for (auto i = type_lib.begin(); i!= type_lib.end(); i++)
    {
        std::cerr << *i << std::endl;
    }
}
