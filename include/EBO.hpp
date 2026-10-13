#pragma once

#include <vector>

#include "glad/glad.h"

class EBO {
public:
    GLuint ID{};
    explicit EBO(const std::vector<GLuint>& indices);

    void bind_ebo() const ;
    static void unbind_ebo() ;
    void delete_ebo() const;
};