#include "VBO.hpp"

VBO::VBO(const std::vector<Vertex>& vertices)
{
    glGenBuffers(1, &ID);
    glBindBuffer(GL_ARRAY_BUFFER, ID);
    glBufferData(
        GL_ARRAY_BUFFER,
        vertices.size() * sizeof(Vertex),
        vertices.data(), GL_STATIC_DRAW);
}

void VBO::bind_vbo() const
{
    glBindBuffer(GL_ARRAY_BUFFER, ID);
}

void VBO::unbind_vbo()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VBO::delete_vbo() const
{
    glDeleteBuffers(1, &ID);
}
