#define SOL_ALL_SAFETIES_ON 1

#include "mv/objects/node.h"
#include "sol/sol.hpp"
#include <string>

namespace mv {

Node::Node(const char* _name) { name = _name; }

Node::Node(sol::this_state lua, const char* _name) : Node(_name) {
    this->lua = lua;

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

void Node::lua_dynamic_set(std::string key, sol::stack_object value) {
    auto it = entries.find(key);
    if (it == entries.cend()) {
        entries.insert(
                it, { std::move(key), std::move(value) });
    }
    else {
        std::pair<const std::string, sol::object>& kvp
                = *it;
        sol::object& entry = kvp.second;
        entry = sol::object(std::move(value));
    }
}

sol::object Node::lua_dynamic_get(std::string key) {
    auto it = entries.find(key);
    if (it == entries.cend()) {
        return sol::lua_nil;
    }
    return it->second;
}

void Node::_update(double dt) {
    for (auto &child : children) {
        child->_update(dt);
    }
    if(lua) {
        // printf("calling update on %s\n", name.c_str());
        lua_update.call<void>(this, dt);
        // sol::optional<sol::error> err = lua_update.call<sol::error>(this, dt);
        // if (err.has_value()) {
        //     printf("%s\n", err.value().what());
        // }
    }
    else update(dt);
}


} // namespace mv