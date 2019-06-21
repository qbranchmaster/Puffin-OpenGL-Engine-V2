/*
 * Puffin OpenGL Engine ver. 2.1
 * Coded by: Sebastian 'qbranchmaster' Tabaka
 * Contact: sebastian.tabaka@outlook.com
 */

#ifndef PUFFIN_DEFAULT_SKYBOX_RENDERER_HPP
#define PUFFIN_DEFAULT_SKYBOX_RENDERER_HPP

#include "PuffinEngine/AlphaBlend.hpp"
#include "PuffinEngine/Camera.hpp"
#include "PuffinEngine/DepthTest.hpp"
#include "PuffinEngine/FaceCull.hpp"
#include "PuffinEngine/RenderSettings.hpp"
#include "PuffinEngine/ShaderProgram.hpp"
#include "PuffinEngine/SkyboxRenderer.hpp"

namespace puffin {
    class DefaultSkyboxRenderer : public SkyboxRenderer {
    public:
        DefaultSkyboxRenderer(RenderSettingsPtr render_settings, CameraPtr camera);

        void render(FrameBufferPtr frame_buffer, ScenePtr scene);

    private:
        void loadShaders();
        void setDefaultShaderUniforms(SkyboxPtr skybox);
        void drawSkybox(SkyboxPtr skybox);

        CameraPtr camera_{nullptr};
        RenderSettingsPtr render_settings_{nullptr};
        ShaderProgramPtr default_shader_program_{nullptr};
    };

    using DefaultSkyboxRendererPtr = std::shared_ptr<DefaultSkyboxRenderer>;
} // namespace puffin

#endif // PUFFIN_DEFAULT_SKYBOX_RENDERER_HPP