#ifndef PUFFIN_SHADER_PROGRAM_HPP
#define PUFFIN_SHADER_PROGRAM_HPP

#include <GL/glew.h>

#include <fstream>
#include <memory>
#include <string>
#include <vector>

#include "PuffinEngine/Logger.hpp"

namespace puffin {
    class ShaderProgram {
    public:
        ShaderProgram(std::string vs_path, std::string fs_path);
        ~ShaderProgram();

        void activate() {
            if (!handle_) {
                logError("ShaderProgram::activate()",
                    "Cannot activate empty shader program.");
                return;
            }

            glUseProgram(handle_);
        }

    private:
        GLint loadShaderCode(std::string file_path,
            std::string &shader_code) const;
        GLint compileShader(GLuint shader_handle) const;
        GLint checkShaderCompileStatus(GLuint shader_handle) const;
        std::string getShaderCompileMessage(GLuint shader_handle) const;
        GLint linkProgram() const;
        GLint checkProgramLinkStatus() const;

        GLuint handle_{0};
        GLuint handle_vs_{0};
        GLuint handle_fs_{0};
    };

    using ShaderProgramPtr = std::shared_ptr<ShaderProgram>;
} // namespace puffin

#endif // PUFFIN_SHADER_PROGRAM_HPP