#pragma once

#include <span>
#include <string>

#include "VAO.hpp"
#include "EBO.hpp"
#include "camera.hpp"
#include "texture.hpp"

class Mesh {
public:
    std::vector<Vertex> vertices {};
    std::vector<GLuint> indices {};
    std::vector<Texture> textures {};

    VAO vao {};

    Mesh(
        std::span<Vertex> vertices,
        std::span<GLuint> indices,
        std::span<Texture> textures);

    void draw(const Shader& shader, const Camera& camera) const;
};
