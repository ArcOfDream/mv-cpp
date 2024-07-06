#include "sol/sol.hpp"

#pragma once

namespace mv {
void register_glm_types(sol::state&);
void register_resource_types(sol::state&);
void register_node_types(sol::state&);
}