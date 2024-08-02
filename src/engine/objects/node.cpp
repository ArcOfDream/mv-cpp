#define SOL_ALL_SAFETIES_ON 1

#include "mv/objects/node.h"
#include "mv/util.h"
#include <string>

namespace mv {

Node::Node(std::string _name) { name = _name; }
Node::Node(wren::Variable derived, std::string _name) : Node(_name) {
    wren_init = derived.func("init(_)");
    wren_update = derived.func("update(_,_)");
    wren_input = derived.func("input(_)");

    wren_constructed = true;
}

DEFAULT_UPDATE(Node);

} // namespace mv
