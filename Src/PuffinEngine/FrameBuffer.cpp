/*
 * Puffin OpenGL Engine ver. 2.0.1
 * Coded by: Sebastian 'qbranchmaster' Tabaka
 * Contact: sebastian.tabaka@outlook.com
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

void FrameBuffer::setClearColor(const glm::vec3 &color) {
    clear_color_ = glm::vec3(glm::clamp(color.r, 0.0f, 1.0f), glm::clamp(color.g, 0.0f, 1.0f),
        glm::clamp(color.b, 0.0f, 1.0f));
}

void FrameBuffer::copyFrameBuffer(
    FrameBufferPtr target, GLushort attachment_index, GLboolean copy_depth) {
    if (!target) {
        logError("FrameBuffer::copyFrameBuffer()", PUFFIN_MSG_NULL_OBJECT);
        return;
    }

    if (attachment_index >= texture_buffers_.size()) {
        logError("FrameBuffer::copyFrameBuffer()", PUFFIN_MSG_INVALID_VALUE);
        return;
    }

    bind(FrameBufferBindType::OnlyRead);
    target->bind(FrameBufferBindType::OnlyWrite);

    glReadBuffer(GL_COLOR_ATTACHMENT0 + attachment_index);
    glDrawBuffer(GL_COLOR_ATTACHMENT0 + attachment_index);

    GLuint copy_bits = GL_COLOR_BUFFER_BIT;
    if (copy_depth) {
        copy_bits |= GL_DEPTH_BUFFER_BIT;
    }

    glBlitFramebuffer(0, 0, width_, height_, 0, 0, width_, height_, copy_bits, GL_NEAREST);
}

void FrameBuffer::addRenderBuffer(GLboolean multisample) {
    if (render_buffer_ != nullptr) {
        return;
    }

    render_buffer_.reset(new RenderBuffer(width_, height_, multisample));

    bind(FrameBufferBindType::Normal);
    glFramebufferRenderbuffer(
        GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, render_buffer_->getHandle());
}

GLboolean FrameBuffer::isComplete() {
    bind(FrameBufferBindType::Normal);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        return false;
    }

    return true;
}

void FrameBuffer::addTextureBuffer(GLushort index, GLboolean multisample, GLboolean float_buffer) {
    if (index < texture_buffers_.size() && texture_buffers_[index] != nullptr) {
        return;
    }

    TextureBufferPtr buffer(new TextureBuffer(width_, height_, multisample, float_buffer));

    bind(FrameBufferBindType::Normal);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index,
        multisample ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D, buffer->getHandle(), 0);

    texture_buffers_.push_back(buffer);

    std::vector<GLuint> attachments;
    for (GLushort i = 0; i < texture_buffers_.size(); i++) {
        attachments.push_back(GL_COLOR_ATTACHMENT0 + i);
    }

    glDrawBuffers(texture_buffers_.size(), attachments.data());
}

void FrameBuffer::addDepthTextureBuffer(GLboolean multisample, GLboolean float_buffer) {
    if (depth_texture_buffer_) {
        return;
    }

    depth_texture_buffer_.reset(new DepthTextureBuffer(width_, height_, multisample, float_buffer));

    bind(FrameBufferBindType::Normal);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
        multisample ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D, depth_texture_buffer_->getHandle(),
        0);
}

void FrameBuffer::addCubeTextureBuffer() {
    cube_texture_buffer_.reset(new CubeTextureBuffer(width_, height_));

    bind(FrameBufferBindType::Normal);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, cube_texture_buffer_->getHandle(), 0);
}

void FrameBuffer::disableDrawBuffer() {
    bind(FrameBufferBindType::Normal);
    glDrawBuffer(GL_NONE);
}

void FrameBuffer::disableReadBuffer() {
    bind(FrameBufferBindType::Normal);
    glReadBuffer(GL_NONE);
}

void FrameBuffer::clear(FrameBufferClearType clear_type, const glm::vec3 &color) {
    glClearColor(color.r, color.g, color.b, 1.0f);

    switch (clear_type) {
    case FrameBufferClearType::OnlyColor:
        glClear(GL_COLOR_BUFFER_BIT);
        break;
    case FrameBufferClearType::OnlyDepth:
        glClear(GL_DEPTH_BUFFER_BIT);
        break;
    case FrameBufferClearType::DepthAndColor:
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        break;
    }
}

void FrameBuffer::setViewportSize(GLuint width, GLuint height) {
    if (width == 0 || height == 0) {
        logError("FrameBuffer::setViewportSize()", PUFFIN_MSG_INVALID_VALUE);
        return;
    }

    glViewport(0, 0, width, height);
}

void FrameBuffer::setViewportSize(FrameBufferPtr frame_buffer) {
    if (!frame_buffer) {
        logError("FrameBuffer::setViewportSize()", PUFFIN_MSG_NULL_OBJECT);
        return;
    }

    FrameBuffer::setViewportSize(frame_buffer->getWidth(), frame_buffer->getHeight());
}