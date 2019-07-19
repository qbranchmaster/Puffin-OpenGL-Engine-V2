/*
 * Puffin OpenGL Engine ver. 2.0.1
 * Coded by: Sebastian 'qbranchmaster' Tabaka
 * Contact: sebastian.tabaka@outlook.com
 */

#ifndef PUFFIN_CUBE_TEXTURE_BUFFER_HPP
#define PUFFIN_CUBE_TEXTURE_BUFFER_HPP

#ifdef WIN32 // Prevents APIENTRY redefinition
#include <Windows.h>
#endif // WIN32
#include <GL/glew.h>

#include <memory>

#include "PuffinEngine/InitConfig.hpp"

namespace puffin {
    class CubeTextureBuffer {
    public:
        CubeTextureBuffer(GLuint width, GLuint height) {
            glGenTextures(1, &handle_);

            width_ = width;
            height_ = height;

            bind();

            for (GLint i = 0; i < 6; i++) {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, width,
                    height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
            }

            // Setup filtering
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            // Setup wrap
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        }

        ~CubeTextureBuffer() {
            glDeleteTextures(1, &handle_);
        }

        void bind() {
            glBindTexture(GL_TEXTURE_CUBE_MAP, handle_);
        }

        GLuint getWidth() const {
            return width_;
        }

        GLuint getHeight() const {
            return height_;
        }

        GLuint getHandle() const {
            return handle_;
        }

    private:
        GLuint handle_{0};

        GLuint width_{0};
        GLuint height_{0};
    };

    using CubeTextureBufferPtr = std::shared_ptr<CubeTextureBuffer>;
} // namespace puffin

#endif // PUFFIN_CUBE_TEXTURE_BUFFER_HPP