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
    std::list<std::unique_ptr<Node>> children;
    Node *parent = nullptr;

    Node(const char*);

  template <typename T, typename... Targs>
  T* add_child(const Targs &...args) {
    static_assert(std::is_base_of<Node, T>::value, "T must derive from Node");

    auto c = std::make_unique<T>(args...);
    T* ptr = c.get();
    c->parent = this;
    children.emplace_back(std::move(c));
    return ptr;
  }

    virtual void _init() = 0;
    virtual void _update(double) = 0;
    virtual void _draw() = 0;
    virtual void _input(SDL_Event&) = 0;

    virtual void init() = 0;
    virtual void update(double) = 0;
    virtual void draw() = 0;
    virtual void input(SDL_Event&) = 0;
};
} // namespace mv