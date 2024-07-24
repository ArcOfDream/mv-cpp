#include <memory>
#include <string>

#pragma once

namespace mv {

enum ResourceType {
    INVALID = -1,
    IMAGE,
    AUDIO,
    PXTONE,
    SHADER,
    MATERIAL,
    TEXT,
    JSON,
    SCENE,
    SCRIPT,
    ARCHIVE,
    OTHER,
};

class IResource : public std::enable_shared_from_this<IResource> {
  protected:
    ResourceType type;
    std::string name;

    template <typename D> std::shared_ptr<D> shared_from_base() {
        return std::static_pointer_cast<D>(shared_from_this());
    }

  public:
    ResourceType get_type() { return type; }
    std::string get_name() { return name; }
    std::shared_ptr<IResource> getptr() { return shared_from_this(); }
};

} // namespace mv
