/*
* Puffin OpenGL Engine ver. 2.0
* Coded by: Sebastian 'qbranchmaster' Tabaka
*/

#ifndef PUFFIN_SKYBOX_RENDERER_HPP
#define PUFFIN_SKYBOX_RENDERER_HPP

#include "PuffinEngine/FrameBuffer.hpp"
#include "PuffinEngine/Renderer.hpp"
#include "PuffinEngine/Skybox.hpp"

namespace puffin {
    class SkyboxRenderer : public Renderer {
    public:
        virtual void render(FrameBufferPtr frame_buffer, SkyboxPtr skybox) = 0;
    };

    using SkyboxRendererPtr = std::shared_ptr<SkyboxRenderer>;
} // namespace puffin

#endif // PUFFIN_SKYBOX_RENDERER_HPP