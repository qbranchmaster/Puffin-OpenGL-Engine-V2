/*
 * Puffin OpenGL Engine ver. 2.1
 * Coded by: Sebastian 'qbranchmaster' Tabaka
 * Contact: sebastian.tabaka@outlook.com
 */

#ifndef PUFFIN_GIZMO_RENDERER_HPP
#define PUFFIN_GIZMO_RENDERER_HPP

#include "PuffinEngine/FrameBuffer.hpp"
#include "PuffinEngine/Renderer.hpp"

namespace puffin {
    class GizmoRenderer : public Renderer {
    public:
        virtual void render(FrameBufferPtr frame_buffer) = 0;
    };

    using GizmoRendererPtr = std::shared_ptr<GizmoRenderer>;
} // namespace puffin

#endif // PUFFIN_GIZMO_RENDERER_HPP