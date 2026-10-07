#pragma once

#include "glad/glad.h"

class VBO {
public:
    GLuint ID{};
    VBO(const GLfloat* vertices, GLsizeiptr size);

    void bind_vbo() const ;
    static void unbind_vbo() ;
    void delete_vbo() const;
};