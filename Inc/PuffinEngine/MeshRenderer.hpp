/*
 * Puffin OpenGL Engine ver. 2.0.1
 * Coded by: Sebastian 'qbranchmaster' Tabaka
 * Contact: sebastian.tabaka@outlook.com
 */

#ifndef PUFFIN_MESH_RENDERER_HPP
#define PUFFIN_MESH_RENDERER_HPP

#include "PuffinEngine/FrameBuffer.hpp"
#include "PuffinEngine/Renderer.hpp"
#include "PuffinEngine/Scene.hpp"

namespace puffin {
    class MeshRenderer : public Renderer {
    public:
        virtual void render(FrameBufferPtr frame_buffer, ScenePtr scene) = 0;
    };

    using MeshRendererPtr = std::shared_ptr<MeshRenderer>;
} // namespace puffin

#endif // PUFFIN_MESH_RENDERER_HPP