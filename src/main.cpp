#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "texture.h"
#include "shader.hpp"
#include "VAO.hpp"
#include "VBO.hpp"
#include "EBO.hpp"
#include "camera.hpp"

// Vertices coordinates
GLfloat vertices[] =
{ //     COORDINATES     /        COLORS         /    TexCoord   //
    -0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,     0.0f, 0.0f,
    -0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,     1.0f, 0.0f,
     0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,     0.0f, 0.0f,
     0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,     1.0f, 0.0f,
     0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,     0.5f, 1.0f
};

// Indices for vertices order
GLuint indices[] =
{
    0, 1, 2,
    0, 2, 3,
    0, 1, 4,
    1, 2, 4,
    2, 3, 4,
    3, 0, 4
};

constexpr int WIDTH = 800;
constexpr int HEIGHT = 800;

int main() {
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WIDTH,HEIGHT, "OpenGL demo", nullptr, nullptr);

    if (window == nullptr)
    {
        std::cout << "Failed\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    gladLoadGL();

    glViewport(0,0, WIDTH,HEIGHT);

    // Shader setup

    const Shader shader_program {"shader/default.vert", "shader/default.frag"};

    const VAO VAO1;
    VAO1.bind_vao();

    const VBO VBO1{vertices, sizeof(vertices)};
    const EBO EBO1{indices, sizeof(indices)};

    VAO1.link_attrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), nullptr);
    VAO1.link_attrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
    VAO1.link_attrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), reinterpret_cast<void *>(6 * sizeof(float)));
    VAO1.unbind_vao();
    VBO1.unbind_vbo();
    EBO1.unbind_ebo();

    // Texture

    Texture checker{"assets/uv_checker.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE};
    checker.tex_unit(shader_program, "tex0", 0);
    // end texture

    glEnable(GL_DEPTH_TEST);

    Camera camera {WIDTH, HEIGHT, glm::vec3(0.0f, 0.0f, 2.0f)};

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shader_program.activate_shader();

        camera.input(window);
        camera.update_matrix(45.0f, 0.1f, 100.0f);
        camera.matrix(shader_program, "camera_matrix");

        checker.bind_tex();

        VAO1.bind_vao();

        glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(int), GL_UNSIGNED_INT, nullptr);
        glfwSwapBuffers(window);

        glfwPollEvents();
    }
    VAO1.delete_vao();
    VBO1.delete_vbo();
    EBO1.delete_ebo();
    shader_program.delete_shader();
    checker.delete_tex();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}