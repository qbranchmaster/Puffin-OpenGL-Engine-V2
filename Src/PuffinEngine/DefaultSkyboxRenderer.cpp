/*
 * Puffin OpenGL Engine ver. 2.0.1
 * Coded by: Sebastian 'qbranchmaster' Tabaka
 * Contact: sebastian.tabaka@outlook.com
 */

#include "PuffinEngine/DefaultSkyboxRenderer.hpp"

using namespace puffin;

DefaultSkyboxRenderer::DefaultSkyboxRenderer(PostprocessPtr postprocess) {
    if (!postprocess) {
        throw Exception("DefaultSkyboxRenderer::DefaultSkyboxRenderer()", PUFFIN_MSG_NULL_OBJECT);
    }

    postprocess_ = postprocess;

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

    if (!enabled_) {
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
    auto skybox = scene->getActiveSkybox();
    if (!skybox) {
        return;
    }

    default_shader_program_->activate();
    setDefaultShaderUniforms(skybox, scene->camera(), scene->lighting(), scene->fog());

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

void DefaultSkyboxRenderer::setDefaultShaderUniforms(
    SkyboxPtr skybox, CameraPtr camera, LightingPtr lighting, FogPtr fog) {
    // Skybox should not be affected by changing near and far plane values
    auto near_plane = camera->getNearPlane();
    auto far_plane = camera->getFarPlane();
    camera->setProjection(camera->getFov(), camera->getAspect(), 0.5f, 2.5f);

    default_shader_program_->setUniform("matrices.view_matrix", camera->getViewMatrixStatic());
    default_shader_program_->setUniform(
        "matrices.projection_matrix", camera->getProjectionMatrix());

    default_shader_program_->setUniform("matrices.model_matrix", skybox->getModelMatrix());

    // Restore previous near and far plane values
    camera->setProjection(camera->getFov(), camera->getAspect(), near_plane, far_plane);

    default_shader_program_->setUniform("color.cube_texture", 0);
    default_shader_program_->setUniform("color.light_color", lighting->getSkyboxLightColor());
    default_shader_program_->setUniform("color.gamma", postprocess_->getGamma());
    default_shader_program_->setUniform(
        "color.bloom_threshold_color", postprocess_->getGlowBloomThresholdColor());

    default_shader_program_->setUniform("fog.enabled", fog->isEnabled());
    default_shader_program_->setUniform("fog.color", fog->getColor());
    default_shader_program_->setUniform("fog.density", fog->getSkyboxFogOverallDensity());
    default_shader_program_->setUniform("fog.transition_power", fog->getSkyboxFogTransitionPower());
    default_shader_program_->setUniform("fog.height", fog->getSkyboxFogHeight());
}