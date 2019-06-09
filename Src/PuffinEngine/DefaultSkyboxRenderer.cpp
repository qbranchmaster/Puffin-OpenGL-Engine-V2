/*
* Puffin OpenGL Engine ver. 2.0
* Coded by: Sebastian 'qbranchmaster' Tabaka
* Contact: sebastian.tabaka@outlook.com
*/

#include "PuffinEngine/DefaultSkyboxRenderer.hpp"

using namespace puffin;

DefaultSkyboxRenderer::DefaultSkyboxRenderer(RenderSettingsPtr render_settings, CameraPtr camera) {
    if (!render_settings || !camera) {
        throw Exception("DefaultSkyboxRenderer::DefaultSkyboxRenderer()", PUFFIN_MSG_NULL_OBJECT);
    }

    render_settings_ = render_settings;
    camera_ = camera;

    loadShaders();
}

void DefaultSkyboxRenderer::loadShaders() {
    default_shader_program_.reset(new ShaderProgram("skybox_shader_program"));
    default_shader_program_->loadShaders("Data/Shaders/Skybox.vert", "Data/Shaders/Skybox.frag");
}

void DefaultSkyboxRenderer::drawSkybox(SkyboxPtr skybox) {
    if (!skybox) {
        return;
    }

    skybox->bind();
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void DefaultSkyboxRenderer::render(FrameBufferPtr frame_buffer, ScenePtr scene) {
    if (!frame_buffer || !scene) {
        logError("DefaultSkyboxRenderer::render()", PUFFIN_MSG_NULL_OBJECT);
        return;
    }

    // Prepare rendering process
    frame_buffer->bind(FrameBufferBindType::Normal);
    FrameBuffer::setViewportSize(frame_buffer);

    AlphaBlend::instance().enable(false);
    DepthTest::instance().enable(true);
    DepthTest::instance().enableDepthMask(false);
    FaceCull::instance().enable(true);
    FaceCull::instance().setCulledSide(CulledSide::Back);

    // Get skybox from scene and check if it is valid
    auto skybox = scene->getSkybox();
    if (!skybox) {
        return;
    }

    default_shader_program_->activate();
    setShadersUniforms(skybox);

    Texture::setTextureSlot(0);
    auto texture = skybox->getTexture();
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
        logError("DefaultSkyboxRenderer::setShadersUniforms()", PUFFIN_MSG_NULL_OBJECT);
        return;
    }

    // Skybox should not be affected by changing near and far plane values
    auto near_plane = camera_->getNearPlane();
    auto far_plane = camera_->getFarPlane();
    camera_->setProjection(camera_->getFov(), camera_->getAspect(), 0.5f, 2.5f);

    default_shader_program_->setUniform("matrices.view_matrix", camera_->getViewMatrixStatic());
    default_shader_program_->setUniform("matrices.projection_matrix",
        camera_->getProjectionMatrix());
    default_shader_program_->setUniform("matrices.model_matrix", skybox->getModelMatrix());

    // Restore previous near and far plane values
    camera_->setProjection(camera_->getFov(), camera_->getAspect(), near_plane, far_plane);

    default_shader_program_->setUniform("color.cube_texture", 0);
    default_shader_program_->setUniform("color.light_color", render_settings_->lighting()->
        getSkyboxLightColor());
    default_shader_program_->setUniform("color.gamma", render_settings_->postprocess()->getGamma());
    default_shader_program_->setUniform("color.bloom_threshold_color",
        render_settings_->postprocess()->getGlowBloomThresholdColor());
}