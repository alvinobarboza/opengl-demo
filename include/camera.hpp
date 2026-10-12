#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "shader.hpp"


class Camera {
public:
    glm::vec3 position{0.0f, 0.0f, 0.0f};
    glm::vec3 orientation{0.0f, 0.0f, -1.0f};
    glm::vec3 up{0.0f, 1.0f, 0.0f};

    int width{800};
    int height{800};

    bool lock_was_pressed{false};
    bool lock_camera{false};

    float speed {0.01f};
    float sensitivity {100.0f};

    Camera(int width, int height, glm::vec3 position);

    void matrix(float fov_deg, float near_plane, float far_plane, const Shader& shader, const std::string& uniform_name) const;
    void input(GLFWwindow* window);
};