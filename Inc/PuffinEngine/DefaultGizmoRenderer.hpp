/*
 * Puffin OpenGL Engine ver. 2.1
 * Coded by: Sebastian 'qbranchmaster' Tabaka
 * Contact: sebastian.tabaka@outlook.com
 */

#ifndef PUFFIN_DEFAULT_GIZMO_RENDERER_HPP
#define PUFFIN_DEFAULT_GIZMO_RENDERER_HPP

#ifdef WIN32 // Prevents APIENTRY redefinition
#include <Windows.h>
#endif // WIN32
#include <GL/glew.h>

#include "PuffinEngine/AlphaBlend.hpp"
#include "PuffinEngine/Camera.hpp"
#include "PuffinEngine/DepthTest.hpp"
#include "PuffinEngine/FaceCull.hpp"
#include "PuffinEngine/GizmoRenderer.hpp"
#include "PuffinEngine/Mesh.hpp"
#include "PuffinEngine/ShaderProgram.hpp"

namespace puffin {
    class DefaultGizmoRenderer : public GizmoRenderer {
    public:
        DefaultGizmoRenderer();

        void render(FrameBufferPtr frame_buffer, ScenePtr scene);

    private:
        void loadShaders();
        void loadTextures();
        void createGizmoMesh();

        TexturePtr point_light_texture_{nullptr};

        MeshPtr gizmo_mesh_{nullptr};
        ShaderProgramPtr default_shader_program_{nullptr};
    };

    using DefaultGizmoRendererPtr = std::shared_ptr<DefaultGizmoRenderer>;
} // namespace puffin

#endif // PUFFIN_DEFAULT_GIZMO_RENDERER_HPP