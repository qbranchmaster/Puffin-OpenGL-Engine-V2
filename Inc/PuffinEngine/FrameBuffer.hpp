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
#include <vector>

#include "PuffinEngine/Configuration.hpp"
#include "PuffinEngine/DepthTextureBuffer.hpp"
#include "PuffinEngine/Logger.hpp"
#include "PuffinEngine/RenderBuffer.hpp"
#include "PuffinEngine/TextureBuffer.hpp"

namespace puffin {
    enum class FrameBufferBindType {
        NORMAL,
        ONLY_READ,
        ONLY_WRITE,
    };

    enum class FrameBufferClearType {
        ONLY_DEPTH,
        ONLY_COLOR,
        DEPTH_AND_COLOR,
    };

    class FrameBuffer;
    using FrameBufferPtr = std::shared_ptr<FrameBuffer>;

    class FrameBuffer {
    public:
        static void unbindAll() {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            bound_frame_buffer_ = 0;
            bound_frame_buffer_read_ = 0;
            bound_frame_buffer_write_ = 0;
        }

        static void clear(FrameBufferClearType clear_type,
            const glm::vec3 &color = glm::vec3(0.0f, 0.0f, 0.0f));
        static void setViewportSize(GLuint width, GLuint height);
        static void setViewportSize(FrameBufferPtr frame_buffer);

        FrameBuffer(GLuint width, GLuint height);
        ~FrameBuffer();

        GLuint getWidth() const {
            return width_;
        }

        GLuint getHeight() const {
            return height_;
        }

        void bind(FrameBufferBindType bind_type) {
            GLuint target = 0;
            GLuint *handle_ptr = nullptr;
            switch (bind_type) {
            case FrameBufferBindType::NORMAL:
                handle_ptr = &bound_frame_buffer_;
                target = GL_FRAMEBUFFER;
                break;
            case FrameBufferBindType::ONLY_READ:
                handle_ptr = &bound_frame_buffer_read_;
                target = GL_READ_FRAMEBUFFER;
                break;
            case FrameBufferBindType::ONLY_WRITE:
                handle_ptr = &bound_frame_buffer_write_;
                target = GL_DRAW_FRAMEBUFFER;
                break;
            }

            if (*handle_ptr == handle_) {
                return;
            }

            glBindFramebuffer(target, handle_);
            *handle_ptr = handle_;

            if (bind_type == FrameBufferBindType::NORMAL) {
                bound_frame_buffer_read_ = handle_;
                bound_frame_buffer_write_ = handle_;
            }
        }

        void unbind() {
            if (bound_frame_buffer_ == handle_) {
                glBindFramebuffer(GL_FRAMEBUFFER, 0);
                bound_frame_buffer_ = 0;
                bound_frame_buffer_read_ = 0;
                bound_frame_buffer_write_ = 0;
            }
            else if (bound_frame_buffer_read_ == handle_) {
                glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
                bound_frame_buffer_read_ = 0;
            }
            else if (bound_frame_buffer_write_ == handle_) {
                glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
                bound_frame_buffer_write_ = 0;
            }
        }

        void setClearColor(const glm::vec3 &color);

        glm::vec3 getClearColor() const {
            return clear_color_;
        }

        GLboolean isComplete();

        void copyFrameBuffer(FrameBufferPtr target, GLushort attachment_index);
        void addRenderBuffer(GLboolean multisample);
        void addTextureBuffer(GLushort index, GLboolean multisample,
            GLboolean float_buffer);
        void addDepthTextureBuffer();

        TextureBufferPtr getTextureBuffer(GLushort index) const {
            if (index >= texture_buffers_.size()) {
                logError("FrameBuffer::getTextureBuffer()",
                    "Invalid value.");
                return 0;
            }

            return texture_buffers_[index];
        }

        DepthTextureBufferPtr getDepthTextureBuffer() const {
            return depth_texture_buffer_;
        }

        void disableDrawBuffer();
        void disableReadBuffer();

    private:
        GLuint handle_{0};

        GLuint width_{0};
        GLuint height_{0};

        glm::vec3 clear_color_{0.0f, 0.0f, 0.0f};

        RenderBufferPtr render_buffer_;
        DepthTextureBufferPtr depth_texture_buffer_;
        std::vector<TextureBufferPtr> texture_buffers_;

        static GLuint bound_frame_buffer_;
        static GLuint bound_frame_buffer_write_;
        static GLuint bound_frame_buffer_read_;
    };
} // namespace puffin

#endif // PUFFIN_FRAME_BUFFER_HPP