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
    default_shader_program_->loadShaders("Data/Shaders/Skybox.vert",
        "Data/Shaders/Skybox.frag");
}

void DefaultSkyboxRenderer::render(FrameBufferPtr frame_buffer,
    SkyboxPtr skybox) {
    if (!frame_buffer || !skybox) {
        logError("DefaultSkyboxRenderer::render()", "Null input.");
        return;
    }

    FrameBuffer::setViewportSize(frame_buffer);
    frame_buffer->bind(FrameBufferBindType::NORMAL);

    default_shader_program_->activate();
    setShadersUniforms(skybox);

    DepthTest::instance().enable(true);
    DepthTest::instance().enableDepthMask(false);

    auto texture = skybox->getTexture();
    Texture::setTextureSlot(0);
    if (texture) {
        texture->bind();
    }
    else {
        Texture::unbindTextureType(TextureType::TextureCube);
    }

    drawSkybox(skybox);
}

void DefaultSkyboxRenderer::setShadersUniforms(SkyboxPtr skybox) {
    if (!skybox) {
        logError("DefaultSkyboxRenderer::setShadersUniforms()", "Null input.");
        return;
    }

    // Skybox should not be affected by changing near and far plane values
    auto near_plane = camera_->getNearPlane();
    auto far_plane = camera_->getFarPlane();
    camera_->setProjection(camera_->getFov(), camera_->getAspect(),
        0.1f, 2.0f);

    default_shader_program_->setUniform("matrices.view_matrix",
        camera_->getViewMatrixStatic());
    default_shader_program_->setUniform("matrices.projection_matrix",
        camera_->getProjectionMatrix());
    default_shader_program_->setUniform("matrices.model_matrix",
        skybox->getModelMatrix());

    // Restore previous near and far plane values
    camera_->setProjection(camera_->getFov(), camera_->getAspect(),
        near_plane, far_plane);

    default_shader_program_->setUniform("color.cube_texture", 0);
    default_shader_program_->setUniform("color.light_color",
        render_settings_->lighting()->getSkyboxLightColor());
    default_shader_program_->setUniform("color.gamma",
        render_settings_->getGamma());
    default_shader_program_->setUniform("color.bloom_threshold_color",
        render_settings_->postprocess()->getGlowBloomThresholdColor());
}

void DefaultSkyboxRenderer::drawSkybox(SkyboxPtr skybox) {
    if (!skybox) {
        logError("DefaultSkyboxRenderer::drawSkybox()", "Null input.");
        return;
    }

    skybox->bind();
    glDrawArrays(GL_TRIANGLES, 0, 36);
}