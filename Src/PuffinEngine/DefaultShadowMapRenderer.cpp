/*
* Puffin OpenGL Engine ver. 2.0
* Coded by: Sebastian 'qbranchmaster' Tabaka
*/

#include "PuffinEngine/DefaultShadowMapRenderer.hpp"

using namespace puffin;

DefaultShadowMapRenderer::DefaultShadowMapRenderer(
    RenderSettingsPtr render_settings, CameraPtr camera) {
    if (!render_settings || !camera) {
        throw Exception("DefaultShadowMapRenderer::DefaultShadowMapRenderer()",
            "Not initialized object.");
    }

    render_settings_ = render_settings;
    camera_ = camera;

    camera_frustum_.reset(new CameraFrustum());

    loadShaders();
}

void DefaultShadowMapRenderer::loadShaders() {
    directional_light_shader_program_.reset(new ShaderProgram());
    directional_light_shader_program_->loadShaders(
        "Shaders/DepthMapDirectionalLight.vert",
        "Shaders/DepthMapDirectionalLight.frag");
}

void DefaultShadowMapRenderer::createDirectionalLightFrameBuffer() {
    auto size = render_settings_->lighting()->
        getDirectionalLightShadowMapSize();
    directional_light_shadow_map_frame_bufer_.reset(
        new FrameBuffer(size, size));
    directional_light_shadow_map_frame_bufer_->addDepthTextureBuffer();
    directional_light_shadow_map_frame_bufer_->disableDrawBuffer();
    directional_light_shadow_map_frame_bufer_->disableReadBuffer();

    renderers_shared_data_->shadow_map_texture =
        directional_light_shadow_map_frame_bufer_->getDepthTextureBuffer();
}

glm::mat4 DefaultShadowMapRenderer::calculateDirectionalLightSpaceMatrix() {
    camera_frustum_->setCameraParameters(camera_->getNearPlane(),
        render_settings_->lighting()->getShadowDistance(),
        camera_->getAspect(), camera_->getFov());
    camera_frustum_->setCameraVectors(camera_->getDirectionVector(),
        camera_->getRightVector(), camera_->getUpVector(),
        camera_->getPosition());
    camera_frustum_->calculateFrustumPoints();
    camera_frustum_->calculateBoundingBoxSize();

    glm::vec3 dir_light_pos = camera_frustum_->getCenter();
    glm::mat4 dir_light_view_matrix = glm::lookAt(dir_light_pos,
        dir_light_pos + render_settings_->lighting()->
        directionalLight()->getDirection(), glm::vec3(0.0f, 1.0f, 0.0f));
    camera_frustum_->recalculateToFrame(dir_light_view_matrix);
    camera_frustum_->calculateBoundingBoxSize();

    auto box_size = camera_frustum_->getSize();
    glm::mat4 projection_matrix = glm::ortho(-box_size.x, box_size.x,
        -box_size.y, box_size.y, -box_size.z, box_size.z);

    return (projection_matrix * dir_light_view_matrix);
}

void DefaultShadowMapRenderer::render(MeshPtr mesh) {
    if (!mesh) {
        logError("DefaultShadowMapRenderer::render()", "Null input.");
        return;
    }

    renderDirectionalLightShadowMap(mesh);
}

void DefaultShadowMapRenderer::renderDirectionalLightShadowMap(MeshPtr mesh) {
    if (!render_settings_->lighting()->directionalLight()->isEnabled()) {
        return;
    }

    if (!directional_light_shadow_map_frame_bufer_) {
        createDirectionalLightFrameBuffer();
    }

    renderers_shared_data_->dir_light_space_matrix =
        calculateDirectionalLightSpaceMatrix();

    DepthTest::instance().enable(true);
    DepthTest::instance().enableDepthMask(true);
    FaceCull::instance().enable(true);
    AlphaBlend::instance().enable(false);

    directional_light_shadow_map_frame_bufer_->bind(
        FrameBufferBindType::NORMAL);

    auto size = render_settings_->lighting()->
        getDirectionalLightShadowMapSize();
    FrameBuffer::setViewportSize(size, size);
    FrameBuffer::clear(FrameBufferClearType::ONLY_DEPTH);

    directional_light_shader_program_->activate();
    directional_light_shader_program_->setUniform(
        "matrices.light_space_matrix", renderers_shared_data_->
        dir_light_space_matrix);
    directional_light_shader_program_->setUniform("matrices.model_matrix",
        mesh->getModelMatrix());

    mesh->bind();
    for (GLushort i = 0; i < mesh->getEntitiesCount(); i++) {
        auto entity = mesh->getEntity(i);
        auto material = entity->getMaterial();
        if (material && material->hasTransparency()) {
            continue;
        }

        glDrawElements(GL_TRIANGLES, entity->getIndicesCount(),
            GL_UNSIGNED_INT, reinterpret_cast<void*>((
                entity->getStartingVertexIndex() * sizeof(GLuint))));
    }
}