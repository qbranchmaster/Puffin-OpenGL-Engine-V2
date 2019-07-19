/*
 * Puffin OpenGL Engine ver. 2.0.1
 * Coded by: Sebastian 'qbranchmaster' Tabaka
 * Contact: sebastian.tabaka@outlook.com
 */

#ifndef PUFFIN_RENDER_BUFFER_HPP
#define PUFFIN_RENDER_BUFFER_HPP

#ifdef WIN32 // Prevents APIENTRY redefinition
#include <Windows.h>
#endif // WIN32
#include <GL/glew.h>

#include <memory>

#include "PuffinEngine/InitConfig.hpp"

namespace puffin {
    class RenderBuffer {
    public:
        RenderBuffer(GLuint width, GLuint height, GLboolean multisample) {
            glGenRenderbuffers(1, &handle_);

            width_ = width;
            height_ = height;
            multisample_ = multisample;

            bind();

            if (multisample) {
                glRenderbufferStorageMultisample(GL_RENDERBUFFER,
                    InitConfig::instance().getMsaaSamples(), GL_DEPTH24_STENCIL8, width_, height_);
            }
            else {
                glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width_, height_);
            }
        }

        ~RenderBuffer() {
            glDeleteRenderbuffers(1, &handle_);
        }

        void bind() {
            glBindRenderbuffer(GL_RENDERBUFFER, handle_);
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

        GLboolean isMultisample() const {
            return multisample_;
        }

    private:
        GLuint handle_{0};

        GLuint width_{0};
        GLuint height_{0};
        GLboolean multisample_{false};
    };

    using RenderBufferPtr = std::shared_ptr<RenderBuffer>;
} // namespace puffin

#endif // PUFFIN_RENDER_BUFFER_HPP