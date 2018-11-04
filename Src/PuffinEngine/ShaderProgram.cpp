/*
* Puffin OpenGL Engine
* Created by: Sebastian 'qbranchmaster' Tabaka
*/

#include "PuffinEngine/ShaderProgram.hpp"

#include "PuffinEngine/Logger.hpp"

using namespace puffin;

puffin::ShaderProgram::ShaderProgram() {
    handle_vs_ = glCreateShader(GL_VERTEX_SHADER);
    handle_fs_ = glCreateShader(GL_FRAGMENT_SHADER);

    handle_ = glCreateProgram();
}

ShaderProgram::~ShaderProgram() {
    if (handle_vs_) {
        glDetachShader(handle_, handle_vs_);
        glDeleteShader(handle_vs_);
    }

    if (handle_fs_) {
        glDetachShader(handle_, handle_fs_);
        glDeleteShader(handle_fs_);
    }

    if (handle_) {
        glDeleteProgram(handle_);
    }
}

void ShaderProgram::loadShaders(std::string vs_path, std::string fs_path) {
    if (vs_path.empty() || fs_path.empty()) {
        logError("ShaderProgram::loadShaders()", "Empty shader file path.");
        return;
    }

    std::vector<GLuint> shaders = {GL_VERTEX_SHADER, GL_FRAGMENT_SHADER};
    for (const auto &type : shaders) {
        GLuint *handle_ptr = nullptr;
        std::string *file_ptr = nullptr;
        if (type == GL_VERTEX_SHADER) {
            handle_ptr = &handle_vs_;
            file_ptr = &vs_path;
        }
        else if (type == GL_FRAGMENT_SHADER) {
            handle_ptr = &handle_fs_;
            file_ptr = &fs_path;
        }

        std::string shader_data;
        if (loadShaderCode(*file_ptr, shader_data)) {
            logError("ShaderProgram::loadShaders()",
                "Reading shader file [" + *file_ptr + "] error.");
            return;
        }

        const GLchar *shader_code = shader_data.c_str();
        glShaderSource(*handle_ptr, 1, &shader_code, nullptr);

        GLint result = compileShader(*handle_ptr);
        logInfo("ShaderProgram::loadShaders()", "Shader file [" + *file_ptr +
            "] compile message:\n" + getShaderCompileMessage(*handle_ptr));

        if (result) {
            logError("ShaderProgram::loadShaders()", "Shader file [" +
                *file_ptr + "] compile error.");
            return;
        }

        logInfo("ShaderProgram::loadShaders()", "Shader file [" +
            *file_ptr + "] compile success.");
    }

    if (linkProgram()) {
        logError("ShaderProgram::loadShaders()", "Shader program link error.");
        return;
    }

    fetchUniforms();
    logInfo("ShaderProgram::loadShaders()", "Shader program link success.");
}

GLint ShaderProgram::loadShaderCode(std::string file_path,
    std::string &shader_code) const {
    std::ifstream shader_file(file_path, std::ios::in);
    if (shader_file.is_open()) {
        std::string line;
        while (std::getline(shader_file, line))
            shader_code += line + "\n";

        shader_file.close();
    }
    else {
        return -1;
    }

    return 0;
}

GLint ShaderProgram::compileShader(GLuint shader_handle) const {
    glCompileShader(shader_handle);
    if (checkShaderCompileStatus(shader_handle)) {
        return -1;
    }

    return 0;
}

GLint ShaderProgram::checkShaderCompileStatus(GLuint shader_handle) const {
    GLint shader_status = -1;
    glGetShaderiv(shader_handle, GL_COMPILE_STATUS, &shader_status);
    if (shader_status != GL_TRUE) {
        return -1;
    }

    return 0;
}

std::string ShaderProgram::getShaderCompileMessage(GLuint shader_handle) const {
    GLint log_size = 256;
    glGetShaderiv(shader_handle, GL_INFO_LOG_LENGTH, &log_size);

    std::string compile_msg;
    // There are some errors in shader, so log is not empty.
    if (log_size > 1) {
        GLchar *log_text = new GLchar[log_size];
        glGetShaderInfoLog(shader_handle, log_size, nullptr, log_text);
        compile_msg = std::string(log_text);
        delete log_text;
    }
    else {
        compile_msg = "[NO WARNINGS AND ERRORS]";
    }

    return compile_msg;
}

GLint ShaderProgram::linkProgram() const {
    glAttachShader(handle_, handle_vs_);
    glAttachShader(handle_, handle_fs_);

    glLinkProgram(handle_);
    if (checkProgramLinkStatus()) {
        return -1;
    }

    return 0;
}

GLint ShaderProgram::checkProgramLinkStatus() const {
    GLint link_status = -1;
    glGetProgramiv(handle_, GL_LINK_STATUS, &link_status);
    if (link_status != GL_TRUE) {
        return -1;
    }

    return 0;
}

void ShaderProgram::fetchUniforms() {
    std::vector<GLenum> properties;
    properties.push_back(GL_NAME_LENGTH);
    properties.push_back(GL_TYPE);
    properties.push_back(GL_ARRAY_SIZE);

    std::vector<GLint> values(properties.size());

    GLint uniforms_count = 0;
    glGetProgramInterfaceiv(handle_, GL_UNIFORM, GL_ACTIVE_RESOURCES,
        &uniforms_count);

    std::vector<GLchar> name_buffer;
    for (GLint i = 0; i < uniforms_count; i++) {
        glGetProgramResourceiv(handle_, GL_UNIFORM, i, properties.size(),
            &properties[0], values.size(), NULL, &values[0]);

        name_buffer.clear();
        name_buffer.resize(values[0]);

        glGetProgramResourceName(handle_, GL_UNIFORM, i, name_buffer.size(),
            nullptr, &name_buffer[0]);
        std::string uniform_name((GLchar*)&name_buffer[0],
            name_buffer.size() - 1);

        GLint location = glGetUniformLocation(handle_, uniform_name.c_str());
        uniforms_[uniform_name] = location;
    }
}