#include "shader.hpp"

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
    compile_errors(vertex_shader, "VERTEX");

    const GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_source, nullptr);
    glCompileShader(fragment_shader);
    compile_errors(vertex_shader, "FRAGMENT");

    ID = glCreateProgram();

    glAttachShader(ID, vertex_shader);
    glAttachShader(ID, fragment_shader);
    glLinkProgram(ID);
    compile_errors(ID, "PROGRAM");

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


void Shader::compile_errors(const unsigned int shader, const std::string& type)
{
    GLint has_compiled;
    char info_log[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &has_compiled);
        if (has_compiled == GL_FALSE)
        {
            glGetShaderInfoLog(shader, 1024, nullptr, info_log);
            std::cout << "SHADER_COMPILATION_ERROR for: "<< type << '\n' << info_log << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &has_compiled);
        if (has_compiled == GL_FALSE)
        {
            glGetProgramInfoLog(shader, 1024, nullptr, info_log);
            std::cout << "SHADER_LINKING_ERROR for: "<< type << '\n' << info_log << std::endl;
        }
    }
}
