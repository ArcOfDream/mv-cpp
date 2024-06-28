#include <glm/ext/matrix_float3x3.hpp>
#include <glm/ext/vector_float2.hpp>
#include <glm/mat3x3.hpp>
#include <glm/vec2.hpp>

#pragma once

namespace mv {
class Camera2D {
  public:
    glm::mat3 view_transform = glm::mat3(1.0f);
    glm::vec2 size = {0, 0};
    glm::vec2 position = {0, 0};
    glm::vec2 offset = {0, 0};
    glm::vec2 scale = {1, 1};
    float rotation = 0.0f;
    bool center_camera = true;

    Camera2D();
    void set_position(glm::vec2);
    // void set_rotation(float);
    void update();
};
} // namespace mv
