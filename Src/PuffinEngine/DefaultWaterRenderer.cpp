/*
* Puffin OpenGL Engine ver. 2.0
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

        renderToReflectionFrameBuffer(water_tile, scene);
        renderToRefractionFrameBuffer(water_tile, scene);

        FrameBuffer::setViewportSize(frame_buffer);
        frame_buffer->bind(FrameBufferBindType::Normal);

        DepthTest::instance().enable(true);
        DepthTest::instance().enableDepthMask(true);
        FaceCull::instance().enable(true);
        AlphaBlend::instance().enable(false);

        default_shader_program_->activate();
        setShadersUniforms(water_tile);
        drawWaterTile(water_tile);
    }
}

void DefaultWaterRenderer::renderToReflectionFrameBuffer(WaterTilePtr water_tile, ScenePtr scene) {
    if (!water_tile || !scene) {
        return;
    }

    reflection_frame_buffer_->bind(FrameBufferBindType::Normal);
    FrameBuffer::clear(FrameBufferClearType::DepthAndColor);

    render_settings_->lighting()->enableShadowMapping(false);

    auto water_level = water_tile->getPosition().y;

    auto camera_pos = camera_->getPosition();
    GLfloat offset = 2.0f * (camera_pos.y - water_level);

    glm::vec3 new_camera_pos(camera_pos.x, camera_pos.y - offset, camera_pos.z);
    camera_->setPosition(new_camera_pos);
    camera_->flipPitch();

    skybox_renderer_->render(reflection_frame_buffer_, scene);

    mesh_renderer_->enableClippingPlane(true);
    mesh_renderer_->setClippingPlane(glm::vec4(0.0f, 1.0f, 0.0f, water_level));
    mesh_renderer_->render(reflection_frame_buffer_, scene);
    mesh_renderer_->enableClippingPlane(false);

    render_settings_->lighting()->enableShadowMapping(true);

    // Restore previous camera position and orientation
    camera_->setPosition(camera_pos);
    camera_->flipPitch();
}

void DefaultWaterRenderer::renderToRefractionFrameBuffer(WaterTilePtr water_tile, ScenePtr scene) {

}

void DefaultWaterRenderer::loadShaders() {
    default_shader_program_.reset(new ShaderProgram("water_shader_program"));
    default_shader_program_->loadShaders("Data/Shaders/Water.vert", "Data/Shaders/Water.frag");
}

void DefaultWaterRenderer::setShadersUniforms(WaterTilePtr water_tile) {
    default_shader_program_->setUniform("matrices.view_matrix", camera_->getViewMatrix());
    default_shader_program_->setUniform("matrices.projection_matrix",
        camera_->getProjectionMatrix());
    default_shader_program_->setUniform("matrices.model_matrix", water_tile->getModelMatrix());
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