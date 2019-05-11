/*
* Puffin OpenGL Engine ver. 2.0
* Coded by: Sebastian 'qbranchmaster' Tabaka
* Contact: sebastian.tabaka@outlook.com
*/

#ifndef PUFFIN_DEFAULT_WATER_RENDERER_HPP
#define PUFFIN_DEFAULT_WATER_RENDERER_HPP

#include "PuffinEngine/Camera.hpp"
#include "PuffinEngine/RenderSettings.hpp"
#include "PuffinEngine/ShaderProgram.hpp"
#include "PuffinEngine/WaterRenderer.hpp"

namespace puffin {
    class DefaultWaterRenderer : public WaterRenderer {
    public:
        DefaultWaterRenderer(RenderSettingsPtr render_settings, CameraPtr camera);

        void render(FrameBufferPtr frame_buffer, ScenePtr scene);

    private:
        void loadShaders();
        void setShadersUniforms(WaterTilePtr water_tile);
        void drawWaterTile(WaterTilePtr water_tile);

        CameraPtr camera_{nullptr};
        RenderSettingsPtr render_settings_{nullptr};
        ShaderProgramPtr default_shader_program_{nullptr};
    };

    using DefaultWaterRendererPtr = std::shared_ptr<DefaultWaterRenderer>;
} // namespace puffin

#endif // PUFFIN_DEFAULT_WATER_RENDERER_HPP