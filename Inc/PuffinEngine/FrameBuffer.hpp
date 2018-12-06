/*
* Puffin OpenGL Engine ver. 2.0
* Coded by: Sebastian 'qbranchmaster' Tabaka
*/

#ifndef PUFFIN_FRAME_BUFFER_HPP
#define PUFFIN_FRAME_BUFFER_HPP

#include <GL/glew.h>

#include <memory>

#include "PuffinEngine/Logger.hpp"
#include "PuffinEngine/RenderBuffer.hpp"
#include "PuffinEngine/StateMachine.hpp"
#include "PuffinEngine/Texture.hpp"

namespace puffin {
    class FrameBuffer {
    public:
        FrameBuffer();
        ~FrameBuffer();

        void bind() const {
            if (!handle_) {
                logError("FrameBuffer::bind()",
                    "Cannot bind null frame buffer.");
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

        GLuint getHandle() const {
            return handle_;
        }

        GLboolean isComplete() const;

        TexturePtr getRgbBufferTexture() const {
            return rgb_buffer_;
        }

        RenderBufferPtr getDepthRenderBuffer() const {
            return depth_buffer_;
        }

        void addTextureBuffer(GLuint width, GLuint height);
        void addRenderBuffer(GLuint width, GLuint height);

    private:
        GLboolean isBound() const;

        GLuint handle_{0};

        TexturePtr rgb_buffer_;
        RenderBufferPtr depth_buffer_;
    };

    using FrameBufferPtr = std::shared_ptr<FrameBuffer>;
} // namespace puffin

#endif // PUFFIN_FRAME_BUFFER_HPP