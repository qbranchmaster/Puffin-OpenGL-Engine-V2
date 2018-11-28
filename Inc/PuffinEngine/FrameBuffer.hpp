/*
* Puffin OpenGL Engine ver. 2.0
* Created by: Sebastian 'qbranchmaster' Tabaka
*/

#ifndef PUFFIN_FRAME_BUFFER_HPP
#define PUFFIN_FRAME_BUFFER_HPP

#include <GL/glew.h>

#include <memory>

#include "PuffinEngine/Logger.hpp"
#include "PuffinEngine/StateMachine.hpp"

namespace puffin {
    class FrameBuffer {
    public:
        FrameBuffer();
        ~FrameBuffer();

        void bind() const {
            if (!handle_) {
                logError("FrameBuffer::bind()",
                    "Cannot bind null framebuffer.");
                return;
            }

            if (StateMachine::instance().bound_frame_buffer_ == handle_) {
                return;
            }

            glBindFramebuffer(GL_FRAMEBUFFER, handle_);
            StateMachine::instance().bound_frame_buffer_ = handle_;
        }

        void unbind() const {
            if (StateMachine::instance().bound_frame_buffer_ == handle_) {
                glBindFramebuffer(GL_FRAMEBUFFER, 0);
                StateMachine::instance().bound_frame_buffer_ = 0;
            }
        }

        void addTextureBuffer(GLuint width, GLuint height);
        void addRenderBuffer(GLuint width, GLuint height);

        GLboolean isComplete() const;

    private:
        GLuint handle_{0};

        GLuint rgb_texture_buffer_handle_{0};
        GLuint depth_render_buffer_handle_{0};
    };

    using FrameBufferPtr = std::shared_ptr<FrameBuffer>;
} // namespace puffin

#endif // PUFFIN_FRAME_BUFFER_HPP