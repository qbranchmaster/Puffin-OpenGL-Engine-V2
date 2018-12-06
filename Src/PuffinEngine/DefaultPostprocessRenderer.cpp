/*
* Puffin OpenGL Engine ver. 2.0
* Coded by: Sebastian 'qbranchmaster' Tabaka
*/

#include "PuffinEngine/DefaultPostprocessRenderer.hpp"

using namespace puffin;

DefaultPostprocessRenderer::DefaultPostprocessRenderer(
    RenderSettingsPtr render_settings) {
    if (!render_settings) {
        throw Exception(
            "DefaultPostprocessRenderer::DefaultPostprocessRenderer()",
            "Not initialized object.");
    }

    render_settings_ = render_settings;

    loadShaders();
    // TODO: ---
    createScreenMesh();
    // ---
}

void DefaultPostprocessRenderer::loadShaders() {
    default_shader_program_.reset(new ShaderProgram());
    default_shader_program_->loadShaders("Shaders/Postprocess.vert",
        "Shaders/Postprocess.frag");
}

void DefaultPostprocessRenderer::setShadersUniforms() {
    // TODO: ---
    if (1) {
        default_shader_program_->setUniform("color.effect",
            static_cast<GLint>(render_settings_->postprocess()->getEffect()));
        default_shader_program_->setUniform("color.kernel_size",
            render_settings_->postprocess()->getKernelSize());
        default_shader_program_->setUniform("color.tint_color",
            render_settings_->postprocess()->getTintColor());
        default_shader_program_->setUniform("color.screen_texture", 0);
    }
    // ---
}
// TODO: ---
void DefaultPostprocessRenderer::createScreenMesh() {
    screen_mesh_.reset(new Mesh());
    std::vector<GLfloat> positions = {
        -1.0f,  1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f,
        1.0f,  1.0f, 0.0f,
        1.0f,  1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f
    };

    std::vector<GLfloat> texture_coords = {
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 1.0f,
        1.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f
    };

    screen_mesh_->bind();
    screen_mesh_->setMeshData(positions, 0, 3);
    screen_mesh_->setMeshData(texture_coords, 1, 2);
    screen_mesh_->unbind();

    auto entity = screen_mesh_->addEntity();
    entity->setVerticesCount(6);
}
// ---

void DefaultPostprocessRenderer::render(FrameBufferPtr frame_buffer) {
    // TODO: ----
    frame_buffer->unbind();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    default_shader_program_->activate();
    setShadersUniforms();

    auto i = frame_buffer->getRgbBufferTexture()->getHandle();
    glBindTexture(GL_TEXTURE_2D, i);
    render_settings_->depthTest()->enable(false);
    render_settings_->depthTest()->enableDepthMask(false);
    screen_mesh_->bind();
    screen_mesh_->draw(0);
    // ---
}