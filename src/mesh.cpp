#include "mesh.hpp"

Mesh::Mesh(
    std::span<Vertex> vertices,
    std::span<GLuint> indices,
    std::span<Texture> textures):
vertices(vertices.begin(), vertices.end()),
indices(indices.begin(), indices.end()),
textures(textures.begin(), textures.end())
{
    vao.bind_vao();
    const VBO vbo {this->vertices};
    const EBO ebo {this->indices};

    vao.link_attrib(vbo, 0, 3, GL_FLOAT, sizeof(Vertex), nullptr);
    vao.link_attrib(vbo, 1, 3, GL_FLOAT, sizeof(Vertex), reinterpret_cast<void *>(3 * sizeof(float)));
    vao.link_attrib(vbo, 2, 3, GL_FLOAT, sizeof(Vertex), reinterpret_cast<void *>(6 * sizeof(float)));
    vao.link_attrib(vbo, 3, 2, GL_FLOAT, sizeof(Vertex), reinterpret_cast<void *>(9 * sizeof(float)));

    vao.unbind_vao();
    vbo.unbind_vbo();
    ebo.unbind_ebo();
}

void Mesh::draw(const Shader &shader, const Camera &camera) const
{
    shader.activate_shader();
    vao.bind_vao();

    unsigned int num_diffuse {0};
    unsigned int num_specular {0};

    for (unsigned int i = 0; i < textures.size(); ++i)
    {
        std::string num {};
        std::string type { textures[i].type };

        if (type == "diffuse")
        {
            num = std::to_string(num_diffuse++);
        }
        else if (type == "specular")
        {
            num = std::to_string(num_specular++);
        }
        textures[i].tex_unit(shader, (type + num), i);
        textures[i].bind_tex();
    }

    glUniform3f(
        glGetUniformLocation(shader.ID, "cam_pos"),
        camera.position.x, camera.position.y, camera.position.z
        );

    camera.matrix(shader, "camera_matrix");

    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
}
