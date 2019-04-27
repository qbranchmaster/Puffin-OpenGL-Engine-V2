/*
* Puffin OpenGL Engine ver. 2.0
* Coded by: Sebastian 'qbranchmaster' Tabaka
*/

#ifndef PUFFIN_DEFAULT_MESH_RENDERER_HPP
#define PUFFIN_DEFAULT_MESH_RENDERER_HPP

#include "PuffinEngine/AlphaBlend.hpp"
#include "PuffinEngine/Camera.hpp"
#include "PuffinEngine/DepthTest.hpp"
#include "PuffinEngine/FaceCull.hpp"
#include "PuffinEngine/MeshRenderer.hpp"
#include "PuffinEngine/RenderersSharedData.hpp"
#include "PuffinEngine/RenderSettings.hpp"
#include "PuffinEngine/ShaderProgram.hpp"

namespace puffin {
    class DefaultMeshRenderer : public MeshRenderer {
    public:
        DefaultMeshRenderer(RenderSettingsPtr render_settings,
            RenderersSharedDataPtr shared_data, CameraPtr camera);

        void render(FrameBufferPtr frame_buffer, MeshPtr mesh);

    private:
        void loadShaders();
        void setShadersUniforms(MeshPtr mesh);
        void setMeshEntityShadersUniforms(MeshEntityPtr entity);
        void drawMesh(MeshPtr mesh, GLuint entity_index);

        CameraPtr camera_;
        RenderSettingsPtr render_settings_;
        RenderersSharedDataPtr shared_data_;
        ShaderProgramPtr default_shader_program_;
    };

    using DefaultMeshRendererPtr = std::shared_ptr<DefaultMeshRenderer>;
} // namespace puffin

#endif // PUFFIN_DEFAULT_MESH_RENDERER_HPP