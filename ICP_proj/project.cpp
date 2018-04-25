#include "project.h"

project::project()
{
    main_parts.type_lib = type_mgr();
    make_actual();
}

void project::make_actual()
{
    editor_general = &main_parts;
}
