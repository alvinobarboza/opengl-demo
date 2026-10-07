#include "shader.h"

std::string get_file_contents(const char *filename)
{
    if (std::ifstream in(filename, std::ios::binary); in)
    {
        std::string contents;
        in.seekg(0, std::ios::end);
        contents.resize(in.tellg());
        in.seekg(0, std::ios::beg);
        in.read(&contents[0], static_cast<std::streamsize>(contents.size()));
        in.close();
        return contents;
    }
    throw(errno);
}


Shader::Shader(const char *vertex_file, const char *fragment_file)
{
    const std::string fragment_code = get_file_contents(fragment_file);
    const std::string vertex_code = get_file_contents(vertex_file);

    const char* fragment_source = fragment_code.c_str();
    const char* vertex_source = vertex_code.c_str();

    const GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_source, nullptr);
    glCompileShader(vertex_shader);

    const GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_source, nullptr);
    glCompileShader(fragment_shader);

    ID = glCreateProgram();

    glAttachShader(ID, vertex_shader);
    glAttachShader(ID, fragment_shader);
    glLinkProgram(ID);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
}

void Shader::activate_shader() const
{
    glUseProgram(ID);
}

void Shader::delete_shader() const
{
    glDeleteProgram(ID);
}
