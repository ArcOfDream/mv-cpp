#define SOL_ALL_SAFETIES_ON 1

#include "mv/objects/node.h"
#include <string>

namespace mv {

Node::Node(const char* _name) { name = _name; }

void Node::_update(double dt) {
    for (auto &child : children) {
        child->_update(dt);
    }
    update(dt);
}


} // namespace mv