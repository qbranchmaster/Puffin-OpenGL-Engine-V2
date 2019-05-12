/*
* Puffin OpenGL Engine ver. 2.0
* Coded by: Sebastian 'qbranchmaster' Tabaka
* Contact: sebastian.tabaka@outlook.com
*/

#ifndef PUFFIN_SKYBOX_RENDERER_HPP
#define PUFFIN_SKYBOX_RENDERER_HPP

#include "PuffinEngine/FrameBuffer.hpp"
#include "PuffinEngine/Renderer.hpp"
#include "PuffinEngine/Scene.hpp"

namespace puffin {
    class SkyboxRenderer : public Renderer {
    public:
        virtual void render(FrameBufferPtr frame_buffer, ScenePtr scene) = 0;
    };

    using SkyboxRendererPtr = std::shared_ptr<SkyboxRenderer>;
} // namespace puffin

#endif // PUFFIN_SKYBOX_RENDERER_HPP