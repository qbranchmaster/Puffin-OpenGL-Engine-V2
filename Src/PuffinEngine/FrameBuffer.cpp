/*
* Puffin OpenGL Engine
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

GLboolean FrameBuffer::isComplete() {
    bind();
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        return false;
    }

    return true;
}