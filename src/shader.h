#ifndef SHADER_H
#define SHADER_H
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <iostream>
#include "common.h"

namespace rtr {
    
    /**
     * Represents an OpenGL shader program
     */
    class shader {
        public:
            
            /**
             * The program id
             */
            uint id;

            /**
             * Create a shader program using the given vertex and fragment shaders
             */
            shader(const GLchar * vertexPath, const GLchar * fragmentPath);

            ~shader() {
                std::cout << "Shader " << id << " destroyed" << std::endl;
            }
            
            /**
             * Bind shader
             */
            void use() const;

            // Uniforms
            void uni_bool(const std::string &name, bool value) const;
            void uni_int(const std::string &name, int value) const;
            void uni_float(const std::string &name, float value) const;
            void uni_vec2(const std::string &name, float x, float y) const;
            void uni_vec3(const std::string &name, float x, float y, float z) const;
            void uni_vec4(const std::string &name, float x, float y, float z, float w) const;
            void uni_vec2(const std::string &name, const glm::vec2 &value) const;
            void uni_vec3(const std::string &name, const glm::vec3 &value) const;
            void uni_vec4(const std::string &name, const glm::vec4 &value) const;
            void uni_mat2(const std::string &name, const glm::mat2 &mat) const;
            void uni_mat3(const std::string &name, const glm::mat3 &mat) const;
            void uni_mat4(const std::string &name, const glm::mat4 &mat) const;

        private:
            
            /**
             * Read a shader from a file
             */
            const GLchar* read_shader(const char* filename) const;

            /**
             * Check for linking or compilation errors
             */
            void check_errors(uint shader, std::string type) const;
    };
}

#endif /* SHADER_H */
