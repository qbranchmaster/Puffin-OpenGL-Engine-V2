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

    frame_buffer->bind(FrameBufferBindType::NORMAL);

    default_shader_program_->activate();
    setShadersUniforms(mesh);

    DepthTest::instance().enable(true);
    DepthTest::instance().enableDepthMask(true);
    FaceCull::instance().enable(true);

    for (GLuint i = 0; i < mesh->getEntitiesCount(); i++) {
        auto entity = mesh->getEntity(i);

        Texture::setTextureSlot(0);
        auto material = entity->getMaterial();
        if (material) {
            auto diffuse_texture = material->getDiffuseTexture();
            if (diffuse_texture) {
                diffuse_texture->bind();
            }
            else {
                // TODO: Unbind texture.
            }
        }

        drawMesh(mesh, i);
    }
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

    default_shader_program_->setUniform("object_material.diffuse_texture", 0);
}

void DefaultMeshRenderer::drawMesh(MeshPtr mesh, GLuint entity_index) {
    if (!mesh) {
        logError("DefaultMeshRenderer::drawMesh()", "Null input.");
        return;
    }

    mesh->bind();

    auto entity = mesh->getEntity(entity_index);
    if (entity) {
        glDrawElements(GL_TRIANGLES, entity->getIndicesCount(),
            GL_UNSIGNED_INT, nullptr);
    }

    mesh->unbind();
}