/*
 * Puffin OpenGL Engine ver. 2.0.1
 * Coded by: Sebastian 'qbranchmaster' Tabaka
 * Contact: sebastian.tabaka@outlook.com
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

#include "PuffinEngine/CubeTextureBuffer.hpp"
#include "PuffinEngine/DepthTextureBuffer.hpp"
#include "PuffinEngine/InitConfig.hpp"
#include "PuffinEngine/Logger.hpp"
#include "PuffinEngine/RenderBuffer.hpp"
#include "PuffinEngine/TextureBuffer.hpp"

namespace puffin {
    enum class FrameBufferBindType {
        Normal,
        OnlyRead,
        OnlyWrite,
    };

    enum class FrameBufferClearType {
        OnlyDepth,
        OnlyColor,
        DepthAndColor,
    };

    class FrameBuffer;
    using FrameBufferPtr = std::shared_ptr<FrameBuffer>;

    class FrameBuffer {
    public:
        static void unbindAll() {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }

        static void clear(
            FrameBufferClearType clear_type, const glm::vec3 &color = glm::vec3(0.0f, 0.0f, 0.0f));
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
            case FrameBufferBindType::Normal:
                target = GL_FRAMEBUFFER;
                break;
            case FrameBufferBindType::OnlyRead:
                target = GL_READ_FRAMEBUFFER;
                break;
            case FrameBufferBindType::OnlyWrite:
                target = GL_DRAW_FRAMEBUFFER;
                break;
            }

            glBindFramebuffer(target, handle_);
        }

        void setClearColor(const glm::vec3 &color);

        glm::vec3 getClearColor() const {
            return clear_color_;
        }

        GLboolean isComplete();

        void copyFrameBuffer(
            FrameBufferPtr target, GLushort attachment_index, GLboolean copy_depth = false);
        void addRenderBuffer(GLboolean multisample);
        void addTextureBuffer(GLushort index, GLboolean multisample, GLboolean float_buffer);
        void addDepthTextureBuffer(GLboolean multisample, GLboolean float_buffer);
        void addCubeTextureBuffer();

        TextureBufferPtr getTextureBuffer(GLushort index) const {
            if (index >= texture_buffers_.size()) {
                logError("FrameBuffer::getTextureBuffer()", PUFFIN_MSG_INVALID_VALUE);
                return 0;
            }

            return texture_buffers_[index];
        }

        DepthTextureBufferPtr getDepthTextureBuffer() const {
            return depth_texture_buffer_;
        }

        CubeTextureBufferPtr getCubeTextureBuffer() const {
            return cube_texture_buffer_;
        }

        void disableDrawBuffer();
        void disableReadBuffer();

    private:
        GLuint handle_{0};

        GLuint width_{0};
        GLuint height_{0};

        glm::vec3 clear_color_{0.0f, 0.0f, 0.0f};

        RenderBufferPtr render_buffer_{nullptr};
        DepthTextureBufferPtr depth_texture_buffer_{nullptr};
        CubeTextureBufferPtr cube_texture_buffer_{nullptr};
        std::vector<TextureBufferPtr> texture_buffers_;
    };
} // namespace puffin

#endif // PUFFIN_FRAME_BUFFER_HPP