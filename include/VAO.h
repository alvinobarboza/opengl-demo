#pragma once

#include "glad/glad.h"
#include "VBO.h"

class VAO {
public:
    GLuint ID{};

    VAO();

    static void link_vbo(VBO& VBO, GLuint layout) ;
    void bind_vao() const ;
    static void unbind_vao() ;
    void delete_vao() const;
};