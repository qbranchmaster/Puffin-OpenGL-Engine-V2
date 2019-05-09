/*
* Puffin OpenGL Engine ver. 2.0
* Coded by: Sebastian 'qbranchmaster' Tabaka
* Contact: sebastian.tabaka@outlook.com
*/

#ifndef PUFFIN_POSTPROCESS_RENDERER_HPP
#define PUFFIN_POSTPROCESS_RENDERER_HPP

#include "PuffinEngine/FrameBuffer.hpp"
#include "PuffinEngine/Renderer.hpp"

namespace puffin {
    class PostprocessRenderer : public Renderer {
    public:
        virtual void render(FrameBufferPtr frame_buffer) = 0;
    };

    using PostprocessRendererPtr = std::shared_ptr<PostprocessRenderer>;
} // namespace puffin

#endif // PUFFIN_POSTPROCESS_RENDERER_HPP