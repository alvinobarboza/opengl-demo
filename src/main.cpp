#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "shader.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

GLfloat vertices[] = {
    -0.5f, -0.5f * std::sqrt(3.0f) / 3.0f, 0.0f,            // Lower left corner
    0.5f, -0.5f * std::sqrt(3.0f) / 3.0f, 0.0f,             // Lower right corner
    0.0f, 0.5f * std::sqrt(3.0f) * 2.0f / 3.0f, 0.0f,       // Upper corner
    -0.5f / 2.0f, 0.5f * std::sqrt(3.0f) / 6.0f, 0.0f,      // Inner left
    0.5f / 2.0f, 0.5f * std::sqrt(3.0f) / 6.0f, 0.0f,       // Inner right
    0.0f, -0.5f * std::sqrt(3.0f) / 3.0f, 0.0f,             // Inner down
};

GLuint indices[] = {
    0, 3, 5, // Lower left triangle
    3, 2, 4, // Lower right triangle
    5, 4, 1 // Upper triangle
};

int main() {
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800,800, "OpenGL demo", nullptr, nullptr);

    if (window == nullptr)
    {
        std::cout << "Failed\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    gladLoadGL();

    glViewport(0,0, 800,800);

    // Shader setup

    const Shader shader_program {"../shader/default.vert", "../shader/default.frag"};

    const VAO VAO1;
    VAO1.bind_vao();

    const VBO VBO1{vertices, sizeof(vertices)};
    const EBO EBO1{indices, sizeof(indices)};

    VAO1.link_vbo(VBO1, 0);
    VAO1.unbind_vao();
    VBO1.unbind_vbo();
    EBO1.unbind_ebo();

    // end shader
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        shader_program.activate_shader();
        VAO1.bind_vao();

        glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, nullptr);
        glfwSwapBuffers(window);

        glfwPollEvents();
    }
    VAO1.delete_vao();
    VBO1.delete_vbo();
    EBO1.delete_ebo();
    shader_program.delete_shader();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}