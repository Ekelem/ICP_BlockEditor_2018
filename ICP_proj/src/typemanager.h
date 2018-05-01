/*!
 * @file
 * @brief Basic block type values
 *
 * @author Erik Kelemen, xkelem01@stud.fit.vutbr.cz
 *
 */

#ifndef TYPEMANAGER_H
#define TYPEMANAGER_H

#include <vector>
#include <string>

typedef uint32_t type_id_t;

class type_mgr
{
public:
    type_mgr();
    void create_type(std::string type_name);
    void insert_type(std::string type_name, type_id_t id);
    type_id_t get_type_id(std::string type_name);

    void debug();

private:
    bool type_already_exists(std::string type_name);
    void default_types();
    std::vector<std::string> type_lib;
};

#endif // TYPEMANAGER_H
