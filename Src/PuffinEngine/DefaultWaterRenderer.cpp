/*
* Puffin OpenGL Engine ver. 2.0
* Coded by: Sebastian 'qbranchmaster' Tabaka
* Contact: sebastian.tabaka@outlook.com
*/

#include "PuffinEngine/DefaultWaterRenderer.hpp"

using namespace puffin;

DefaultWaterRenderer::DefaultWaterRenderer(RenderSettingsPtr render_settings, CameraPtr camera) {
    if (!render_settings || !camera) {
        throw Exception("DefaultWaterRenderer::DefaultWaterRenderer()", PUFFIN_MSG_NULL_OBJECT);
    }

    render_settings_ = render_settings;
    camera_ = camera;

    loadShaders();
}

void DefaultWaterRenderer::render(FrameBufferPtr frame_buffer, ScenePtr scene) {
    if (!frame_buffer || !scene) {
        logError("DefaultWaterRenderer::render()", PUFFIN_MSG_NULL_OBJECT);
        return;
    }

    default_shader_program_->activate();

    for (GLuint i = 0; i < scene->getWaterTilesCount(); i++) {
        auto water_tile = scene->getWaterTile(i);

        setShadersUniforms(water_tile);
        drawWaterTile(water_tile);
    }

}

void DefaultWaterRenderer::loadShaders() {
    default_shader_program_.reset(new ShaderProgram());
    default_shader_program_->loadShaders("Data/Shaders/Water.vert", "Data/Shaders/Water.frag");
}

void DefaultWaterRenderer::setShadersUniforms(WaterTilePtr water_tile) {
    default_shader_program_->setUniform("matrices.view_matrix", camera_->getViewMatrix());
    default_shader_program_->setUniform("matrices.projection_matrix",
        camera_->getProjectionMatrix());
    default_shader_program_->setUniform("matrices.model_matrix", water_tile->getModelMatrix());
}

void DefaultWaterRenderer::drawWaterTile(WaterTilePtr water_tile) {
    if (!water_tile) {
        return;
    }

    water_tile->bind();
    glDrawArrays(GL_TRIANGLES, 0, 6);
}