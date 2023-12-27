#include "Shader.h"
#include "Renderer.h"
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <cassert>
using namespace std;


Shader::Shader(const std::string& filepath)
	: m_FilePath(filepath), m_RendererID(0)
{

    ShaderProgramSource sources = ParseShader(filepath);
    std::cout << "VERTEX" << std::endl << sources.VertexSource << std::endl;
    std::cout << "FRAGMENT" << std::endl << sources.FragmentSource << std::endl;
    this->m_RendererID = CreateShader(sources.VertexSource, sources.FragmentSource);
    //glUseProgram(m_RendererID);
}

Shader::~Shader() {
    glDeleteProgram(m_RendererID); 
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source) {
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str(); // c_str stringin ilk karakterine bir pointeri temsil ediyor yani &source[0] şeklinde de yazılabilirdi
    glShaderSource(id, 1, &src, nullptr); // shader id, number of sources provided, pointer to the source, each string is assumed to be null terminated

    glCompileShader(id);

    //error handling

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result); // sondaki iv: integer vector anlamına geliyor. Resulta iv vector verecek

    if (result == GL_FALSE) {
        int lenght;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &lenght);

        char* message = (char*)alloca(lenght * sizeof(char)); // şu şekilde de yapabilirdik: char* message = new char[lenght]
        glGetShaderInfoLog(id, lenght, &lenght, message);

        std::cout << "Failed to compile " <<
            (type == GL_VERTEX_SHADER ? " vertex" : " fragment") << " shader!" << std::endl;

        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);

	glLinkProgram(program);
    GLint program_linked;
    glGetProgramiv(program, GL_LINK_STATUS, &program_linked);

    if (program_linked != GL_TRUE)
    {
        GLsizei log_length = 0;
        GLchar message[1024];
        glGetProgramInfoLog(program, 1024, &log_length, message);
        cout << message << endl,
        exit(-1);
    }

	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}


ShaderProgramSource Shader::ParseShader(std::string filepath) {

    std::ifstream stream(filepath);

    enum class ShaderType {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::stringstream ss[2];

    ShaderType type = ShaderType::NONE;
    std::string line;
    while (getline(stream, line)) {

        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
        }
        else {
            ss[(int)type] << line << '\n';
        }
    }

    return { ss[0].str() , ss[1].str() };
}

void Shader::Bind() const {
    glUseProgram(this->m_RendererID);
}

void Shader::Unbind() const {
    glUseProgram(0);
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3) {
    int location = GetUniformLocation(name);
    glUniform4f(location, v0, v1, v2, v3);
}

void Shader::SetUniform1f(const std::string& name, float value) {
    int location = GetUniformLocation(name);
    glUniform1f(location, value);
}

void Shader::SetUniform1i(const std::string& name, int value) {
    int location = GetUniformLocation(name);
    glUniform1i(location, value);
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix) {
    glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
}

int Shader::GetUniformLocation(const std::string& name) {

    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
        return m_UniformLocationCache[name];

    int location = glGetUniformLocation(this->m_RendererID, name.c_str());
    if (location == -1) {
        std::cout << "warning uniform " << name.c_str() << " doesnt exist " << std::endl;
    }

    m_UniformLocationCache[name] = location;
    return location;
}