#include "texture.hpp"

Texture::Texture(
    const std::string &image,
    const std::string& tex_type,
    const GLuint slot,
    const GLenum format,
    const GLenum pixel_type): type(tex_type), unit(slot)
{
    int width_tex, height_tex, channels_tex;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* bytes = stbi_load(
        image.c_str(),
        &width_tex,
        &height_tex,
        &channels_tex,
        0);

    glGenTextures(1, &ID);
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, ID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(
        GL_TEXTURE_2D, 0,
        GL_RGBA,
        width_tex,
        height_tex, 0,
        format, pixel_type, bytes);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(bytes);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::tex_unit(
    const Shader& shader,
    const std::string &uniform,
    const GLuint _unit) const
{
    const GLuint tex_uni = glGetUniformLocation(shader.ID, uniform.c_str());
    shader.activate_shader();
    glUniform1i(tex_uni, _unit);
}

void Texture::bind_tex() const
{
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture::unbind_tex() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::delete_tex() const
{
    glDeleteTextures(1, &ID);
}
