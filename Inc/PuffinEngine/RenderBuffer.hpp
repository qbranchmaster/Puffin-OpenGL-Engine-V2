/*
* Puffin OpenGL Engine ver. 2.0
* Created by: Sebastian 'qbranchmaster' Tabaka
*/

#ifndef PUFFIN_RENDER_BUFFER_HPP
#define PUFFIN_RENDER_BUFFER_HPP

#include <GL/glew.h>

#include <memory>

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

        void bind() {
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

        void unbind() {
            if (StateMachine::instance().bound_render_buffer_ == handle_) {
                glBindRenderbuffer(GL_RENDERBUFFER, 0);
                StateMachine::instance().bound_render_buffer_ = 0;
            }
        }

        void create(GLuint width, GLuint height) {
            bind();
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8,
                width, height);
            unbind();
        }

        GLuint getHandle() const {
            return handle_;
        }

    private:
        GLuint handle_{0};
    };

    using RenderBufferPtr = std::shared_ptr<RenderBuffer>;
} // namespace puffin

#endif // PUFFIN_RENDER_BUFFER_HPP