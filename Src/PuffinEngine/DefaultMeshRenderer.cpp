/*
* Puffin OpenGL Engine ver. 2.0
* Coded by: Sebastian 'qbranchmaster' Tabaka
*/

#include "PuffinEngine/DefaultMeshRenderer.hpp"

using namespace puffin;

DefaultMeshRenderer::DefaultMeshRenderer(RenderSettingsPtr render_settings,
    CameraPtr camera) {
    if (!render_settings || !camera) {
        throw Exception("DefaultMeshRenderer::DefaultMeshRenderer()",
            "Not initialized object.");
    }

    render_settings_ = render_settings;
    camera_ = camera;

    loadShaders();
}

void DefaultMeshRenderer::render(FrameBufferPtr frame_buffer, MeshPtr mesh) {
    if (!frame_buffer || !mesh) {
        logError("DefaultMeshRenderer::render()", "Null input.");
        return;
    }

    FrameBuffer::setViewportSize(frame_buffer);
    frame_buffer->bind(FrameBufferBindType::NORMAL);

    default_shader_program_->activate();
    setShadersUniforms(mesh);

    DepthTest::instance().enable(true);
    DepthTest::instance().enableDepthMask(true);
    FaceCull::instance().enable(true);

    if (render_settings_->lighting()->isShadowMappingEnabled()) {
        Texture::setTextureSlot(6);
        renderers_shared_data_->shadow_map_texture->bind();
    }

    // First pass - skip transparent entities
    std::vector<GLuint> skipped_en;

    for (GLuint i = 0; i < mesh->getEntitiesCount(); i++) {
        auto entity = mesh->getEntity(i);
        auto material = entity->getMaterial();
        if (material && material->hasTransparency()) {
            skipped_en.push_back(i);
            continue;
        }

        drawMesh(mesh, i);
    }

    // Second pass - draw transparent entities
    // TODO: Sort them in order.

    AlphaBlend::instance().enable(true);
    AlphaBlend::instance().setBlendFunction(BlendFunction::NORMAL);

    for (const auto &i : skipped_en) {
        auto entity = mesh->getEntity(i);
        drawMesh(mesh, i);
    }

    AlphaBlend::instance().enable(false);
}

void DefaultMeshRenderer::loadShaders() {
    default_shader_program_.reset(new ShaderProgram());
    default_shader_program_->loadShaders("Shaders/Mesh.vert",
        "Shaders/Mesh.frag");
}

void DefaultMeshRenderer::setShadersUniforms(MeshPtr mesh) {
    if (!mesh) {
        logError("DefaultMeshRenderer::setShadersUniforms()", "Null input.");
        return;
    }

    default_shader_program_->setUniform("matrices.view_matrix",
        camera_->getViewMatrix());
    default_shader_program_->setUniform("matrices.projection_matrix",
        camera_->getProjectionMatrix());
    default_shader_program_->setUniform("matrices.model_matrix",
        mesh->getModelMatrix());
    default_shader_program_->setUniform("matrices.normal_matrix",
        mesh->getNormalMatrix());

    auto lighting = render_settings_->lighting();
    default_shader_program_->setUniform("lighting.enabled",
        lighting->isEnabled());

    default_shader_program_->setUniform("lighting.directional_light.enabled",
        lighting->directionalLight()->isEnabled());
    default_shader_program_->setUniform("lighting.directional_light.direction",
        lighting->directionalLight()->getDirection());
    default_shader_program_->setUniform(
        "lighting.directional_light.ambient_color",
        lighting->directionalLight()->getAmbientColor());
    default_shader_program_->setUniform(
        "lighting.directional_light.diffuse_color",
        lighting->directionalLight()->getDiffuseColor());
    default_shader_program_->setUniform(
        "lighting.directional_light.specular_color",
        lighting->directionalLight()->getSpecularColor());
    default_shader_program_->setUniform("lighting.blinn_phong",
        lighting->isBlinnPhongEnabled());

    default_shader_program_->setUniform("other.gamma",
        render_settings_->getGamma());
    default_shader_program_->setUniform("other.bloom_threshold_color",
        render_settings_->postprocess()->getGlowBloomThresholdColor());

    // Shadow mapping
    default_shader_program_->setUniform("shadow_mapping.enabled",
        render_settings_->lighting()->isShadowMappingEnabled());
    if (render_settings_->lighting()->isShadowMappingEnabled()) {
        default_shader_program_->setUniform("shadow_mapping.shadow_map_texture",
            6);
        default_shader_program_->setUniform("matrices.dir_light_matrix",
            renderers_shared_data_->dir_light_space_matrix);
    }
}

void DefaultMeshRenderer::setMeshEntityShadersUniforms(MeshEntityPtr entity) {
    if (!entity) {
        logError("DefaultMeshRenderer::setMeshEntityShadersUniforms()",
            "Null input.");
        return;
    }

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

    default_shader_program_->setUniform("material.shininess",
        material->getShininess());
    default_shader_program_->setUniform("material.transparency",
        material->getTransparency());
}

void DefaultMeshRenderer::drawMesh(MeshPtr mesh, GLuint entity_index) {
    if (!mesh) {
        logError("DefaultMeshRenderer::drawMesh()", "Null input.");
        return;
    }

    mesh->bind();

    auto entity = mesh->getEntity(entity_index);
    if (entity) {
        setMeshEntityShadersUniforms(entity);

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

        glDrawElements(GL_TRIANGLES, entity->getIndicesCount(),
            GL_UNSIGNED_INT, reinterpret_cast<void*>((
                entity->getStartingIndex() * sizeof(GLuint))));
    }
}