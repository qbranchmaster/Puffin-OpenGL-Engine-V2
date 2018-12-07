/*
* Puffin OpenGL Engine ver. 2.0
* Coded by: Sebastian 'qbranchmaster' Tabaka
*/

#include "PuffinEngine/DefaultSkyboxRenderer.hpp"

using namespace puffin;

DefaultSkyboxRenderer::DefaultSkyboxRenderer(RenderSettingsPtr render_settings,
    CameraPtr camera) {
    if (!render_settings || !camera) {
        throw Exception("DefaultSkyboxRenderer::DefaultSkyboxRenderer()",
            "Not initialized object.");
    }

    render_settings_ = render_settings;
    camera_ = camera;

    loadShaders();
}

void DefaultSkyboxRenderer::loadShaders() {
    default_shader_program_.reset(new ShaderProgram());
    default_shader_program_->loadShaders("Shaders/Skybox.vert",
        "Shaders/Skybox.frag");
}

void DefaultSkyboxRenderer::render(FrameBufferPtr frame_buffer,
    SkyboxPtr skybox) {
    if (!frame_buffer || !skybox) {
        logError("DefaultSkyboxRenderer::render()", "Null input.");
        return;
    }

    frame_buffer->bind();

    default_shader_program_->activate();
    setShadersUniforms(skybox);

    render_settings_->depthTest()->enable(true);
    render_settings_->depthTest()->enableDepthMask(false);

    auto texture = skybox->getTexture();
    Texture::setTextureSlot(0);
    if (texture) {
        texture->bind();
    }
    else {
        Texture::unbindAllTextures(TextureType::TextureCube);
    }

    drawSkybox(skybox);
}

void DefaultSkyboxRenderer::setShadersUniforms(SkyboxPtr skybox) {
    if (!skybox) {
        logError("DefaultSkyboxRenderer::setShadersUniforms()", "Null input.");
        return;
    }

    default_shader_program_->setUniform("matrices.view_matrix",
        camera_->getViewMatrixStatic());
    default_shader_program_->setUniform("matrices.projection_matrix",
        camera_->getProjectionMatrix());
    default_shader_program_->setUniform("matrices.model_matrix",
        skybox->getModelMatrix());
}

void DefaultSkyboxRenderer::drawSkybox(SkyboxPtr skybox) {
    if (!skybox) {
        logError("DefaultSkyboxRenderer::drawSkybox()", "Null input.");
        return;
    }

    skybox->bind();
    glDrawArrays(GL_TRIANGLES, 0, 36);
    skybox->unbind();
}