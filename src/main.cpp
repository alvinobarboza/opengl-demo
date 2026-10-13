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
{ //     COORDINATES     /        COLORS        /    TexCoord    /       NORMALS     //
    -1.0f, 0.0f,  1.0f,		0.0f, 0.0f, 0.0f,		0.0f, 0.0f,		0.0f, 1.0f, 0.0f,
    -1.0f, 0.0f, -1.0f,		0.0f, 0.0f, 0.0f,		0.0f, 1.0f,		0.0f, 1.0f, 0.0f,
     1.0f, 0.0f, -1.0f,		0.0f, 0.0f, 0.0f,		1.0f, 1.0f,		0.0f, 1.0f, 0.0f,
     1.0f, 0.0f,  1.0f,		0.0f, 0.0f, 0.0f,		1.0f, 0.0f,		0.0f, 1.0f, 0.0f
};

// Indices for vertices order
GLuint indices[] =
{
    0, 1, 2,
    0, 2, 3
};

GLfloat light_vertices[] = {
    -0.1f, -0.1f,  0.1f,
    -0.1f, -0.1f, -0.1f,
     0.1f, -0.1f, -0.1f,
     0.1f, -0.1f,  0.1f,
    -0.1f,  0.1f,  0.1f,
    -0.1f,  0.1f, -0.1f,
     0.1f,  0.1f, -0.1f,
     0.1f,  0.1f,  0.1f,
};

GLuint light_indices[] = {
    0, 1, 2,
    0, 2, 3,
    0, 4, 7,
    0, 7, 3,
    3, 7, 6,
    3, 6, 2,
    2, 6, 5,
    2, 5, 1,
    1, 5, 4,
    1, 4, 0,
    4, 5, 6,
    4, 6, 7
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

    VAO1.link_attrib(VBO1, 0, 3, GL_FLOAT, 11 * sizeof(float), nullptr);
    VAO1.link_attrib(VBO1, 1, 3, GL_FLOAT, 11 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
    VAO1.link_attrib(VBO1, 2, 2, GL_FLOAT, 11 * sizeof(float), reinterpret_cast<void *>(6 * sizeof(float)));
    VAO1.link_attrib(VBO1, 3, 3, GL_FLOAT, 11 * sizeof(float), reinterpret_cast<void *>(8 * sizeof(float)));
    VAO1.unbind_vao();
    VBO1.unbind_vbo();
    EBO1.unbind_ebo();

    // Light shader

    Shader light_shader {"shader/light.vert", "shader/light.frag"};

    const VAO light_VAO;
    light_VAO.bind_vao();

    const VBO light_VBO{light_vertices, sizeof(light_vertices)};
    const EBO light_EBO{light_indices, sizeof(light_indices)};

    light_VAO.link_attrib(light_VBO, 0, 3, GL_FLOAT, 3 * sizeof(float), reinterpret_cast<void *>(0));

    light_VAO.unbind_vao();
    light_VBO.unbind_vbo();
    light_EBO.unbind_ebo();

    glm::vec4 light_color {1.0f, 1.0f, 1.0f, 1.0f};

    glm::vec3 light_pos {0.5f, 0.5f, 0.5f};
    glm::mat4 light_model {1.0f};
    light_model = glm::translate(light_model, light_pos);

    glm::vec3 pyramid_pos {0.0f, 0.0f, 0.0f};
    glm::mat4 pyramid_model {1.0f};
    pyramid_model = glm::translate(pyramid_model, pyramid_pos);

    light_shader.activate_shader();
    glUniformMatrix4fv(glGetUniformLocation(light_shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(light_model));
    glUniform4f(glGetUniformLocation(light_shader.ID, "light_color"), light_color.x, light_color.y, light_color.z, light_color.w);

    shader_program.activate_shader();
    glUniformMatrix4fv(glGetUniformLocation(shader_program.ID, "model"), 1, GL_FALSE, glm::value_ptr(pyramid_model));
    glUniform4f(glGetUniformLocation(shader_program.ID, "light_color"), light_color.x, light_color.y, light_color.z, light_color.w);
    glUniform3f(glGetUniformLocation(shader_program.ID, "light_pos"), light_pos.x, light_pos.y, light_pos.z);

    // Texture

    Texture checker{"assets/uv_checker.png", GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE};
    checker.tex_unit(shader_program, "tex0", 0);

    Texture checker_spec{"assets/uv_checker_specular.png", GL_TEXTURE_2D, 1, GL_RGBA, GL_UNSIGNED_BYTE};
    checker_spec.tex_unit(shader_program, "tex1", 1);
    // end texture

    glEnable(GL_DEPTH_TEST);

    Camera camera {WIDTH, HEIGHT, glm::vec3(0.0f, 0.0f, 2.0f)};

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        camera.input(window);
        camera.update_matrix(45.0f, 0.1f, 100.0f);

        shader_program.activate_shader();

        glUniform3f(glGetUniformLocation(shader_program.ID, "cam_pos"), camera.position.x, camera.position.y, camera.position.z);
        camera.matrix(shader_program, "camera_matrix");

        checker.bind_tex();
        checker_spec.bind_tex();

        VAO1.bind_vao();

        glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(int), GL_UNSIGNED_INT, nullptr);

        light_shader.activate_shader();
        camera.matrix(light_shader, "camera_matrix");
        light_VAO.bind_vao();
        glDrawElements(GL_TRIANGLES, sizeof(light_indices)/sizeof(int), GL_UNSIGNED_INT, nullptr);

        glfwSwapBuffers(window);

        glfwPollEvents();

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
    }
    VAO1.delete_vao();
    light_VAO.delete_vao();
    VBO1.delete_vbo();
    light_VBO.delete_vbo();
    EBO1.delete_ebo();
    light_EBO.delete_ebo();
    shader_program.delete_shader();
    light_shader.delete_shader();
    checker.delete_tex();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}