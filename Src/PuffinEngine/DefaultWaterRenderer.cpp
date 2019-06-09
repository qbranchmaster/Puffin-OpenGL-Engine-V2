/*
 * Puffin OpenGL Engine ver. 2.1
 * Coded by: Sebastian 'qbranchmaster' Tabaka
 * Contact: sebastian.tabaka@outlook.com
 */

#include "PuffinEngine/DefaultWaterRenderer.hpp"

using namespace puffin;

DefaultWaterRenderer::DefaultWaterRenderer(RenderSettingsPtr render_settings, CameraPtr camera,
    DefaultMeshRendererPtr mesh_renderer, DefaultSkyboxRendererPtr skybox_renderer) {
    if (!render_settings || !camera || !mesh_renderer || !skybox_renderer) {
        throw Exception("DefaultWaterRenderer::DefaultWaterRenderer()", PUFFIN_MSG_NULL_OBJECT);
    }

    render_settings_ = render_settings;
    camera_ = camera;
    mesh_renderer_ = mesh_renderer;
    skybox_renderer_ = skybox_renderer;

    dudv_map_path_ = "Data/Textures/WaterDuDv.png";
    dudv_map_.reset(new Texture());
    dudv_map_->loadTexture2D(dudv_map_path_);

    normal_map_path_ = "Data/Textures/WaterNormalMap.png";
    normal_map_.reset(new Texture());
    normal_map_->loadTexture2D(normal_map_path_);

    loadShaders();
    createFrameBuffers();
}

void DefaultWaterRenderer::render(FrameBufferPtr frame_buffer, ScenePtr scene) {
    if (!frame_buffer || !scene) {
        logError("DefaultWaterRenderer::render()", PUFFIN_MSG_NULL_OBJECT);
        return;
    }

    for (GLuint i = 0; i < scene->getWaterTilesCount(); i++) {
        auto water_tile = scene->getWaterTile(i);
        water_tile->updateWaveMovementFactor();

        renderToReflectionFrameBuffer(water_tile, scene);
        renderToRefractionFrameBuffer(water_tile, scene);

        frame_buffer->bind(FrameBufferBindType::Normal);
        FrameBuffer::setViewportSize(frame_buffer);

        DepthTest::instance().enable(true);
        DepthTest::instance().enableDepthMask(true);
        FaceCull::instance().enable(true);
        AlphaBlend::instance().enable(false);

        default_shader_program_->activate();
        setShadersUniforms(water_tile);

        Texture::setTextureSlot(0);
        reflection_frame_buffer_->getTextureBuffer(0)->bind();
        Texture::setTextureSlot(1);
        refraction_frame_buffer_->getTextureBuffer(0)->bind();
        Texture::setTextureSlot(2);
        dudv_map_->bind();
        Texture::setTextureSlot(3);
        normal_map_->bind();

        drawWaterTile(water_tile);
    }
}

void DefaultWaterRenderer::setTextureTiling(GLushort tiling) {
    if (tiling <= 0) {
        logError("DefaultWaterRenderer::setTextureTiling()", PUFFIN_MSG_INVALID_VALUE);
        return;
    }

    texture_tiling_ = tiling;
}

void DefaultWaterRenderer::renderToReflectionFrameBuffer(WaterTilePtr water_tile, ScenePtr scene) {
    if (!water_tile || !scene) {
        return;
    }

    reflection_frame_buffer_->bind(FrameBufferBindType::Normal);
    FrameBuffer::setViewportSize(
        reflection_frame_buffer_->getWidth(), reflection_frame_buffer_->getHeight());
    FrameBuffer::clear(FrameBufferClearType::DepthAndColor);

    GLboolean shadow_map_state = render_settings_->lighting()->isShadowMappingEnabled();
    render_settings_->lighting()->enableShadowMapping(false);

    auto water_level = water_tile->getPosition().y;

    auto camera_pos = camera_->getPosition();
    GLfloat offset = 2.0f * (camera_pos.y - water_level);
    glm::vec3 new_camera_pos(camera_pos.x, camera_pos.y - offset, camera_pos.z);
    camera_->setPosition(new_camera_pos);
    camera_->flipPitch();

    skybox_renderer_->render(reflection_frame_buffer_, scene);

    mesh_renderer_->enableClippingPlane(true);
    mesh_renderer_->setClippingPlane(glm::vec4(0.0f, 1.0f, 0.0f, -water_level));
    mesh_renderer_->render(reflection_frame_buffer_, scene);
    mesh_renderer_->enableClippingPlane(false);

    render_settings_->lighting()->enableShadowMapping(shadow_map_state);

    // Restore previous camera position and orientation
    camera_->setPosition(camera_pos);
    camera_->flipPitch();
}

void DefaultWaterRenderer::renderToRefractionFrameBuffer(WaterTilePtr water_tile, ScenePtr scene) {
    if (!water_tile || !scene) {
        return;
    }

    refraction_frame_buffer_->bind(FrameBufferBindType::Normal);
    FrameBuffer::setViewportSize(
        refraction_frame_buffer_->getWidth(), refraction_frame_buffer_->getHeight());
    FrameBuffer::clear(FrameBufferClearType::DepthAndColor);

    GLboolean shadow_map_state = render_settings_->lighting()->isShadowMappingEnabled();
    render_settings_->lighting()->enableShadowMapping(false);

    auto water_level = water_tile->getPosition().y;

    skybox_renderer_->render(refraction_frame_buffer_, scene);

    mesh_renderer_->enableClippingPlane(true);
    mesh_renderer_->setClippingPlane(glm::vec4(0.0f, -1.0f, 0.0f, water_level));
    mesh_renderer_->render(refraction_frame_buffer_, scene);
    mesh_renderer_->enableClippingPlane(false);

    render_settings_->lighting()->enableShadowMapping(shadow_map_state);
}

void DefaultWaterRenderer::loadShaders() {
    default_shader_program_.reset(new ShaderProgram("water_shader_program"));
    default_shader_program_->loadShaders("Data/Shaders/Water.vert", "Data/Shaders/Water.frag");
}

void DefaultWaterRenderer::setShadersUniforms(WaterTilePtr water_tile) {
    default_shader_program_->setUniform("matrices.view_matrix", camera_->getViewMatrix());
    default_shader_program_->setUniform(
        "matrices.projection_matrix", camera_->getProjectionMatrix());
    default_shader_program_->setUniform("matrices.model_matrix", water_tile->getModelMatrix());

    default_shader_program_->setUniform("reflection_texture", 0);
    default_shader_program_->setUniform("refraction_texture", 1);
    default_shader_program_->setUniform("dudv_map", 2);
    default_shader_program_->setUniform("normal_map", 3);

    default_shader_program_->setUniform("water_tile.wave_strength", water_tile->getWaveStrength());
    default_shader_program_->setUniform(
        "water_tile.move_factor", water_tile->getWaveMovementFactor());
    default_shader_program_->setUniform("water_tile.water_color", water_tile->getWaterColor());
    default_shader_program_->setUniform("water_tile.shininess", water_tile->getShininess());
    default_shader_program_->setUniform(
        "water_tile.ambient_factor", water_tile->getAmbientFactor());
    default_shader_program_->setUniform(
        "water_tile.specular_factor", water_tile->getSpecularFactor());

    auto lighting = render_settings_->lighting();
    default_shader_program_->setUniform(
        "directional_light.enabled", lighting->directionalLight()->isEnabled());
    default_shader_program_->setUniform(
        "directional_light.direction", lighting->directionalLight()->getDirection());
    default_shader_program_->setUniform(
        "directional_light.ambient_color", lighting->directionalLight()->getAmbientColor());
    default_shader_program_->setUniform(
        "directional_light.diffuse_color", lighting->directionalLight()->getDiffuseColor());
    default_shader_program_->setUniform(
        "directional_light.specular_color", lighting->directionalLight()->getSpecularColor());

    // default_shader_program_->setUniform(
    //    "postprocess.gamma", render_settings_->postprocess()->getGamma());
    default_shader_program_->setUniform("postprocess.bloom_threshold_color",
        render_settings_->postprocess()->getGlowBloomThresholdColor());

    default_shader_program_->setUniform("camera_position", camera_->getPosition());
    default_shader_program_->setUniform("texture_tiling", texture_tiling_);
}

void DefaultWaterRenderer::createFrameBuffers() {
    auto reflection_res = InitConfig::instance().getWaterReflectionResolution();
    reflection_frame_buffer_.reset(new FrameBuffer(reflection_res.first, reflection_res.second));
    reflection_frame_buffer_->addTextureBuffer(0, false, false);
    reflection_frame_buffer_->addDepthTextureBuffer(false, true);

    auto refraction_res = InitConfig::instance().getWaterRefractionResolution();
    refraction_frame_buffer_.reset(new FrameBuffer(refraction_res.first, refraction_res.second));
    refraction_frame_buffer_->addTextureBuffer(0, false, false);
    refraction_frame_buffer_->addDepthTextureBuffer(false, true);
}

void DefaultWaterRenderer::drawWaterTile(WaterTilePtr water_tile) {
    if (!water_tile) {
        return;
    }

    water_tile->bind();
    glDrawArrays(GL_TRIANGLES, 0, 6);
}