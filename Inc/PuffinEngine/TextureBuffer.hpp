/*
 * Puffin OpenGL Engine ver. 2.1
 * Coded by: Sebastian 'qbranchmaster' Tabaka
 * Contact: sebastian.tabaka@outlook.com
 */

#ifndef PUFFIN_TEXTURE_BUFFER_HPP
#define PUFFIN_TEXTURE_BUFFER_HPP

#ifdef WIN32 // Prevents APIENTRY redefinition
#include <Windows.h>
#endif // WIN32
#include <GL/glew.h>

#include <memory>

#include "PuffinEngine/InitConfig.hpp"
#include "PuffinEngine/StateMachine.hpp"

namespace puffin {
    class TextureBuffer {
    public:
        TextureBuffer(GLuint width, GLuint height, GLboolean multisample, GLboolean float_buffer) {
            glGenTextures(1, &handle_);

            width_ = width;
            height_ = height;
            multisample_ = multisample;

            bind();

            if (multisample_) {
                glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE,
                    InitConfig::instance().getMsaaSamples(), float_buffer ? GL_RGB16F : GL_RGB,
                    width_, height_, GL_TRUE);
            }
            else {
                glTexImage2D(GL_TEXTURE_2D, 0, float_buffer ? GL_RGB16F : GL_RGB, width_, height_,
                    0, GL_RGB, GL_FLOAT, nullptr);
            }

            // Setup default filtering
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            // Setup wrap
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        }

        ~TextureBuffer() {
            glDeleteTextures(1, &handle_);
        }

        void bind() {
            if (handle_ == StateMachine::instance().bound_texture_) {
                return;
            }

            glBindTexture(multisample_ ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D, handle_);
            StateMachine::instance().bound_texture_ = handle_;
        }

        GLuint getWidth() const {
            return width_;
        }

        GLuint getHeight() const {
            return height_;
        }

        GLboolean isMultisample() const {
            return multisample_;
        }

        GLuint getHandle() const {
            return handle_;
        }

    private:
        GLuint handle_{0};

        GLuint width_{0};
        GLuint height_{0};
        GLboolean multisample_{false};
    };

    using TextureBufferPtr = std::shared_ptr<TextureBuffer>;
} // namespace puffin

#endif // PUFFIN_TEXTURE_BUFFER_HPP