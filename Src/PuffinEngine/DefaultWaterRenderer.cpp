/*
 * Puffin OpenGL Engine ver. 2.0.1
 * Coded by: Sebastian 'qbranchmaster' Tabaka
 * Contact: sebastian.tabaka@outlook.com
 */

#include "PuffinEngine/DefaultWaterRenderer.hpp"

using namespace puffin;

DefaultWaterRenderer::DefaultWaterRenderer(PostprocessPtr postprocess,
    DefaultMeshRendererPtr mesh_renderer, DefaultSkyboxRendererPtr skybox_renderer) {
    if (!postprocess || !mesh_renderer || !skybox_renderer) {
        throw Exception("DefaultWaterRenderer::DefaultWaterRenderer()", PUFFIN_MSG_NULL_OBJECT);
    }

    postprocess_ = postprocess;
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

    if (!enabled_) {
        return;
    }

    switch (postprocess_->wireframe()->getMode()) {
    case WireframeMode::None:
        renderNormal(frame_buffer, scene);
        break;
    case WireframeMode::Overlay:
        renderNormal(frame_buffer, scene);
        renderWireframe(frame_buffer, scene);
        break;
    case WireframeMode::Full:
        renderWireframe(frame_buffer, scene);
        break;
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

    DepthTest::instance().enable(true);
    DepthTest::instance().enableDepthMask(true);
    FaceCull::instance().enable(true);
    AlphaBlend::instance().enable(false);

    reflection_frame_buffer_->bind(FrameBufferBindType::Normal);
    FrameBuffer::setViewportSize(
        reflection_frame_buffer_->getWidth(), reflection_frame_buffer_->getHeight());
    FrameBuffer::clear(FrameBufferClearType::DepthAndColor);

    auto wireframe_mode = postprocess_->wireframe()->getMode();
    postprocess_->wireframe()->setMode(WireframeMode::None);

    auto water_level = water_tile->getPosition().y;

    auto camera_pos = scene->camera()->getPosition();
    GLfloat offset = 2.0f * (camera_pos.y - water_level);
    glm::vec3 new_camera_pos(camera_pos.x, camera_pos.y - offset, camera_pos.z);
    scene->camera()->setPosition(new_camera_pos);
    scene->camera()->flipPitch();

    skybox_renderer_->render(reflection_frame_buffer_, scene);

    mesh_renderer_->enableClippingPlane(true);
    mesh_renderer_->setClippingPlane(glm::vec4(0.0f, 1.0f, 0.0f, -water_level));
    mesh_renderer_->render(reflection_frame_buffer_, scene);
    mesh_renderer_->enableClippingPlane(false);

    // Restore previous camera position and orientation
    scene->camera()->setPosition(camera_pos);
    scene->camera()->flipPitch();

    postprocess_->wireframe()->setMode(wireframe_mode);
}

void DefaultWaterRenderer::renderToRefractionFrameBuffer(WaterTilePtr water_tile, ScenePtr scene) {
    if (!water_tile || !scene) {
        return;
    }

    DepthTest::instance().enable(true);
    DepthTest::instance().enableDepthMask(true);
    FaceCull::instance().enable(true);
    AlphaBlend::instance().enable(false);

    refraction_frame_buffer_->bind(FrameBufferBindType::Normal);
    FrameBuffer::setViewportSize(
        refraction_frame_buffer_->getWidth(), refraction_frame_buffer_->getHeight());
    FrameBuffer::clear(FrameBufferClearType::DepthAndColor);

    auto wireframe_mode = postprocess_->wireframe()->getMode();
    postprocess_->wireframe()->setMode(WireframeMode::None);

    auto water_level = water_tile->getPosition().y;

    skybox_renderer_->render(refraction_frame_buffer_, scene);

    mesh_renderer_->enableClippingPlane(true);
    mesh_renderer_->setClippingPlane(glm::vec4(0.0f, -1.0f, 0.0f, water_level));
    mesh_renderer_->render(refraction_frame_buffer_, scene);
    mesh_renderer_->enableClippingPlane(false);

    postprocess_->wireframe()->setMode(wireframe_mode);
}

void DefaultWaterRenderer::loadShaders() {
    default_shader_program_.reset(new ShaderProgram("water_shader_program"));
    default_shader_program_->loadShaders("Data/Shaders/Water.vert", "Data/Shaders/Water.frag");

    wireframe_shader_program_.reset(new ShaderProgram("wireframe_shader_program"));
    wireframe_shader_program_->loadShaders(
        "Data/Shaders/Wireframe.vert", "Data/Shaders/Wireframe.frag");
}

void DefaultWaterRenderer::setDefaultShaderUniforms(
    WaterTilePtr water_tile, CameraPtr camera, LightingPtr lighting, FogPtr fog) {
    default_shader_program_->setUniform("matrices.view_matrix", camera->getViewMatrix());
    default_shader_program_->setUniform(
        "matrices.projection_matrix", camera->getProjectionMatrix());
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
        "water_tile.ambient_factor", water_tile->getAmbientLightingFactor());
    default_shader_program_->setUniform(
        "water_tile.specular_factor", water_tile->getSpecularLightingFactor());

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

    default_shader_program_->setUniform(
        "postprocess.bloom_threshold_color", postprocess_->getGlowBloomThresholdColor());

    default_shader_program_->setUniform("camera_position", camera->getPosition());
    default_shader_program_->setUniform("texture_tiling", texture_tiling_);

    default_shader_program_->setUniform("fog.enabled", fog->isEnabled());
    default_shader_program_->setUniform("fog.density", fog->getDensity());
    default_shader_program_->setUniform("fog.color", fog->getColor());
}

void DefaultWaterRenderer::setWireframeShaderUniforms(WaterTilePtr water_tile, CameraPtr camera) {
    wireframe_shader_program_->setUniform("matrices.view_matrix", camera->getViewMatrix());
    wireframe_shader_program_->setUniform(
        "matrices.projection_matrix", camera->getProjectionMatrix());
    wireframe_shader_program_->setUniform("matrices.model_matrix", water_tile->getModelMatrix());

    wireframe_shader_program_->setUniform("wireframe_color", postprocess_->wireframe()->getColor());
}

void DefaultWaterRenderer::renderNormal(FrameBufferPtr frame_buffer, ScenePtr scene) {
    if (!frame_buffer || !scene) {
        logError("DefaultWaterRenderer::renderNormal()", PUFFIN_MSG_NULL_OBJECT);
        return;
    }

    for (GLuint i = 0; i < scene->getWaterTilesCount(); i++) {
        auto water_tile = scene->getWaterTile(i);
        water_tile->updateWaveMovementFactor();

        renderToRefractionFrameBuffer(water_tile, scene);
        renderToReflectionFrameBuffer(water_tile, scene);

        frame_buffer->bind(FrameBufferBindType::Normal);
        FrameBuffer::setViewportSize(frame_buffer);

        DepthTest::instance().enable(true);
        DepthTest::instance().enableDepthMask(true);
        FaceCull::instance().enable(true);
        AlphaBlend::instance().enable(false);

        default_shader_program_->activate();
        setDefaultShaderUniforms(water_tile, scene->camera(), scene->lighting(), scene->fog());

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

void DefaultWaterRenderer::renderWireframe(FrameBufferPtr frame_buffer, ScenePtr scene) {
    if (!frame_buffer || !scene) {
        logError("DefaultWaterRenderer::renderWireframe()", PUFFIN_MSG_NULL_OBJECT);
        return;
    }

    for (GLuint i = 0; i < scene->getWaterTilesCount(); i++) {
        auto water_tile = scene->getWaterTile(i);

        frame_buffer->bind(FrameBufferBindType::Normal);
        FrameBuffer::setViewportSize(frame_buffer);

        DepthTest::instance().enable(true);
        DepthTest::instance().enableDepthMask(true);
        FaceCull::instance().enable(true);
        AlphaBlend::instance().enable(false);

        wireframe_shader_program_->activate();
        setWireframeShaderUniforms(water_tile, scene->camera());

        postprocess_->wireframe()->enable(true);

        drawWaterTile(water_tile);

        postprocess_->wireframe()->enable(false);
    }
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