/*
 * Puffin OpenGL Engine ver. 2.0.1
 * Coded by: Sebastian 'qbranchmaster' Tabaka
 * Contact: sebastian.tabaka@outlook.com
 */

#include "PuffinEngine/DefaultShadowMapRenderer.hpp"

using namespace puffin;

DefaultShadowMapRenderer::DefaultShadowMapRenderer() {
    camera_frustum_.reset(new CameraFrustum());

    loadShaders();
}

void DefaultShadowMapRenderer::loadShaders() {
    directional_light_shader_program_.reset(
        new ShaderProgram("dir_light_shadow_map_shader_program"));
    directional_light_shader_program_->loadShaders(
        "Data/Shaders/DepthMapDirectionalLight.vert", "Data/Shaders/DepthMapDirectionalLight.frag");

    point_light_shader_program_.reset(new ShaderProgram("point_light_shadow_map_shader_program"));
    point_light_shader_program_->loadShaders("Data/Shaders/DepthMapPointLight.vert",
        "Data/Shaders/DepthMapPointLight.frag", "Data/Shaders/DepthMapPointLight.geom");
}

void DefaultShadowMapRenderer::createDirectionalLightFrameBuffer() {
    auto size = InitConfig::instance().getDirectionalLightShadowMapSize();
    directional_light_shadow_map_frame_bufer_.reset(new FrameBuffer(size, size));
    directional_light_shadow_map_frame_bufer_->addDepthTextureBuffer(false, true);
    directional_light_shadow_map_frame_bufer_->disableDrawBuffer();
    directional_light_shadow_map_frame_bufer_->disableReadBuffer();

    output_data_.shadow_map_texture =
        directional_light_shadow_map_frame_bufer_->getDepthTextureBuffer();
}

glm::mat4 DefaultShadowMapRenderer::calculateDirectionalLightSpaceMatrix(
    LightingPtr lighting, CameraPtr camera) {
    camera_frustum_->setCameraParameters(camera->getNearPlane(), lighting->getShadowDistance(),
        camera->getAspect(), camera->getFov());
    camera_frustum_->setCameraVectors(camera->getDirectionVector(), camera->getRightVector(),
        camera->getUpVector(), camera->getPosition());
    camera_frustum_->calculateFrustumPoints();
    camera_frustum_->calculateBoundingBoxSize();

    glm::vec3 dir_light_pos = camera_frustum_->getCenter();
    glm::mat4 dir_light_view_matrix = glm::lookAt(dir_light_pos,
        dir_light_pos + lighting->directionalLight()->getDirection(), glm::vec3(0.0f, 1.0f, 0.0f));
    camera_frustum_->recalculateToFrame(dir_light_view_matrix);
    camera_frustum_->calculateBoundingBoxSize();

    // Resize shadow box to minimize disappearing shadows glitch
    auto box_size = camera_frustum_->getSize() * 2.0f;
    glm::mat4 projection_matrix =
        glm::ortho(-box_size.x, box_size.x, -box_size.y, box_size.y, -box_size.z, box_size.z);

    return (projection_matrix * dir_light_view_matrix);
}

void DefaultShadowMapRenderer::createPointLightFrameBuffer(GLushort light_index) {
    if (light_index >= InitConfig::getMaxPointLightsCount()) {
        logError(
            "DefaultShadowMapRenderer::createPointLightFrameBuffer()", PUFFIN_MSG_INVALID_VALUE);
        return;
    }

    auto size = InitConfig::instance().getPointLightShadowMapSize();

    point_light_shadow_map_frame_buffer_[light_index].reset(new FrameBuffer(size, size));
    point_light_shadow_map_frame_buffer_[light_index]->addCubeTextureBuffer();

    point_light_shadow_map_frame_buffer_[light_index]->disableDrawBuffer();
    point_light_shadow_map_frame_buffer_[light_index]->disableReadBuffer();

    output_data_.point_light_texture_buffer[light_index] =
        point_light_shadow_map_frame_buffer_[light_index]->getCubeTextureBuffer();
}

void DefaultShadowMapRenderer::renderPointLightShadowMap(ScenePtr scene) {
    if (!scene) {
        logError("DefaultShadowMapRenderer::renderPointLightShadowMap()", PUFFIN_MSG_NULL_OBJECT);
        return;
    }

    DepthTest::instance().enable(true);
    DepthTest::instance().enableDepthMask(true);
    FaceCull::instance().enable(true);
    AlphaBlend::instance().enable(false);

    glm::mat4 point_light_projection_matrix_ =
        glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, scene->lighting()->getShadowDistance());

    point_light_shader_program_->activate();
    point_light_shader_program_->setUniform(
        "shadow_distance", scene->lighting()->getShadowDistance());

    for (GLushort i = 0; i < scene->lighting()->getPointLightsCount(); i++) {
        if (!point_light_shadow_map_frame_buffer_[i]) {
            createPointLightFrameBuffer(i);
        }

        auto point_light = scene->lighting()->getPointLight(i);
        if (!point_light->isEnabled()) {
            continue;
        }

        point_light_shader_program_->setUniform("light_position", point_light->getPosition());

        std::vector<glm::mat4> shadow_transforms;
        auto light_pos = point_light->getPosition();

        shadow_transforms.push_back(point_light_projection_matrix_ *
            glm::lookAt(
                light_pos, light_pos + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
        shadow_transforms.push_back(point_light_projection_matrix_ *
            glm::lookAt(
                light_pos, light_pos + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
        shadow_transforms.push_back(point_light_projection_matrix_ *
            glm::lookAt(
                light_pos, light_pos + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
        shadow_transforms.push_back(point_light_projection_matrix_ *
            glm::lookAt(
                light_pos, light_pos + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
        shadow_transforms.push_back(point_light_projection_matrix_ *
            glm::lookAt(
                light_pos, light_pos + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
        shadow_transforms.push_back(point_light_projection_matrix_ *
            glm::lookAt(
                light_pos, light_pos + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));

        for (GLushort i = 0; i < 6; i++) {
            point_light_shader_program_->setUniform(
                "shadow_matrices[" + std::to_string(i) + "].mat", shadow_transforms[i]);
        }

        point_light_shadow_map_frame_buffer_[i]->bind(FrameBufferBindType::Normal);
        FrameBuffer::setViewportSize(point_light_shadow_map_frame_buffer_[i]);
        FrameBuffer::clear(FrameBufferClearType::OnlyDepth);

        for (GLuint i = 0; i < scene->getMeshesCount(); i++) {
            MeshPtr mesh = scene->getMesh(i);
            if (!mesh) {
                continue;
            }

            if (!mesh->isShadowCastingEnabled()) {
                continue;
            }

            point_light_shader_program_->setUniform(
                "matrices.model_matrix", mesh->getModelMatrix());

            mesh->bind();
            for (GLushort i = 0; i < mesh->getEntitiesCount(); i++) {
                auto entity = mesh->getEntity(i);
                auto material = entity->getMaterial();
                if (material && material->hasTransparency()) {
                    continue;
                }

                glDrawElements(GL_TRIANGLES, entity->getIndicesCount(), GL_UNSIGNED_INT,
                    reinterpret_cast<void *>((entity->getStartingVertexIndex() * sizeof(GLuint))));
            }
        }
    }
}

void DefaultShadowMapRenderer::render(ScenePtr scene) {
    if (!scene) {
        logError("DefaultShadowMapRenderer::render()", PUFFIN_MSG_NULL_OBJECT);
        return;
    }

    renderDirectionalLightShadowMap(scene);
    renderPointLightShadowMap(scene);
}

void DefaultShadowMapRenderer::renderDirectionalLightShadowMap(ScenePtr scene) {
    if (!scene) {
        logError(
            "DefaultShadowMapRenderer::renderDirectionalLightShadowMap()", PUFFIN_MSG_NULL_OBJECT);
        return;
    }

    if (!directional_light_shadow_map_frame_bufer_) {
        createDirectionalLightFrameBuffer();
    }

    DepthTest::instance().enable(true);
    DepthTest::instance().enableDepthMask(true);
    FaceCull::instance().enable(true);
    AlphaBlend::instance().enable(false);

    directional_light_shadow_map_frame_bufer_->bind(FrameBufferBindType::Normal);
    FrameBuffer::setViewportSize(directional_light_shadow_map_frame_bufer_);
    FrameBuffer::clear(FrameBufferClearType::OnlyDepth);

    if (!scene->lighting()->isShadowMappingEnabled() ||
        !scene->lighting()->directionalLight()->isEnabled()) {
        return;
    }

    output_data_.dir_light_space_matrix =
        calculateDirectionalLightSpaceMatrix(scene->lighting(), scene->camera());

    directional_light_shader_program_->activate();
    directional_light_shader_program_->setUniform(
        "matrices.light_space_matrix", output_data_.dir_light_space_matrix);

    for (GLuint i = 0; i < scene->getMeshesCount(); i++) {
        MeshPtr mesh = scene->getMesh(i);
        if (!mesh) {
            continue;
        }

        if (!mesh->isShadowCastingEnabled()) {
            continue;
        }

        directional_light_shader_program_->setUniform(
            "matrices.model_matrix", mesh->getModelMatrix());

        mesh->bind();
        for (GLushort i = 0; i < mesh->getEntitiesCount(); i++) {
            auto entity = mesh->getEntity(i);
            auto material = entity->getMaterial();
            if (material && material->hasTransparency()) {
                continue;
            }

            glDrawElements(GL_TRIANGLES, entity->getIndicesCount(), GL_UNSIGNED_INT,
                reinterpret_cast<void *>((entity->getStartingVertexIndex() * sizeof(GLuint))));
        }
    }
}