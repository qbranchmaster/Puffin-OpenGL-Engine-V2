/*
 * Puffin OpenGL Engine ver. 2.1
 * Coded by: Sebastian 'qbranchmaster' Tabaka
 * Contact: sebastian.tabaka@outlook.com
 */

#include "PuffinEngine/DefaultMeshRenderer.hpp"

using namespace puffin;

DefaultMeshRenderer::DefaultMeshRenderer(
    DefaultShadowMapRendererPtr shadow_map_renderer) {
    if (!shadow_map_renderer) {
        throw Exception("DefaultMeshRenderer::DefaultMeshRenderer()", PUFFIN_MSG_NULL_OBJECT);
    }

    shadow_map_renderer_ = shadow_map_renderer;

    loadShaders();
}

void DefaultMeshRenderer::loadShaders() {
    default_shader_program_.reset(new ShaderProgram("mesh_shader_program"));
    default_shader_program_->loadShaders("Data/Shaders/Mesh.vert", "Data/Shaders/Mesh.frag");

    wireframe_shader_program_.reset(new ShaderProgram("wireframe_shader_program"));
    wireframe_shader_program_->loadShaders(
        "Data/Shaders/Wireframe.vert", "Data/Shaders/Wireframe.frag");
}

void DefaultMeshRenderer::setDefaultShaderUniforms(ScenePtr scene) {
    default_shader_program_->setUniform("clipping_plane", clipping_plane_);

    default_shader_program_->setUniform("matrices.view_matrix", scene->camera()->getViewMatrix());
    default_shader_program_->setUniform(
        "matrices.projection_matrix", scene->camera()->getProjectionMatrix());

    // Lighting
    auto lighting = scene->lighting();
    default_shader_program_->setUniform("lighting.enabled", lighting->isEnabled());
    default_shader_program_->setUniform(
        "lighting.directional_light.enabled", lighting->directionalLight()->isEnabled());
    default_shader_program_->setUniform(
        "lighting.directional_light.direction", lighting->directionalLight()->getDirection());
    default_shader_program_->setUniform("lighting.directional_light.ambient_color",
        lighting->directionalLight()->getAmbientColor());
    default_shader_program_->setUniform("lighting.directional_light.diffuse_color",
        lighting->directionalLight()->getDiffuseColor());
    default_shader_program_->setUniform("lighting.directional_light.specular_color",
        lighting->directionalLight()->getSpecularColor());
    default_shader_program_->setUniform("lighting.blinn_phong", lighting->isBlinnPhongEnabled());
    default_shader_program_->setUniform("lighting.emission_factor", lighting->getEmissionFactor());

    default_shader_program_->setUniform("other.gamma", render_settings_->postprocess()->getGamma());
    default_shader_program_->setUniform("other.bloom_threshold_color",
        render_settings_->postprocess()->getGlowBloomThresholdColor());

    // Shadow mapping
    default_shader_program_->setUniform(
        "shadow_mapping.enabled", lighting->isShadowMappingEnabled());
    default_shader_program_->setUniform("shadow_mapping.shadow_map_texture", 6);
    default_shader_program_->setUniform(
        "matrices.dir_light_matrix", shadow_map_renderer_->getOutputData().dir_light_space_matrix);
    default_shader_program_->setUniform("shadow_mapping.shadow_map_size",
        static_cast<GLint>(InitConfig::instance().getDirectionalLightShadowMapSize()));
    default_shader_program_->setUniform("shadow_mapping.pcf_filter_count",
        static_cast<GLint>(lighting->getShadowMappingPcfSamplesCount()));

    default_shader_program_->setUniform("fog.enabled", scene->fog()->isEnabled());
    default_shader_program_->setUniform("fog.color", scene->fog()->getColor());
    default_shader_program_->setUniform("fog.density", scene->fog()->getDensity());
}

void DefaultMeshRenderer::setDefaultShaderMeshUniforms(MeshPtr mesh, MeshEntityPtr entity) {
    if (!mesh || !entity) {
        logError("DefaultMeshRenderer::setDefaultShaderMeshUniforms()", PUFFIN_MSG_NULL_OBJECT);
        return;
    }

    auto model_matrix = mesh->getModelMatrix() * entity->getModelMatrix();
    default_shader_program_->setUniform("matrices.model_matrix", model_matrix);
    default_shader_program_->setUniform("matrices.normal_matrix", mesh->getNormalMatrix());

    auto material = entity->getMaterial();
    if (!material) {
        return;
    }

    default_shader_program_->setUniform("material.has_ambient_texture",
        material->getAmbientTexture() != nullptr ? GL_TRUE : GL_FALSE);
    default_shader_program_->setUniform("material.ambient_texture", 0);
    default_shader_program_->setUniform("material.ka", material->getKa());

    default_shader_program_->setUniform("material.has_diffuse_texture",
        material->getDiffuseTexture() != nullptr ? GL_TRUE : GL_FALSE);
    default_shader_program_->setUniform("material.diffuse_texture", 1);
    default_shader_program_->setUniform("material.kd", material->getKd());

    default_shader_program_->setUniform("material.has_specular_texture",
        material->getSpecularTexture() != nullptr ? GL_TRUE : GL_FALSE);
    default_shader_program_->setUniform("material.specular_texture", 2);
    default_shader_program_->setUniform("material.ks", material->getKs());

    default_shader_program_->setUniform("material.has_emissive_texture",
        material->getEmissiveTexture() != nullptr ? GL_TRUE : GL_FALSE);
    default_shader_program_->setUniform("material.emissive_texture", 3);
    default_shader_program_->setUniform("material.ke", material->getKe());

    default_shader_program_->setUniform("material.has_normalmap_texture",
        material->getNormalMapTexture() != nullptr ? GL_TRUE : GL_FALSE);
    default_shader_program_->setUniform("material.normalmap_texture", 4);

    default_shader_program_->setUniform("material.has_opacity_texture",
        material->getOpacityTexture() != nullptr ? GL_TRUE : GL_FALSE);
    default_shader_program_->setUniform("material.opacity_texture", 5);

    default_shader_program_->setUniform("material.shininess", material->getShininess());
    default_shader_program_->setUniform("material.transparency", material->getTransparency());
}

void DefaultMeshRenderer::renderMeshEntity(MeshPtr mesh, GLuint entity_index) {
    if (!mesh) {
        logError("DefaultMeshRenderer::renderMeshEntity()", PUFFIN_MSG_NULL_OBJECT);
        return;
    }

    mesh->bind();

    auto entity = mesh->getEntity(entity_index);
    if (entity) {
        setDefaultShaderMeshUniforms(mesh, entity);

        auto material = entity->getMaterial();
        if (material) {
            Texture::setTextureSlot(0);
            auto ambient_texture = material->getAmbientTexture();
            if (ambient_texture) {
                ambient_texture->bind();
            }
            else {
                Texture::unbindTextureType(TextureType::Texture2D);
            }

            Texture::setTextureSlot(1);
            auto diffuse_texture = material->getDiffuseTexture();
            if (diffuse_texture) {
                diffuse_texture->bind();
            }
            else {
                Texture::unbindTextureType(TextureType::Texture2D);
            }

            Texture::setTextureSlot(2);
            auto specular_texture = material->getSpecularTexture();
            if (specular_texture) {
                specular_texture->bind();
            }
            else {
                Texture::unbindTextureType(TextureType::Texture2D);
            }

            Texture::setTextureSlot(3);
            auto emissive_texture = material->getEmissiveTexture();
            if (emissive_texture) {
                emissive_texture->bind();
            }
            else {
                Texture::unbindTextureType(TextureType::Texture2D);
            }

            Texture::setTextureSlot(4);
            auto normalmap_texture = material->getNormalMapTexture();
            if (normalmap_texture) {
                normalmap_texture->bind();
            }
            else {
                Texture::unbindTextureType(TextureType::Texture2D);
            }

            Texture::setTextureSlot(5);
            auto opacity_texture = material->getOpacityTexture();
            if (opacity_texture) {
                opacity_texture->bind();
            }
            else {
                Texture::unbindTextureType(TextureType::Texture2D);
            }
        }

        drawMeshEntity(entity);
    }
}

void DefaultMeshRenderer::drawMeshEntity(MeshEntityPtr entity) {
    if (!entity) {
        logError("DefaultMeshRenderer::drawMeshEntity()", PUFFIN_MSG_NULL_OBJECT);
        return;
    }

    glDrawElements(GL_TRIANGLES, entity->getIndicesCount(), GL_UNSIGNED_INT,
        reinterpret_cast<void *>((entity->getStartingVertexIndex() * sizeof(GLuint))));
}

void DefaultMeshRenderer::renderNormal(FrameBufferPtr frame_buffer, ScenePtr scene) {
    if (!frame_buffer || !scene) {
        logError("DefaultMeshRenderer::renderNormal()", PUFFIN_MSG_NULL_OBJECT);
        return;
    }

    frame_buffer->bind(FrameBufferBindType::Normal);
    FrameBuffer::setViewportSize(frame_buffer);

    DepthTest::instance().enable(true);
    DepthTest::instance().enableDepthMask(true);
    FaceCull::instance().enable(true);
    AlphaBlend::instance().enable(false);

    if (clipping_plane_enabled_) {
        glEnable(GL_CLIP_DISTANCE0);
    }

    Texture::setTextureSlot(6);
    shadow_map_renderer_->getOutputData().shadow_map_texture->bind();

    default_shader_program_->activate();
    setDefaultShaderUniforms(scene);

    for (GLuint i = 0; i < scene->getMeshesCount(); i++) {
        auto mesh = scene->getMesh(i);

        // First pass - skip transparent entities
        std::vector<GLuint> skipped_en;

        for (GLuint i = 0; i < mesh->getEntitiesCount(); i++) {
            auto entity = mesh->getEntity(i);
            auto material = entity->getMaterial();
            if (material && material->hasTransparency()) {
                skipped_en.push_back(i);
                continue;
            }

            renderMeshEntity(mesh, i);
        }

        // Second pass - draw transparent entities
        // TODO: Sort them in order

        AlphaBlend::instance().enable(true);
        AlphaBlend::instance().setBlendFunction(BlendFunction::Normal);

        for (const auto &i : skipped_en) {
            auto entity = mesh->getEntity(i);
            renderMeshEntity(mesh, i);
        }

        AlphaBlend::instance().enable(false);
    }

    if (clipping_plane_enabled_) {
        glDisable(GL_CLIP_DISTANCE0);
    }
}

void DefaultMeshRenderer::renderWireframe(FrameBufferPtr frame_buffer, ScenePtr scene) {
    if (!frame_buffer || !scene) {
        logError("DefaultMeshRenderer::renderWireframe()", PUFFIN_MSG_NULL_OBJECT);
        return;
    }

    frame_buffer->bind(FrameBufferBindType::Normal);
    FrameBuffer::setViewportSize(frame_buffer);

    DepthTest::instance().enable(true);
    DepthTest::instance().enableDepthMask(true);
    FaceCull::instance().enable(true);
    AlphaBlend::instance().enable(false);

    wireframe_shader_program_->activate();
    setWireframeShaderUniforms(scene->camera());

    for (GLuint i = 0; i < scene->getMeshesCount(); i++) {
        auto mesh = scene->getMesh(i);

        if (!mesh) {
            continue;
        }

        mesh->bind();

        for (GLuint i = 0; i < mesh->getEntitiesCount(); i++) {
            auto entity = mesh->getEntity(i);

            setWireframeShaderMeshUniforms(mesh, entity);

            render_settings_->postprocess()->wireframe()->enable(true);
            drawMeshEntity(entity);
            render_settings_->postprocess()->wireframe()->enable(false);
        }
    }
}

void DefaultMeshRenderer::setWireframeShaderUniforms(CameraPtr camera) {
    wireframe_shader_program_->setUniform(
        "wireframe_color", render_settings_->postprocess()->wireframe()->getColor());
    wireframe_shader_program_->setUniform("matrices.view_matrix", camera->getViewMatrix());
    wireframe_shader_program_->setUniform(
        "matrices.projection_matrix", camera->getProjectionMatrix());
}

void DefaultMeshRenderer::setWireframeShaderMeshUniforms(MeshPtr mesh, MeshEntityPtr entity) {
    if (!mesh || !entity) {
        logError("DefaultMeshRenderer::setWireframeShaderMeshUniforms()", PUFFIN_MSG_NULL_OBJECT);
        return;
    }

    auto model_matrix = mesh->getModelMatrix() * entity->getModelMatrix();
    wireframe_shader_program_->setUniform("matrices.model_matrix", model_matrix);
}

void DefaultMeshRenderer::render(FrameBufferPtr frame_buffer, ScenePtr scene) {
    if (!frame_buffer || !scene) {
        logError("DefaultMeshRenderer::render()", PUFFIN_MSG_NULL_OBJECT);
        return;
    }

    if (!enabled_) {
        return;
    }

    switch (render_settings_->postprocess()->wireframe()->getMode()) {
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