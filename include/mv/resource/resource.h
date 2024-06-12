#include <string>

#pragma once

namespace mv {

enum ResourceType {
    INVALID = -1,
    IMAGE,
    AUDIO,
    PXTONE,
    SHADER,
    TEXT,
    JSON,
    SCENE,
    SCRIPT,
    ARCHIVE,
    OTHER,
};

class IResource {
  protected:
    ResourceType type;
    std::string name;

  public:
    ResourceType get_type() { return type; }
    std::string get_name() { return name; }
};

}