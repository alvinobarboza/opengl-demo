#pragma once

#include "glad/glad.h"
#include "VBO.hpp"

class VAO {
public:
    GLuint ID{};

    VAO();

    static void link_attrib(const VBO& VBO, GLuint layout, GLuint num_components, GLenum type, GLsizeiptr stride, const void* offset);
    void bind_vao() const ;
    static void unbind_vao() ;
    void delete_vao() const;
};