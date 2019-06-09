/*
* Puffin OpenGL Engine ver. 2.0
* Coded by: Sebastian 'qbranchmaster' Tabaka
*/

#include "PuffinEngine/DefaultPostprocessRenderer.hpp"

using namespace puffin;

DefaultPostprocessRenderer::DefaultPostprocessRenderer(
    RenderSettingsPtr render_settings, CameraPtr camera) {
    if (!render_settings || !camera) {
        throw Exception(
            "DefaultPostprocessRenderer::DefaultPostprocessRenderer()",
            "Not initialized object.");
    }

    camera_ = camera;
    render_settings_ = render_settings;

    loadShaders();
    createScreenMesh();

    bloom_frame_buffer_[0].reset(new FrameBuffer(
        InitConfig::instance().getFrameWidth(), InitConfig::instance().getFrameHeight()));
    bloom_frame_buffer_[0]->addTextureBuffer(0, false, true);

    bloom_frame_buffer_[1].reset(new FrameBuffer(
        InitConfig::instance().getFrameWidth(), InitConfig::instance().getFrameHeight()));
    bloom_frame_buffer_[1]->addTextureBuffer(0, false, true);
}

void DefaultPostprocessRenderer::loadShaders() {
    default_shader_program_.reset(new ShaderProgram("postprocess_shader_program"));
    default_shader_program_->loadShaders("Data/Shaders/Postprocess.vert",
        "Data/Shaders/Postprocess.frag");

    bloom_shader_program_.reset(new ShaderProgram("bloom_shader_program"));
    bloom_shader_program_->loadShaders("Data/Shaders/Postprocess.vert",
        "Data/Shaders/PostprocessGlowBloom.frag");
}

void DefaultPostprocessRenderer::setShadersUniforms() {
    default_shader_program_->setUniform("color.effect",
        static_cast<GLint>(render_settings_->postprocess()->getEffect()));
    default_shader_program_->setUniform("color.kernel_size",
        render_settings_->postprocess()->getKernelSize());
    default_shader_program_->setUniform("color.tint_color",
        render_settings_->postprocess()->getTintColor());
    default_shader_program_->setUniform("color.screen_texture", 0);
    default_shader_program_->setUniform("color.depth_texture", 1);
    default_shader_program_->setUniform("color.gamma", render_settings_->postprocess()->getGamma());
    default_shader_program_->setUniform("color.exposure", render_settings_->postprocess()->getExposure());
    default_shader_program_->setUniform("color.glow_bloom_enabled",
        render_settings_->postprocess()->isGlowBloomEnabled());
    default_shader_program_->setUniform("color.dof_enabled",
        render_settings_->postprocess()->isDepthOfFieldEnabled());

    if (render_settings_->postprocess()->isGlowBloomEnabled()) {
        default_shader_program_->setUniform("color.glow_bloom_texture", 2);
    }

    if (render_settings_->postprocess()->isDepthOfFieldEnabled()) {
        default_shader_program_->setUniform("color.aperture",
            render_settings_->postprocess()->getAperture());
        default_shader_program_->setUniform("color.focus_distance",
            render_settings_->postprocess()->getFocusDistance());
        default_shader_program_->setUniform("color.dof_max_blur",
            render_settings_->postprocess()->getDepthOfFieldMaxBlur());
        default_shader_program_->setUniform("color.camera_aspect",
            camera_->getAspect());
    }
}

void DefaultPostprocessRenderer::createScreenMesh() {
    screen_mesh_.reset(new Mesh());
    std::vector<GLfloat> positions = {
        -1.0f,  1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
    };

    std::vector<GLfloat> texture_coords = {
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
    };

    std::vector<GLuint> indices = {
        0, 1, 3, 3, 1, 2
    };

    screen_mesh_->bind();
    screen_mesh_->setMeshData(positions, 0, 3);
    screen_mesh_->setMeshData(texture_coords, 1, 2);
    screen_mesh_->setMeshIndices(indices);
}

void DefaultPostprocessRenderer::drawMesh(MeshPtr mesh) {
    if (!mesh) {
        logError("DefaultPostprocessRenderer::drawMesh()", "Null input.");
        return;
    }

    mesh->bind();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void DefaultPostprocessRenderer::renderGlowBloom(FrameBufferPtr frame_buffer) {
    GLboolean horizontal = true;
    GLboolean first = true;
    constexpr GLushort amount = 10;

    bloom_shader_program_->activate();
    Texture::setTextureSlot(0);

    // Clear framebuffers
    for (GLushort i = 0; i < 2; i++) {
        bloom_frame_buffer_[i]->bind(FrameBufferBindType::Normal);
        FrameBuffer::setViewportSize(bloom_frame_buffer_[i]);
        FrameBuffer::clear(FrameBufferClearType::OnlyColor);
    }

    for (GLushort i = 0; i < amount; i++) {
        bloom_frame_buffer_[horizontal ? 1 : 0]->bind(FrameBufferBindType::Normal);
        FrameBuffer::setViewportSize(bloom_frame_buffer_[horizontal ? 1 : 0]);
        bloom_shader_program_->setUniform("horizontal", horizontal);
        if (first) {
            frame_buffer->getTextureBuffer(1)->bind();
            first = false;
        }
        else {
            bloom_frame_buffer_[horizontal ? 0 : 1]->getTextureBuffer(0)->bind();
        }

        drawMesh(screen_mesh_);
        horizontal = !horizontal;
    }
}

void DefaultPostprocessRenderer::render(FrameBufferPtr frame_buffer) {
    if (!frame_buffer) {
        logError("DefaultPostprocessRenderer::render()", "Null input.");
        return;
    }

    if (render_settings_->postprocess()->isGlowBloomEnabled()) {
        renderGlowBloom(frame_buffer);
    }

    FrameBuffer::unbindAll();
    FrameBuffer::setViewportSize(
        InitConfig::instance().getFrameWidth(), InitConfig::instance().getFrameHeight());
    FrameBuffer::clear(FrameBufferClearType::DepthAndColor,
        glm::vec3(0.0f, 0.0f, 0.0f));

    default_shader_program_->activate();
    setShadersUniforms();

    Texture::setTextureSlot(0);
    frame_buffer->getTextureBuffer(0)->bind();
    Texture::setTextureSlot(1);
    frame_buffer->getDepthTextureBuffer()->bind();
    Texture::setTextureSlot(2);
    if (render_settings_->postprocess()->isGlowBloomEnabled()) {
        bloom_frame_buffer_[0]->getTextureBuffer(0)->bind();
    }
    else {
        Texture::unbindTextureType(TextureType::Texture2D);
    }

    DepthTest::instance().enable(false);
    DepthTest::instance().enableDepthMask(false);

    drawMesh(screen_mesh_);
}