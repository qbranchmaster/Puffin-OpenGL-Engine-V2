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

    frame_buffer->bind();

    default_shader_program_->activate();
    setShadersUniforms(mesh);

    render_settings_->depthTest()->enable(true);
    render_settings_->depthTest()->enableDepthMask(true);
    render_settings_->faceCull()->enable(true);

    Texture::setTextureSlot(0);
    Texture::unbindAllTextures(TextureType::Texture2D);

    drawMesh(mesh, 0);
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
}

void DefaultMeshRenderer::drawMesh(MeshPtr mesh, GLuint entity_index) {
    if (!mesh) {
        logError("DefaultMeshRenderer::drawMesh()", "Null input.");
        return;
    }

    mesh->bind();
    glDrawArrays(GL_TRIANGLES, 0,
        mesh->getEntity(entity_index)->getVerticesCount());
    mesh->unbind();
}