/*
* Puffin OpenGL Engine ver. 2.0
* Created by: Sebastian 'qbranchmaster' Tabaka
*/

#include "PuffinEngine/SkyboxRenderer.hpp"

#include "PuffinEngine/Exception.hpp"
#include "PuffinEngine/Logger.hpp"
#include "PuffinEngine/Texture.hpp"

using namespace puffin;

SkyboxRenderer::SkyboxRenderer(RenderSettingsPtr render_settings,
    CameraPtr camera) {
    if (!render_settings || !camera) {
        throw Exception("SkyboxRenderer::SkyboxRenderer()",
            "Not initialized object.");
    }

    render_settings_ = render_settings;
    camera_ = camera;

    loadShaders();
}

void SkyboxRenderer::render(SkyboxPtr skybox) {
    if (!skybox) {
        return;
    }

    // Configure shader.
    default_shader_program_->bind();
    setShadersUniforms(skybox);

    // Configure render parameters.
    render_settings_->depthTest()->enable(true);
    render_settings_->depthTest()->enableDepthMask(false);

    // Bind texture.
    auto texture = skybox->getTexture();
    Texture::setTextureSlot(0);
    if (texture) {
        texture->bind();
    }
    else {
        Texture::unbindAllTextures(TextureType::TextureCube);
    }

    // Draw.
    skybox->bind();
    skybox->draw(0);
}

void SkyboxRenderer::loadShaders() {
    default_shader_program_.reset(new ShaderProgram());
    default_shader_program_->loadShaders("Shaders/Skybox.vert",
        "Shaders/Skybox.frag");
}

void SkyboxRenderer::setShadersUniforms(SkyboxPtr skybox) {
    if (!skybox) {
        return;
    }

    // Camera uniforms.
    default_shader_program_->setUniform("matrices.view_matrix",
        camera_->getViewMatrixStatic());
    default_shader_program_->setUniform("matrices.projection_matrix",
        camera_->getProjectionMatrix());
    default_shader_program_->setUniform("matrices.model_matrix",
        skybox->getModelMatrix());

    if (colors_changed_) {
        default_shader_program_->setUniform("color.filter_color",
            filter_color_);
        default_shader_program_->setUniform("color.cube_texture", 0);

        colors_changed_ = false;
    }
}

void SkyboxRenderer::setFilterColor(const glm::vec3 &color) {
    filter_color_ = color;
    colors_changed_ = true;
}