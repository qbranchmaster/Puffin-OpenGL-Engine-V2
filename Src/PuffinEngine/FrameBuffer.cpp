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
    if (!rgb_texture_buffer_handle_) {
        glGenTextures(1, &rgb_texture_buffer_handle_);
    }

    glBindTexture(GL_TEXTURE_2D, rgb_texture_buffer_handle_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
        GL_UNSIGNED_BYTE, nullptr);

    // TODO: Check nearest.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);

    bind();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
        rgb_texture_buffer_handle_, 0);
    unbind();
}

void FrameBuffer::addRenderBuffer(GLuint width, GLuint height) {
    if (!depth_render_buffer_handle_) {
        glGenRenderbuffers(1, &depth_render_buffer_handle_);
    }

    glBindRenderbuffer(GL_RENDERBUFFER, depth_render_buffer_handle_);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    bind();
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
        GL_RENDERBUFFER, depth_render_buffer_handle_);
    unbind();
}

GLboolean FrameBuffer::isComplete() const {
    bind();
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        return false;
    }

    return true;
}