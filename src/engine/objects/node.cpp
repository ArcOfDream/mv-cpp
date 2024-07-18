#define SOL_ALL_SAFETIES_ON 1

#include "mv/objects/node.h"
#include <string>

namespace mv {

Node::Node(std::string _name) { name = _name; }
Node::Node(wren::Variable derived, std::string _name) : Node(_name) { 
    wren_init = derived.func("init(_)");
    wren_update = derived.func("update(_,_)");

    wren_constructed = true;
}

void Node::_update(double dt) {
    for (auto &child : children) {
        child->_update(dt);
    }

    if (wren_constructed) wren_update(this, dt);
    else update(dt);
}


} // namespace mv
