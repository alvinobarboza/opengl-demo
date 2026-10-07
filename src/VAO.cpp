#include "VAO.hpp"

VAO::VAO()
{
    glGenVertexArrays(1, &ID);
}

void VAO::link_attrib(const VBO &VBO, const GLuint layout, const GLuint num_components, const GLenum type, const GLsizeiptr stride, const void *offset)
{
    VBO.bind_vbo();
    glVertexAttribPointer(layout, num_components, type, GL_FALSE, stride, offset);
    glEnableVertexAttribArray(layout);
    VBO.unbind_vbo();
}

void VAO::bind_vao() const
{
    glBindVertexArray(ID);
}

void VAO::unbind_vao()
{
    glBindVertexArray(0);
}

void VAO::delete_vao() const
{
    glDeleteVertexArrays(1, &ID);
}
