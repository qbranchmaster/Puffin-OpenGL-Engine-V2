/*
 * Puffin OpenGL Engine ver. 2.0.1
 * Coded by: Sebastian 'qbranchmaster' Tabaka
 * Contact: sebastian.tabaka@outlook.com
 */

#ifndef PUFFIN_DEFAULT_GIZMO_RENDERER_HPP
#define PUFFIN_DEFAULT_GIZMO_RENDERER_HPP

#include "PuffinEngine/AlphaBlend.hpp"
#include "PuffinEngine/Camera.hpp"
#include "PuffinEngine/DepthTest.hpp"
#include "PuffinEngine/FaceCull.hpp"
#include "PuffinEngine/FrameBuffer.hpp"
#include "PuffinEngine/GizmoRenderer.hpp"
#include "PuffinEngine/Mesh.hpp"
#include "PuffinEngine/ShaderProgram.hpp"

namespace puffin {
    class DefaultGizmoRenderer : public GizmoRenderer {
    public:
        DefaultGizmoRenderer();

        void render(ScenePtr scene);

        void setGizmosScale(GLushort scale);

        GLushort getGizmosScale() const {
            return gizmo_scale_;
        }

    private:
        void loadShaders();
        void loadTextures();
        void createGizmoMesh();
        void drawGizmoMesh();
        void setDefaultShaderProgramUniforms(CameraPtr camera);

        glm::mat4 prepareGizmoModelMatrix(const glm::vec3 &position, CameraPtr camera);

        void renderPointLightsGizmos(ScenePtr scene);

        GLushort gizmo_scale_{1};

        TexturePtr point_light_texture_{nullptr};

        MeshPtr gizmo_mesh_{nullptr};
        ShaderProgramPtr default_shader_program_{nullptr};
    };

    using DefaultGizmoRendererPtr = std::shared_ptr<DefaultGizmoRenderer>;
} // namespace puffin

#endif // PUFFIN_DEFAULT_GIZMO_RENDERER_HPP