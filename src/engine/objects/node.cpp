#include "mv/objects/node.h"
#include "sol/sol.hpp"
#include <string>

namespace mv {

Node::Node(const char* _name) { name = _name; }

Node::Node(sol::this_state lua, const char* _name) : Node(_name) {
    lua_init = sol::make_reference<sol::function>(
        lua.lua_state(), &Node::init
    );

    lua_update = sol::make_reference<sol::function>(
        lua.lua_state(), &Node::update
    );

    lua_draw = sol::make_reference<sol::function>(
        lua.lua_state(), &Node::draw
    );

    lua_input = sol::make_reference<sol::function>(
        lua.lua_state(), &Node::input
    );

}

} // namespace mv