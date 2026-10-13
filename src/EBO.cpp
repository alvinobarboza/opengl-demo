#include "EBO.hpp"

EBO::EBO(const std::vector<GLuint>& indices)
{
    glGenBuffers(1, &ID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        indices.size() * sizeof(GLuint), indices.data(),
        GL_STATIC_DRAW);
}

void EBO::bind_ebo() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
}

void EBO::unbind_ebo()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void EBO::delete_ebo() const
{
    glDeleteBuffers(1, &ID);
}
