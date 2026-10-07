#include "VAO.h"

VAO::VAO()
{
    glGenVertexArrays(1, &ID);
}

void VAO::link_vbo(const VBO& VBO, const GLuint layout)
{
    VBO.bind_vbo();
    glVertexAttribPointer(layout, 3, GL_FLOAT, GL_FALSE, 0, static_cast<void *>(nullptr));
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
