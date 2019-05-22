/*
* Puffin OpenGL Engine ver. 2.0
* Coded by: Sebastian 'qbranchmaster' Tabaka
* Contact: sebastian.tabaka@outlook.com
*/

#ifndef PUFFIN_DEFAULT_MESH_RENDERER_HPP
#define PUFFIN_DEFAULT_MESH_RENDERER_HPP

#include "PuffinEngine/AlphaBlend.hpp"
#include "PuffinEngine/Camera.hpp"
#include "PuffinEngine/DepthTest.hpp"
#include "PuffinEngine/FaceCull.hpp"
#include "PuffinEngine/MeshRenderer.hpp"
#include "PuffinEngine/RenderSettings.hpp"
#include "PuffinEngine/ShaderProgram.hpp"
#include "PuffinEngine/DefaultShadowMapRenderer.hpp"

namespace puffin {
    class DefaultMeshRenderer : public MeshRenderer {
        friend class DefaultWaterRenderer;

    public:
        DefaultMeshRenderer(RenderSettingsPtr render_settings, CameraPtr camera,
            DefaultShadowMapRendererPtr shadow_map_renderer);

        void render(FrameBufferPtr frame_buffer, ScenePtr scene);

        void setClippingPlane(const glm::vec4 &plane) {
            clipping_plane_ = plane;
        }

        glm::vec4 getClippingPlane() const {
            return clipping_plane_;
        }

        void enableClippingPlane(GLboolean state) {
            clipping_plane_enabled_ = state;
        }

    private:
        void loadShaders();
        void setShadersUniforms(MeshPtr mesh);
        void setMeshEntityShadersUniforms(MeshEntityPtr entity);
        void drawMesh(MeshPtr mesh, GLuint entity_index);

        GLboolean clipping_plane_enabled_{false};
        glm::vec4 clipping_plane_{0.0f, -1.0f, 0.0f, 1.0f};

        CameraPtr camera_{nullptr};
        RenderSettingsPtr render_settings_{nullptr};
        ShaderProgramPtr default_shader_program_{nullptr};

        DefaultShadowMapRendererPtr shadow_map_renderer_{nullptr};
    };

    using DefaultMeshRendererPtr = std::shared_ptr<DefaultMeshRenderer>;
} // namespace puffin

#endif // PUFFIN_DEFAULT_MESH_RENDERER_HPP