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
#include "PuffinEngine/ShaderProgram.hpp"
#include "PuffinEngine/SkyboxRenderer.hpp"
#include "Postprocess.hpp"

namespace puffin {
    class DefaultSkyboxRenderer : public SkyboxRenderer {
    public:
        DefaultSkyboxRenderer(PostprocessPtr postprocess);

        void render(FrameBufferPtr frame_buffer, ScenePtr scene);

    private:
        void loadShaders();
        void setDefaultShaderUniforms(ScenePtr scene);
        void drawSkybox(SkyboxPtr skybox);

        ShaderProgramPtr default_shader_program_{nullptr};

		PostprocessPtr postprocess_;
    };

    using DefaultSkyboxRendererPtr = std::shared_ptr<DefaultSkyboxRenderer>;
} // namespace puffin

#endif // PUFFIN_DEFAULT_SKYBOX_RENDERER_HPP