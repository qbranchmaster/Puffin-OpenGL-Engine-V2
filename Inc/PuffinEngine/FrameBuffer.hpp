/*
* Puffin OpenGL Engine
* Created by: Sebastian 'qbranchmaster' Tabaka
*/

#ifndef PUFFIN_FRAME_BUFFER_HPP
#define PUFFIN_FRAME_BUFFER_HPP

#include <GL/glew.h>

#include <memory>

#include "PuffinEngine/Logger.hpp"

namespace puffin {
    class FrameBuffer {
    public:
        FrameBuffer();
        ~FrameBuffer();

        void bind() {
            if (!handle_) {
                logError("FrameBuffer::bind()",
                    "Cannot bind null framebuffer.");
                return;
            }

            if (bound_) {
                return;
            }

            glBindFramebuffer(GL_FRAMEBUFFER, handle_);
            bound_ = 1;
        }

        void unbind() {
            if (bound_) {
                glBindFramebuffer(GL_FRAMEBUFFER, 0);
                bound_ = 0;
            }
        }

        // TODO: After adding StateMachine this function should be const.
        GLboolean isComplete();

    private:
        GLuint handle_{0};
        GLboolean bound_{0};
    };

    using FrameBufferPtr = std::shared_ptr<FrameBuffer>;
} // namespace puffin

#endif // PUFFIN_FRAME_BUFFER_HPP