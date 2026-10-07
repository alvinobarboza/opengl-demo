#pragma once

#include "glad/glad.h"

class EBO {
public:
    GLuint ID{};
    EBO(GLuint* indices, GLsizeiptr size);

    void bind_ebo() const ;
    static void unbind_ebo() ;
    void delete_ebo() const;
};