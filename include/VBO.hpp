#pragma once

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <vector>

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 color;
    glm::vec2 texUV;
};

class VBO {
public:
    GLuint ID{};

    explicit VBO(const std::vector<Vertex>& vertices);

    void bind_vbo() const ;
    static void unbind_vbo() ;
    void delete_vbo() const;
};