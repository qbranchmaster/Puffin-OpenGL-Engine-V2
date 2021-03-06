/*
 * Puffin OpenGL Engine ver. 2.0.1
 * Coded by: Sebastian 'qbranchmaster' Tabaka
 * Contact: sebastian.tabaka@outlook.com
 */

#ifndef PUFFIN_DEPTH_TEXTURE_BUFFER_HPP
#define PUFFIN_DEPTH_TEXTURE_BUFFER_HPP

#ifdef WIN32 // Prevents APIENTRY redefinition
#include <Windows.h>
#endif // WIN32
#include <GL/glew.h>

#include <memory>

#include "PuffinEngine/InitConfig.hpp"

namespace puffin {
    class DepthTextureBuffer {
    public:
        DepthTextureBuffer(
            GLuint width, GLuint height, GLboolean multisample, GLboolean float_buffer) {
            glGenTextures(1, &handle_);

            width_ = width;
            height_ = height;
            multisample_ = multisample;

            bind();

            if (multisample_) {
                glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE,
                    InitConfig::instance().getMsaaSamples(),
                    float_buffer ? GL_DEPTH_COMPONENT32F : GL_DEPTH_COMPONENT, width_, height_,
                    GL_TRUE);
            }
            else {
                glTexImage2D(GL_TEXTURE_2D, 0,
                    float_buffer ? GL_DEPTH_COMPONENT32F : GL_DEPTH_COMPONENT, width_, height_, 0,
                    GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
            }

            // Setup filtering
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            // Setup wrap
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
            // Setup border color
            GLfloat border_color[] = {1.0f, 0.0f, 0.0f, 0.0f};
            glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border_color);
        }

        ~DepthTextureBuffer() {
            glDeleteTextures(1, &handle_);
        }

        void bind() {
            glBindTexture(multisample_ ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D, handle_);
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

        GLboolean isMultisampled() const {
            return multisample_;
        }

    private:
        GLuint handle_{0};

        GLuint width_{0};
        GLuint height_{0};
        GLboolean multisample_{false};
    };

    using DepthTextureBufferPtr = std::shared_ptr<DepthTextureBuffer>;
} // namespace puffin

#endif // PUFFIN_DEPTH_TEXTURE_BUFFER_HPP