#include "texture.h"

Texture::Texture(
    const std::string &image,
    const GLenum tex_type,
    const GLenum slot,
    const GLenum format,
    const GLenum pixel_type): type(tex_type)
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
    glActiveTexture(slot);
    glBindTexture(type, ID);

    glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(
        type, 0,
        GL_RGBA,
        width_tex,
        height_tex, 0,
        format, pixel_type, bytes);
    glGenerateMipmap(type);

    stbi_image_free(bytes);
    glBindTexture(type, 0);
}

void Texture::tex_unit(
    const Shader& shader,
    const std::string &uniform,
    const GLuint unit) const
{
    const GLuint tex0uni = glGetUniformLocation(shader.ID, uniform.c_str());
    shader.activate_shader();
    glUniform1i(tex0uni, unit);
}

void Texture::bind_tex() const
{
    glBindTexture(type, ID);
}

void Texture::unbind_tex() const
{
    glBindTexture(type, 0);
}

void Texture::delete_tex() const
{
    glDeleteTextures(1, &ID);
}
