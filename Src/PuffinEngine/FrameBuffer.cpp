/*
* Puffin OpenGL Engine ver. 2.0
* Coded by: Sebastian 'qbranchmaster' Tabaka
*/

#include "PuffinEngine/FrameBuffer.hpp"

using namespace puffin;

FrameBuffer::FrameBuffer() {
    glGenFramebuffers(1, &handle_);
}

FrameBuffer::~FrameBuffer() {
    if (handle_) {
        glDeleteFramebuffers(1, &handle_);
    }
}

void FrameBuffer::copyFrameBuffer(FrameBufferPtr target) {
    if (!target) {
        logError("FrameBuffer::copyFrameBuffer()", "Null input.");
    }

    bind(FrameBufferBindType::ONLY_READ);
    target->bind(FrameBufferBindType::ONLY_WRITE);

    auto size = rgb_buffer_->getSize();
    glBlitFramebuffer(0, 0, size.first, size.second, 0, 0, size.first,
        size.second, GL_COLOR_BUFFER_BIT, GL_NEAREST);

    unbind();
}

GLboolean FrameBuffer::isComplete() const {
    if (!isBound()) {
        logError("FrameBuffer::isComplete()", "Frame buffer is not bound.");
        return false;
    }

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        return false;
    }

    return true;
}

GLboolean FrameBuffer::isBound() const {
    if (StateMachine::instance().bound_frame_buffer_ == handle_) {
        return true;
    }

    return false;
}

void FrameBuffer::addRenderBuffer(GLuint width, GLuint height,
    GLboolean multisample) {
    if (!depth_buffer_) {
        depth_buffer_.reset(new RenderBuffer());
    }

    depth_buffer_->create(width, height, multisample);

    bind(FrameBufferBindType::NORMAL);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
        GL_RENDERBUFFER, depth_buffer_->getHandle());
    unbind();
}

void FrameBuffer::setBackgroundColor(const glm::vec3 & color) {
    background_color_ = glm::vec3(
        glm::clamp(color.r, 0.0f, 1.0f),
        glm::clamp(color.g, 0.0f, 1.0f),
        glm::clamp(color.b, 0.0f, 1.0f));
}

void FrameBuffer::addTextureBuffer(GLuint width, GLuint height,
    GLboolean multisample) {
    if (!rgb_buffer_) {
        rgb_buffer_.reset(new Texture());
    }

    rgb_buffer_->createTextureBuffer(width, height, multisample);

    bind(FrameBufferBindType::NORMAL);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
        multisample ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D,
        rgb_buffer_->getHandle(), 0);
    unbind();
}