#include "SDL_events.h"
#include "sol/sol.hpp"
#include <list>
#include <lua.h>
#include <memory>
#include <string>

#pragma once

namespace mv {
class Node {
  public:
    std::string name;

    // Lua related stuff
    lua_State *lua = nullptr;
    std::unordered_map<std::string, sol::object> entries;
    void lua_dynamic_set(std::string key, sol::stack_object value);
    sol::object lua_dynamic_get(std::string key);

    // scene graph type of thing
    std::list<std::shared_ptr<Node>> children;
    Node *parent = nullptr;

    Node(const char*);
    Node(sol::this_state, const char*);

  template <typename T, typename... Targs>
  T* add_child(const Targs &...args) {
    static_assert(std::is_base_of<Node, T>::value, "T must derive from Node");

    auto c = std::make_shared<T>(args...);
    T* ptr = c.get();
    c->parent = this;
    children.emplace_back(c);
    return ptr;
  }

  template <typename T>
  T* add_instanced_child(T *what) {
    static_assert(std::is_base_of<Node, T>::value, "T must derive from Node");

    what->parent = this;
    children.emplace_back(std::shared_ptr<T>(what));
    return what;
  }

    virtual void _init() {};
    virtual void _update(double);
    virtual void _draw() {};
    virtual void _input(SDL_Event&) {};

    virtual void init() {};
    virtual void update(double) {};
    virtual void draw() {};
    virtual void input(SDL_Event&) {};

    sol::function lua_init;
    sol::function lua_update;
    sol::function lua_draw;
    sol::function lua_input;
};
} // namespace mv