#pragma once

#include <stb_image.h>
#include <string>
#include <glad/glad.h>

#include "shader.hpp"

class Texture {
public:
    GLuint ID{};
    const std::string type;
    GLuint unit;

    Texture(
        const std::string& image,
        const std::string& tex_type,
        GLuint slot,
        GLenum format,
        GLenum pixel_type);

    void tex_unit(const Shader& shader, const std::string& uniform, GLuint _unit) const;
    void bind_tex() const;
    void unbind_tex() const;
    void delete_tex() const;
};
