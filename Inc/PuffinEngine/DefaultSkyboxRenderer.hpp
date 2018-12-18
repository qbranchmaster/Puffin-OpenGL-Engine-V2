/*
* Puffin OpenGL Engine ver. 2.0
* Coded by: Sebastian 'qbranchmaster' Tabaka
*/

#ifndef PUFFIN_DEFAULT_SKYBOX_RENDERER_HPP
#define PUFFIN_DEFAULT_SKYBOX_RENDERER_HPP

#include "PuffinEngine/Camera.hpp"
#include "PuffinEngine/DepthTest.hpp"
#include "PuffinEngine/RenderSettings.hpp"
#include "PuffinEngine/ShaderProgram.hpp"
#include "PuffinEngine/SkyboxRenderer.hpp"

namespace puffin {
    class DefaultSkyboxRenderer : public SkyboxRenderer {
    public:
        DefaultSkyboxRenderer(RenderSettingsPtr render_settings,
            CameraPtr camera);

        void render(FrameBufferPtr frame_buffer, SkyboxPtr skybox);

    private:
        void loadShaders();
        void setShadersUniforms(SkyboxPtr skybox);
        void drawSkybox(SkyboxPtr skybox);

        CameraPtr camera_;
        RenderSettingsPtr render_settings_;
        ShaderProgramPtr default_shader_program_;
    };

    using DefaultSkyboxRendererPtr = std::shared_ptr<DefaultSkyboxRenderer>;
} // namespace puffin

#endif // PUFFIN_DEFAULT_SKYBOX_RENDERER_HPP