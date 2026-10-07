#include "VBO.h"

VBO::VBO(const GLfloat *vertices, const GLsizeiptr size)
{
    glGenBuffers(1, &ID);
    glBindBuffer(GL_ARRAY_BUFFER, ID);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
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
