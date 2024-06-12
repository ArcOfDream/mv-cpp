#define GLM_ENABLE_EXPERIMENTAL

#include "mv/graphics/camera2d.h"
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/mat3x3.hpp>
#include <glm/matrix.hpp>
#include <glm/vec2.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>

namespace mv {
    Camera2D::Camera2D() {}

    void Camera2D::set_position(glm::vec2 p) {
        position.x = p.x;
    }

    void Camera2D::update() {
        glm::vec2 origin = offset;

        if (center_camera) { origin += size*0.5f; }

        view_transform = { 1.0f };
        glm::mat3 rotation_matrix = { 1.0f };

        rotation_matrix = glm::translate(rotation_matrix, origin);
        rotation_matrix = glm::scale(rotation_matrix, scale);
        rotation_matrix = glm::rotate(rotation_matrix, rotation);
        rotation_matrix = glm::translate(rotation_matrix, -origin);

        view_transform = glm::translate(view_transform, -origin);
        view_transform = glm::translate(view_transform, position);
        view_transform *= rotation_matrix;

        view_transform = glm::inverse(view_transform);

    }
}