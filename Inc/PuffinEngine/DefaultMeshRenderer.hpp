/*
 * Puffin OpenGL Engine ver. 2.1
 * Coded by: Sebastian 'qbranchmaster' Tabaka
 * Contact: sebastian.tabaka@outlook.com
 */

#ifndef PUFFIN_DEFAULT_MESH_RENDERER_HPP
#define PUFFIN_DEFAULT_MESH_RENDERER_HPP

#include "PuffinEngine/AlphaBlend.hpp"
#include "PuffinEngine/Camera.hpp"
#include "PuffinEngine/DefaultShadowMapRenderer.hpp"
#include "PuffinEngine/DepthTest.hpp"
#include "PuffinEngine/FaceCull.hpp"
#include "PuffinEngine/MeshRenderer.hpp"
#include "PuffinEngine/ShaderProgram.hpp"
#include "PuffinEngine/Postprocess.hpp"

namespace puffin {
    class DefaultMeshRenderer : public MeshRenderer {
        friend class DefaultWaterRenderer;

    public:
        DefaultMeshRenderer(DefaultShadowMapRendererPtr shadow_map_renderer, PostprocessPtr postprocess);

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
        void setDefaultShaderUniforms(ScenePtr scene);
        void setDefaultShaderMeshUniforms(MeshPtr mesh, MeshEntityPtr entity);
        void setWireframeShaderUniforms(CameraPtr camera);
        void setWireframeShaderMeshUniforms(MeshPtr mesh, MeshEntityPtr entity);
        void renderMeshEntity(MeshPtr mesh, GLuint entity_index);
        void drawMeshEntity(MeshEntityPtr entity);

        void renderNormal(FrameBufferPtr frame_buffer, ScenePtr scene);
        void renderWireframe(FrameBufferPtr frame_buffer, ScenePtr scene);

        GLboolean clipping_plane_enabled_{false};
        glm::vec4 clipping_plane_{0.0f, -1.0f, 0.0f, 1.0f};

        ShaderProgramPtr default_shader_program_{nullptr};
        ShaderProgramPtr wireframe_shader_program_{nullptr};

        DefaultShadowMapRendererPtr shadow_map_renderer_{nullptr};

		PostprocessPtr postprocess_{nullptr};
    };

    using DefaultMeshRendererPtr = std::shared_ptr<DefaultMeshRenderer>;
} // namespace puffin

#endif // PUFFIN_DEFAULT_MESH_RENDERER_HPP