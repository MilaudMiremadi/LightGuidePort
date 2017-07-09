#include "shader.h"
#include <stdio.h>

using std::cerr;
using std::endl;
using std::string;

namespace rtr {
    
    shader::shader(const GLchar * vertexPath, const GLchar * fragmentPath) {
        uint vert;
        uint frag;
        const GLchar * source;
        {
            vert = glCreateShader(GL_VERTEX_SHADER);
            source = read_shader(vertexPath);
            glShaderSource(vert, 1, &source, NULL);
            glCompileShader(vert);
            check_errors(vert, "Vertex");
        }
        {
            frag = glCreateShader(GL_FRAGMENT_SHADER);
            source = read_shader(fragmentPath);
            glShaderSource(frag, 1, &source, NULL);
            glCompileShader(frag);
            check_errors(frag, "Fragment");
        }
        {
            id = glCreateProgram();
            glAttachShader(id, vert);
            glAttachShader(id, frag);
            glLinkProgram(id);
            check_errors(id, "Program");
        }
        glUseProgram(id);
        glDeleteShader(vert);
        glDeleteShader(frag);
        
    }
    
    void shader::use() const {
        glUseProgram(id);
    }
    
    void shader::uni_bool(const string & name, bool value) const {
        glUniform1i(glGetUniformLocation(id, name.c_str()), (int) value);
    }
    void shader::uni_int(const string & name, int value) const {
        glUniform1i(glGetUniformLocation(id, name.c_str()), value);
    }
    void shader::uni_float(const string & name, float value) const {
        glUniform1f(glGetUniformLocation(id, name.c_str()), value);
    }
    void shader::uni_vec2(const string & name, float x, float y) const {
        glUniform2f(glGetUniformLocation(id, name.c_str()), x, y);
    }
    void shader::uni_vec3(const string & name, float x, float y, float z) const {
        glUniform3f(glGetUniformLocation(id, name.c_str()), x, y, z);
    }
    void shader::uni_vec4(const string & name, float x, float y, float z, float w) const {
        glUniform4f(glGetUniformLocation(id, name.c_str()), x, y, z, w);
    }
    void shader::uni_vec2(const string &name, const glm::vec2 &value) const {
        glUniform2fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
    }
    void shader::uni_vec3(const string &name, const glm::vec3 &value) const {
        glUniform3fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
    }
    void shader::uni_vec4(const string &name, const glm::vec4 &value) const {
        glUniform4fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
    }
    void shader::uni_mat2(const string &name, const glm::mat2 &mat) const {
        glUniformMatrix2fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    void shader::uni_mat3(const string &name, const glm::mat3 &mat) const {
        glUniformMatrix3fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    void shader::uni_mat4(const string &name, const glm::mat4 &mat) const {
        glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    
    const GLchar* shader::read_shader(const char* filename) const {
        FILE* infile = fopen(filename, "rb");
        
        if (!infile) {
            printf("Unable to open file '%s'", filename);
            return NULL;
        }
        
        fseek(infile, 0, SEEK_END);
        int len = ftell(infile);
        fseek(infile, 0, SEEK_SET);
        
        GLchar* source = new GLchar[len + 1];
        
        fread(source, 1, len, infile);
        fclose(infile);
        
        source[len] = 0;
        
        return const_cast<const GLchar*>(source);
    }
    
    void shader::check_errors(uint shader, string type) const {
        int success;
        char infoLog[MAX_INFO_LOG_SIZE];
        if (type != "Program") {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(shader, MAX_INFO_LOG_SIZE, NULL, infoLog);
                printf("%s shader compilation error: \n%s\n", type.c_str(), infoLog);
            }
        } else {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success) {
                glGetProgramInfoLog(shader, MAX_INFO_LOG_SIZE, NULL, infoLog);
                printf("%s program linking error: \n%s\n", type.c_str(), infoLog);
            }
        }
    }
}
