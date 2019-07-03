/*
 * Puffin OpenGL Engine ver. 2.1
 * Coded by: Sebastian 'qbranchmaster' Tabaka
 * Contact: sebastian.tabaka@outlook.com
 */

#include "PuffinEngine/DefaultSkyboxRenderer.hpp"

using namespace puffin;

DefaultSkyboxRenderer::DefaultSkyboxRenderer(PostprocessPtr postprocess) {
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
    setDefaultShaderUniforms(scene);

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

void DefaultSkyboxRenderer::setDefaultShaderUniforms(ScenePtr scene) {
    if (!scene) {
        logError("DefaultSkyboxRenderer::setShadersUniforms()", PUFFIN_MSG_NULL_OBJECT);
        return;
    }

	auto skybox = scene->getActiveSkybox();

    // Skybox should not be affected by changing near and far plane values
    auto near_plane = scene->camera()->getNearPlane();
    auto far_plane = scene->camera()->getFarPlane();
    scene->camera()->setProjection(
        scene->camera()->getFov(), scene->camera()->getAspect(), 0.5f, 2.5f);

    default_shader_program_->setUniform(
        "matrices.view_matrix", scene->camera()->getViewMatrixStatic());
    default_shader_program_->setUniform(
        "matrices.projection_matrix", scene->camera()->getProjectionMatrix());
    default_shader_program_->setUniform("matrices.model_matrix", skybox->getModelMatrix());

    // Restore previous near and far plane values
    scene->camera()->setProjection(
        scene->camera()->getFov(), scene->camera()->getAspect(), near_plane, far_plane);

    default_shader_program_->setUniform("color.cube_texture", 0);
    default_shader_program_->setUniform(
        "color.light_color", scene->lighting()->getSkyboxLightColor());
    default_shader_program_->setUniform("color.gamma", postprocess_->getGamma());
    default_shader_program_->setUniform("color.bloom_threshold_color", postprocess_->getGlowBloomThresholdColor());

    default_shader_program_->setUniform("fog.enabled", scene->fog()->isEnabled());
    default_shader_program_->setUniform("fog.color", scene->fog()->getColor());
    default_shader_program_->setUniform("fog.density", scene->fog()->getSkyboxFogOverallDensity());
    default_shader_program_->setUniform(
        "fog.transition_power", scene->fog()->getSkyboxFogTransitionPower());
    default_shader_program_->setUniform("fog.height", scene->fog()->getSkyboxFogHeight());
}