#include "camera.hpp"

Camera::Camera(int width, int height, glm::vec3 position):
position(position),
width(width),
height(height)
{}

void Camera::update_matrix(
    const float fov_deg,
    const float near_plane,
    const float far_plane) {
    glm::mat4 view {1.0f};
    glm::mat4 projection {1.0f};

    view = glm::lookAt(position, position + orientation, up);
    projection = glm::perspective(
        glm::radians(fov_deg),
        static_cast<float>(width)/static_cast<float>(height),
        near_plane,
        far_plane);

    camera_matrix = projection * view;
}

void Camera::matrix(const Shader &shader, const std::string& uniform_name) const {
    glUniformMatrix4fv(
        glGetUniformLocation(shader.ID, uniform_name.c_str()),
        1, GL_FALSE, glm::value_ptr(camera_matrix));
}

void Camera::input(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        position += speed * orientation;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        position -= speed * orientation;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        position -= speed * glm::normalize(glm::cross(orientation, up));
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        position += speed * glm::normalize(glm::cross(orientation, up));
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        position += speed * up;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    {
        position -= speed * up;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        speed = 0.04f;
    } else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
    {
        speed = 0.01f;
    }

    const bool lock_pressed {glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS};
    const bool lock_action {lock_pressed && !lock_was_pressed};
    lock_was_pressed = lock_pressed;
    if (lock_action)
    {
        if (!lock_camera)
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            const auto f_width {static_cast<float>(width)};
            const auto f_height {static_cast<float>(height)};
            lock_camera = true;
            glfwSetCursorPos(window, f_width / 2, f_height / 2);
        } else
        {
            lock_camera = false;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    }

    if (lock_camera)
    {
        const auto f_width {static_cast<float>(width)};
        const auto f_height {static_cast<float>(height)};
        double mouse_x {};
        double mouse_y {};
        glfwGetCursorPos(window, &mouse_x, &mouse_y);

        const float rot_x = sensitivity * static_cast<float>(mouse_y - (f_height / 2.0f)) / f_height;
        const float rot_y = sensitivity * static_cast<float>(mouse_x - (f_width / 2.0f)) / f_width;

        auto new_orientation = glm::rotate(orientation, glm::radians(-rot_x), glm::normalize(glm::cross(orientation, up)));
        if (std::abs(glm::angle(new_orientation, up) - glm::radians(90.0f)) <= glm::radians(85.0f))
        {
            orientation = new_orientation;
        }

        orientation = glm::rotate(orientation, glm::radians(-rot_y), up);

        glfwSetCursorPos(window, f_width / 2, f_height / 2);
    }
}
