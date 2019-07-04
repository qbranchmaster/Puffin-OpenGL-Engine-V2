/*
 * Puffin OpenGL Engine ver. 2.0.1
 * Coded by: Sebastian 'qbranchmaster' Tabaka
 * Contact: sebastian.tabaka@outlook.com
 */

#ifndef PUFFIN_DEFAULT_SKYBOX_RENDERER_HPP
#define PUFFIN_DEFAULT_SKYBOX_RENDERER_HPP

#include "PuffinEngine/AlphaBlend.hpp"
#include "PuffinEngine/Camera.hpp"
#include "PuffinEngine/DepthTest.hpp"
#include "PuffinEngine/FaceCull.hpp"
#include "PuffinEngine/Postprocess.hpp"
#include "PuffinEngine/ShaderProgram.hpp"
#include "PuffinEngine/SkyboxRenderer.hpp"

namespace puffin {
    class DefaultSkyboxRenderer : public SkyboxRenderer {
    public:
        explicit DefaultSkyboxRenderer(PostprocessPtr postprocess);

        void render(FrameBufferPtr frame_buffer, ScenePtr scene);

    private:
        void loadShaders();
        void setDefaultShaderUniforms(
            SkyboxPtr skybox, CameraPtr camera, LightingPtr lighting, FogPtr fog);
        void drawSkybox(SkyboxPtr skybox);

        ShaderProgramPtr default_shader_program_{nullptr};

        PostprocessPtr postprocess_{nullptr};
    };

    using DefaultSkyboxRendererPtr = std::shared_ptr<DefaultSkyboxRenderer>;
} // namespace puffin

#endif // PUFFIN_DEFAULT_SKYBOX_RENDERER_HPP