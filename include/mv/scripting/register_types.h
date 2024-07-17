#pragma once

#include "wrenbind17/vm.hpp"
#include "wrenbind17/wrenbind17.hpp"
#include <glm/common.hpp>

namespace wren = wrenbind17;

namespace mv {
class WrenMath {};

// workaround for an odd error
template <typename T>
T vec_mix(T a, T b, float mod) { return glm::mix(a, b, mod); }

// void load_middleclass(sol::state&);
void register_math_types(wren::VM&);
void register_glm_types(wren::VM&);
void register_core_types(wren::VM&);
void register_resource_types(wren::VM&);
void register_node_types(wren::VM&);
}