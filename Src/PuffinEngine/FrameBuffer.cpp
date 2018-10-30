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