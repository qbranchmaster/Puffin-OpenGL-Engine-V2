/*
* Puffin OpenGL Engine ver. 2.0
* Coded by: Sebastian 'qbranchmaster' Tabaka
*/

#ifndef PUFFIN_FRAME_BUFFER_HPP
#define PUFFIN_FRAME_BUFFER_HPP

#ifdef WIN32 // Prevents APIENTRY redefinition
#include <Windows.h>
#endif // WIN32
#include <GL/glew.h>

#include <glm/glm.hpp>

#include <memory>

#include "PuffinEngine/Logger.hpp"
#include "PuffinEngine/RenderBuffer.hpp"
#include "PuffinEngine/StateMachine.hpp"
#include "PuffinEngine/Texture.hpp"

namespace puffin {
    enum class FrameBufferBindType {
        NORMAL,
        ONLY_READ,
        ONLY_WRITE,
    };

    class FrameBuffer;
    using FrameBufferPtr = std::shared_ptr<FrameBuffer>;

    class FrameBuffer {
    public:
        FrameBuffer();
        ~FrameBuffer();

        void bind(FrameBufferBindType bind_type) const {
            if (!handle_) {
                logError("FrameBuffer::bind()",
                    "Cannot bind null frame buffer.");
                return;
            }

            GLuint target = 0;
            GLuint *handle_ptr = nullptr;
            switch (bind_type) {
            case FrameBufferBindType::NORMAL:
                handle_ptr = &StateMachine::instance().bound_frame_buffer_;
                target = GL_FRAMEBUFFER;
                break;
            case FrameBufferBindType::ONLY_READ:
                handle_ptr = &StateMachine::instance().bound_frame_buffer_read_;
                target = GL_READ_FRAMEBUFFER;
                break;
            case FrameBufferBindType::ONLY_WRITE:
                handle_ptr = &StateMachine::instance().bound_frame_buffer_write_;
                target = GL_DRAW_FRAMEBUFFER;
                break;
            }

            if (*handle_ptr == handle_) {
                return;
            }

            glBindFramebuffer(target, handle_);
            *handle_ptr = handle_;

            if (bind_type == FrameBufferBindType::NORMAL) {
                StateMachine::instance().bound_frame_buffer_read_ = handle_;
                StateMachine::instance().bound_frame_buffer_write_ = handle_;
            }
        }

        void unbind() const {
            if (StateMachine::instance().bound_frame_buffer_ == handle_) {
                glBindFramebuffer(GL_FRAMEBUFFER, 0);
                StateMachine::instance().bound_frame_buffer_ = 0;
                StateMachine::instance().bound_frame_buffer_read_ = 0;
                StateMachine::instance().bound_frame_buffer_write_ = 0;
            }
        }

        void copyFrameBuffer(FrameBufferPtr target);

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

        void addTextureBuffer(GLuint width, GLuint height,
            GLboolean multisample);
        void addRenderBuffer(GLuint width, GLuint height,
            GLboolean multisample);

        void setBackgroundColor(const glm::vec3 &color);

        glm::vec3 getBackgroundColor() const {
            return background_color_;
        }

    private:
        GLboolean isBound() const;

        GLuint handle_{0};

        TexturePtr rgb_buffer_;
        RenderBufferPtr depth_buffer_;

        glm::vec3 background_color_{0.0f, 0.0f, 0.0f};
    };
} // namespace puffin

#endif // PUFFIN_FRAME_BUFFER_HPP