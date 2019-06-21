/*
 * Puffin OpenGL Engine ver. 2.1
 * Coded by: Sebastian 'qbranchmaster' Tabaka
 * Contact: sebastian.tabaka@outlook.com
 */

#ifndef PUFFIN_DEFAULT_WATER_RENDERER_HPP
#define PUFFIN_DEFAULT_WATER_RENDERER_HPP

#include "PuffinEngine/Camera.hpp"
#include "PuffinEngine/DefaultMeshRenderer.hpp"
#include "PuffinEngine/DefaultSkyboxRenderer.hpp"
#include "PuffinEngine/InitConfig.hpp"
#include "PuffinEngine/RenderSettings.hpp"
#include "PuffinEngine/ShaderProgram.hpp"
#include "PuffinEngine/Texture.hpp"
#include "PuffinEngine/WaterRenderer.hpp"

namespace puffin {
    class DefaultWaterRenderer : public WaterRenderer {
    public:
        DefaultWaterRenderer(RenderSettingsPtr render_settings, CameraPtr camera,
            DefaultMeshRendererPtr mesh_renderer, DefaultSkyboxRendererPtr skybox_renderer);

        void render(FrameBufferPtr frame_buffer, ScenePtr scene);

        void setTextureTiling(GLushort tiling);

        GLushort getTextureTiling() const {
            return texture_tiling_;
        }

    private:
        void loadShaders();
        void setDefaultShaderUniforms(WaterTilePtr water_tile);
        void setWireframeShaderUniforms(WaterTilePtr water_tile);

        void renderNormal(FrameBufferPtr frame_buffer, ScenePtr scene);
        void renderWireframe(FrameBufferPtr frame_buffer, ScenePtr scene);

        void createFrameBuffers();
        void renderToReflectionFrameBuffer(WaterTilePtr water_tile, ScenePtr scene);
        void renderToRefractionFrameBuffer(WaterTilePtr water_tile, ScenePtr scene);
        void drawWaterTile(WaterTilePtr water_tile);

        CameraPtr camera_{nullptr};
        DefaultMeshRendererPtr mesh_renderer_{nullptr};
        DefaultSkyboxRendererPtr skybox_renderer_{nullptr};
        RenderSettingsPtr render_settings_{nullptr};
        ShaderProgramPtr default_shader_program_{nullptr};
        ShaderProgramPtr wireframe_shader_program_{nullptr};

        FrameBufferPtr reflection_frame_buffer_{nullptr};
        FrameBufferPtr refraction_frame_buffer_{nullptr};

        std::string dudv_map_path_;
        TexturePtr dudv_map_{nullptr};
        std::string normal_map_path_;
        TexturePtr normal_map_{nullptr};
        GLushort texture_tiling_{1};
    };

    using DefaultWaterRendererPtr = std::shared_ptr<DefaultWaterRenderer>;
} // namespace puffin

#endif // PUFFIN_DEFAULT_WATER_RENDERER_HPP