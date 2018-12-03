/*
* Puffin OpenGL Engine ver. 2.0
* Created by: Sebastian 'qbranchmaster' Tabaka
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

void FrameBuffer::addTextureBuffer(GLuint width, GLuint height) {
    if (!rgb_buffer_) {
        rgb_buffer_.reset(new Texture());
    }

    rgb_buffer_->createTextureBuffer(width, height);

    bind();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
        rgb_buffer_->getHandle(), 0);
    unbind();
}

void FrameBuffer::addRenderBuffer(GLuint width, GLuint height) {
    if (!depth_buffer_) {
        depth_buffer_.reset(new RenderBuffer());
    }

    depth_buffer_->create(width, height);

    bind();
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
        GL_RENDERBUFFER, depth_buffer_->getHandle());
    unbind();
}

GLboolean FrameBuffer::isComplete() const {
    bind();
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        return false;
    }

    unbind();
    return true;
}