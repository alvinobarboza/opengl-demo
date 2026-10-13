
#include "mesh.hpp"

// Vertices coordinates
Vertex vertices[] =
{ //               COORDINATES           /            COLORS          /           NORMALS         /       TEXTURE COORDINATES    //
    Vertex{glm::vec3(-1.0f, 0.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
    Vertex{glm::vec3(-1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
    Vertex{glm::vec3( 1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
    Vertex{glm::vec3( 1.0f, 0.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)}
};

// Indices for vertices order
GLuint indices[] =
{
    0, 1, 2,
    0, 2, 3
};

Vertex light_vertices[] =
{ //     COORDINATES     //
    Vertex{glm::vec3(-0.1f, -0.1f,  0.1f),{},{},{}},
    Vertex{glm::vec3(-0.1f, -0.1f, -0.1f),{},{},{}},
    Vertex{glm::vec3(0.1f, -0.1f, -0.1f),{},{},{}},
    Vertex{glm::vec3(0.1f, -0.1f,  0.1f),{},{},{}},
    Vertex{glm::vec3(-0.1f,  0.1f,  0.1f),{},{},{}},
    Vertex{glm::vec3(-0.1f,  0.1f, -0.1f),{},{},{}},
    Vertex{glm::vec3(0.1f,  0.1f, -0.1f),{},{},{}},
    Vertex{glm::vec3(0.1f,  0.1f,  0.1f), {},{},{}}
};

GLuint light_indices[] =
{
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

    // Texture

    Texture textures [] {
        Texture("assets/uv_checker.png", "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE),
        Texture("assets/uv_checker_specular.png", "specular", 1, GL_RGBA, GL_UNSIGNED_BYTE)
    };// end texture

    // Shader setup

    const Shader shader_program {"shader/default.vert", "shader/default.frag"};

    Mesh floor(vertices, indices, textures);

    // Light shader

    Shader light_shader {"shader/light.vert", "shader/light.frag"};

    Mesh light(light_vertices, light_indices, textures);

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



    glEnable(GL_DEPTH_TEST);

    Camera camera {WIDTH, HEIGHT, glm::vec3(0.0f, 0.0f, 2.0f)};

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        camera.input(window);
        camera.update_matrix(45.0f, 0.1f, 100.0f);

        floor.draw(shader_program, camera);
        light.draw(light_shader, camera);

        glfwSwapBuffers(window);

        glfwPollEvents();

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
    }
    shader_program.delete_shader();
    light_shader.delete_shader();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}