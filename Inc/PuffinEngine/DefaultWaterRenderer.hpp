/*
 * Puffin OpenGL Engine ver. 2.0.1
 * Coded by: Sebastian 'qbranchmaster' Tabaka
 * Contact: sebastian.tabaka@outlook.com
 */

#ifndef PUFFIN_DEFAULT_WATER_RENDERER_HPP
#define PUFFIN_DEFAULT_WATER_RENDERER_HPP

#include "PuffinEngine/AlphaBlend.hpp"
#include "PuffinEngine/Camera.hpp"
#include "PuffinEngine/DefaultMeshRenderer.hpp"
#include "PuffinEngine/DefaultSkyboxRenderer.hpp"
#include "PuffinEngine/DepthTest.hpp"
#include "PuffinEngine/FaceCull.hpp"
#include "PuffinEngine/InitConfig.hpp"
#include "PuffinEngine/Postprocess.hpp"
#include "PuffinEngine/ShaderProgram.hpp"
#include "PuffinEngine/Texture.hpp"
#include "PuffinEngine/WaterRenderer.hpp"

namespace puffin {
    class DefaultWaterRenderer : public WaterRenderer {
    public:
        DefaultWaterRenderer(PostprocessPtr postprocess, DefaultMeshRendererPtr mesh_renderer,
            DefaultSkyboxRendererPtr skybox_renderer);

        void render(FrameBufferPtr frame_buffer, ScenePtr scene);

    private:
        void loadShaders();
        void setDefaultShaderUniforms(
            WaterTilePtr water_tile, CameraPtr camera, LightingPtr lighting, FogPtr fog);
        void setWireframeShaderUniforms(WaterTilePtr water_tile, CameraPtr camera);

        void renderNormal(FrameBufferPtr frame_buffer, ScenePtr scene);
        void renderWireframe(FrameBufferPtr frame_buffer, ScenePtr scene);

        void createFrameBuffers();
        void renderToReflectionFrameBuffer(WaterTilePtr water_tile, ScenePtr scene);
        void renderToRefractionFrameBuffer(WaterTilePtr water_tile, ScenePtr scene);
        void drawWaterTile(WaterTilePtr water_tile);

        DefaultMeshRendererPtr mesh_renderer_{nullptr};
        DefaultSkyboxRendererPtr skybox_renderer_{nullptr};
        PostprocessPtr postprocess_{nullptr};

        ShaderProgramPtr default_shader_program_{nullptr};
        ShaderProgramPtr wireframe_shader_program_{nullptr};

        FrameBufferPtr reflection_frame_buffer_{nullptr};
        FrameBufferPtr refraction_frame_buffer_{nullptr};

        std::string dudv_map_path_;
        TexturePtr dudv_map_{nullptr};
        std::string normal_map_path_;
        TexturePtr normal_map_{nullptr};
    };

    using DefaultWaterRendererPtr = std::shared_ptr<DefaultWaterRenderer>;
} // namespace puffin

#endif // PUFFIN_DEFAULT_WATER_RENDERER_HPP