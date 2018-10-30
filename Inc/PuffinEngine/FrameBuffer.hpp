#ifndef PUFFIN_FRAME_BUFFER_HPP
#define PUFFIN_FRAME_BUFFER_HPP

#include <GL/glew.h>

#include <memory>

namespace puffin {
    class FrameBuffer {
    public:
        FrameBuffer();
        ~FrameBuffer();

    private:
        GLuint handle_{0};
    };

    using FrameBufferPtr = std::shared_ptr<FrameBuffer>;
} // namespace puffin

#endif // PUFFIN_FRAME_BUFFER_HPP