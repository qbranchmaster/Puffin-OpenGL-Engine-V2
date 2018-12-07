/*
* Puffin OpenGL Engine ver. 2.0
* Created by: Sebastian 'qbranchmaster' Tabaka
*/

#ifndef PUFFIN_SKYBOX_RENDERER_HPP
#define PUFFIN_SKYBOX_RENDERER_HPP

#include <memory>

#include "PuffinEngine/Camera.hpp"
#include "PuffinEngine/RenderSettings.hpp"
#include "PuffinEngine/ShaderProgram.hpp"
#include "PuffinEngine/Skybox.hpp"

namespace puffin {
    class SkyboxRenderer {
    public:
        SkyboxRenderer(RenderSettingsPtr render_settings, CameraPtr camera);

        void render(SkyboxPtr skybox);

        void setFilterColor(const glm::vec3 &color);

        glm::vec3 getFilterColor() const {
            return filter_color_;
        }

    private:
        void drawSkybox(SkyboxPtr skybox);

        void loadShaders();
        void setShadersUniforms(SkyboxPtr skybox);

        glm::vec3 filter_color_{1.0f, 1.0f, 1.0f};

        GLboolean colors_changed_{true};

        CameraPtr camera_;
        RenderSettingsPtr render_settings_;
        ShaderProgramPtr default_shader_program_;
    };

    using SkyboxRendererPtr = std::shared_ptr<SkyboxRenderer>;
} // namespace puffin

#endif // PUFFIN_SKYBOX_RENDERER_HPP