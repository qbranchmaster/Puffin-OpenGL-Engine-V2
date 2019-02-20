/*
* Puffin OpenGL Engine ver. 2.0
* Coded by: Sebastian 'qbranchmaster' Tabaka
*/

#include "PuffinEngine/FrameBuffer.hpp"

using namespace puffin;

FrameBuffer::FrameBuffer(GLuint width, GLuint height) {
    glGenFramebuffers(1, &handle_);

    width_ = width;
    height_ = height;
}

FrameBuffer::~FrameBuffer() {
    glDeleteFramebuffers(1, &handle_);
}

void FrameBuffer::setClearColor(const glm::vec3 & color) {
    clear_color_ = glm::vec3(
        glm::clamp(color.r, 0.0f, 1.0f),
        glm::clamp(color.g, 0.0f, 1.0f),
        glm::clamp(color.b, 0.0f, 1.0f));
}

void FrameBuffer::copyFrameBuffer(FrameBufferPtr target,
    GLushort attachment_index) {
    if (!target) {
        logError("FrameBuffer::copyFrameBuffer()", "Null input.");
        return;
    }

    if (attachment_index >= texture_buffers_.size()) {
        logError("FrameBuffer::copyFrameBuffer()", "Invalid value.");
        return;
    }

    bind(FrameBufferBindType::ONLY_READ);
    target->bind(FrameBufferBindType::ONLY_WRITE);

    glReadBuffer(GL_COLOR_ATTACHMENT0 + attachment_index);
    glDrawBuffer(GL_COLOR_ATTACHMENT0 + attachment_index);

    glBlitFramebuffer(0, 0, width_, height_, 0, 0, width_, height_,
        GL_COLOR_BUFFER_BIT, GL_NEAREST);

    target->unbind();
}

void FrameBuffer::addRenderBuffer(GLboolean multisample) {
    if (render_buffer_ != nullptr) {
        return;
    }

    render_buffer_.reset(new RenderBuffer(width_, height_, multisample));

    bind(FrameBufferBindType::NORMAL);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
        GL_RENDERBUFFER, render_buffer_->getHandle());
}

GLboolean FrameBuffer::isComplete() {
    bind(FrameBufferBindType::NORMAL);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        return false;
    }

    return true;
}

void FrameBuffer::addTextureBuffer(GLushort index, GLboolean multisample,
    GLboolean float_buffer) {
    if (index < texture_buffers_.size() &&
        texture_buffers_[index] != nullptr) {
        return;
    }

    TextureBufferPtr buffer(new TextureBuffer(width_, height_, multisample,
        float_buffer));

    bind(FrameBufferBindType::NORMAL);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index,
        multisample ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D,
        buffer->getHandle(), 0);

    texture_buffers_.push_back(buffer);

    std::vector<GLuint> attachments;
    for (GLushort i = 0; i < texture_buffers_.size(); i++) {
        attachments.push_back(GL_COLOR_ATTACHMENT0 + i);
    }

    glDrawBuffers(texture_buffers_.size(), attachments.data());
}