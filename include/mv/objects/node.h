#include "SDL_events.h"
#include <list>
#include <memory>
#include <string>

#pragma once

namespace mv {
class Node {
  public:
    std::string name;

    // scene graph type of thing
    std::list<std::shared_ptr<Node>> children;
    Node *parent = nullptr;

    Node(std::string);

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
  std::shared_ptr<T> add_instanced_child(std::shared_ptr<T> what) {
    static_assert(std::is_base_of<Node, T>::value, "T must derive from Node");

    what->parent = this;
    children.emplace_back(what);
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
};
} // namespace mv
