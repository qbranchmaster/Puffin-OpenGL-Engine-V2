/*
* Puffin OpenGL Engine ver. 2.0
* Created by: Sebastian 'qbranchmaster' Tabaka
*/

#ifndef PUFFIN_SHADER_PROGRAM_HPP
#define PUFFIN_SHADER_PROGRAM_HPP

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "PuffinEngine/Logger.hpp"
#include "PuffinEngine/StateMachine.hpp"

namespace puffin {
    class ShaderProgram {
    public:
        ShaderProgram();
        ~ShaderProgram();

        void bind() {
            if (!handle_) {
                logError("ShaderProgram::bind()",
                    "Cannot bind null shader program.");
                return;
            }

            if (StateMachine::instance().bound_shader_program_handle_ ==
                handle_) {
                return;
            }

            glUseProgram(handle_);
            StateMachine::instance().bound_shader_program_handle_ = handle_;
        }

        void loadShaders(std::string vs_path, std::string fs_path);

        void setUniform(std::string uniform_name, const glm::mat4 &value) {
            bind();
            auto location = getUniformLocation(uniform_name);
            if (location == -1) {
                logError("ShaderProgram::setUniform()", "Uniform [" +
                    uniform_name + "] does not exist in this shader program.");
                return;
            }

            glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
        }

        void setUniform(std::string uniform_name, const glm::vec3 &value) {
            bind();
            auto location = getUniformLocation(uniform_name);
            if (location == -1) {
                logError("ShaderProgram::setUniform()", "Uniform [" +
                    uniform_name + "] does not exist in this shader program.");
                return;
            }

            glUniform3fv(location, 1, glm::value_ptr(value));
        }

        void setUniform(std::string uniform_name, const glm::vec4 &value) {
            bind();
            auto location = getUniformLocation(uniform_name);
            if (location == -1) {
                logError("ShaderProgram::setUniform()", "Uniform [" +
                    uniform_name + "] does not exist in this shader program.");
                return;
            }

            glUniform4fv(location, 1, glm::value_ptr(value));
        }

        void setUniform(std::string uniform_name, GLint value) {
            bind();
            auto location = getUniformLocation(uniform_name);
            if (location == -1) {
                logError("ShaderProgram::setUniform()", "Uniform [" +
                    uniform_name + "] does not exist in this shader program.");
                return;
            }

            glUniform1iv(location, 1, &value);
        }

        void setUniform(std::string uniform_name, GLfloat value) {
            bind();
            auto location = getUniformLocation(uniform_name);
            if (location == -1) {
                logError("ShaderProgram::setUniform()", "Uniform [" +
                    uniform_name + "] does not exist in this shader program.");
                return;
            }

            glUniform1fv(location, 1, &value);
        }

    private:
        GLint loadShaderCode(std::string file_path,
            std::string &shader_code) const;
        GLint compileShader(GLuint shader_handle) const;
        GLint checkShaderCompileStatus(GLuint shader_handle) const;
        std::string getShaderCompileMessage(GLuint shader_handle) const;
        GLint linkProgram() const;
        GLint checkProgramLinkStatus() const;
        void fetchUniforms();

        GLint getUniformLocation(std::string uniform_name) const {
            auto location = uniforms_.find(uniform_name);
            if (location == uniforms_.end()) {
                return -1;
            }

            return location->second;
        }

        GLuint handle_{0};
        GLuint handle_vs_{0};
        GLuint handle_fs_{0};

        std::unordered_map<std::string, GLint> uniforms_;
    };

    using ShaderProgramPtr = std::shared_ptr<ShaderProgram>;
} // namespace puffin

#endif // PUFFIN_SHADER_PROGRAM_HPP