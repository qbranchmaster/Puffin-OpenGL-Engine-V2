/*
* Puffin OpenGL Engine ver. 2.0
* Coded by: Sebastian 'qbranchmaster' Tabaka
* Contact: sebastian.tabaka@outlook.com
*/

#ifndef PUFFIN_DEFAULT_WATER_RENDERER_HPP
#define PUFFIN_DEFAULT_WATER_RENDERER_HPP

#include "PuffinEngine/Camera.hpp"
#include "PuffinEngine/InitConfig.hpp"
#include "PuffinEngine/DefaultMeshRenderer.hpp"
#include "PuffinEngine/DefaultSkyboxRenderer.hpp"
#include "PuffinEngine/RenderSettings.hpp"
#include "PuffinEngine/ShaderProgram.hpp"
#include "PuffinEngine/WaterRenderer.hpp"

namespace puffin {
    class DefaultWaterRenderer : public WaterRenderer {
    public:
        DefaultWaterRenderer(RenderSettingsPtr render_settings, CameraPtr camera,
            DefaultMeshRendererPtr mesh_renderer, DefaultSkyboxRendererPtr skybox_renderer);

        void render(FrameBufferPtr frame_buffer, ScenePtr scene);

    private:
        void loadShaders();
        void setShadersUniforms(WaterTilePtr water_tile);
        void createFrameBuffers();
        void drawWaterTile(WaterTilePtr water_tile);
        void renderToReflectionFrameBuffer(WaterTilePtr water_tile, ScenePtr scene);
        void renderToRefractionFrameBuffer(WaterTilePtr water_tile, ScenePtr scene);

        CameraPtr camera_{nullptr};
        DefaultMeshRendererPtr mesh_renderer_{nullptr};
        DefaultSkyboxRendererPtr skybox_renderer_{nullptr};
        RenderSettingsPtr render_settings_{nullptr};
        ShaderProgramPtr default_shader_program_{nullptr};

        FrameBufferPtr reflection_frame_buffer_{nullptr};
        FrameBufferPtr refraction_frame_buffer_{nullptr};
    };

    using DefaultWaterRendererPtr = std::shared_ptr<DefaultWaterRenderer>;
} // namespace puffin

#endif // PUFFIN_DEFAULT_WATER_RENDERER_HPP