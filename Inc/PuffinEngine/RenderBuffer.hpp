/*
* Puffin OpenGL Engine ver. 2.0
* Coded by: Sebastian 'qbranchmaster' Tabaka
*/

#ifndef PUFFIN_RENDER_BUFFER_HPP
#define PUFFIN_RENDER_BUFFER_HPP

#ifdef WIN32 // Prevents APIENTRY redefinition
#include <Windows.h>
#endif // WIN32
#include <GL/glew.h>

#include <memory>

#include "PuffinEngine/Configuration.hpp"
#include "PuffinEngine/Logger.hpp"
#include "PuffinEngine/StateMachine.hpp"

namespace puffin {
    class RenderBuffer {
    public:
        RenderBuffer() {
            glGenRenderbuffers(1, &handle_);
        }

        ~RenderBuffer() {
            if (handle_) {
                glDeleteRenderbuffers(1, &handle_);
            }
        }

        void bind() const {
            if (!handle_) {
                logError("RenderBuffer::bind()",
                    "Cannot bind null render buffer.");
                return;
            }

            if (StateMachine::instance().bound_render_buffer_ == handle_) {
                return;
            }

            glBindRenderbuffer(GL_RENDERBUFFER, handle_);
            StateMachine::instance().bound_render_buffer_ = handle_;
        }

        void unbind() const {
            if (StateMachine::instance().bound_render_buffer_ == handle_) {
                glBindRenderbuffer(GL_RENDERBUFFER, 0);
                StateMachine::instance().bound_render_buffer_ = 0;
            }
        }

        void create(GLuint width, GLuint height, GLboolean multisample) {
            bind();

            width_ = width;
            height_ = height;

            if (multisample) {
                glRenderbufferStorageMultisample(GL_RENDERBUFFER,
                    Configuration::instance().getMsaaSamples(),
                    GL_DEPTH24_STENCIL8, width_, height_);
            }
            else {
                glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8,
                    width_, height_);
            }

            unbind();
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

    using RenderBufferPtr = std::shared_ptr<RenderBuffer>;
} // namespace puffin

#endif // PUFFIN_RENDER_BUFFER_HPP