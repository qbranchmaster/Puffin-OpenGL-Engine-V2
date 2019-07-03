/*
 * Puffin OpenGL Engine ver. 2.1
 * Coded by: Sebastian 'qbranchmaster' Tabaka
 * Contact: sebastian.tabaka@outlook.com
 */

#include "PuffinEngine/DefaultPostprocessRenderer.hpp"

using namespace puffin;

DefaultPostprocessRenderer::DefaultPostprocessRenderer(PostprocessPtr postprocess) {
    postprocess_ = postprocess;

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
    default_shader_program_->loadShaders(
        "Data/Shaders/Postprocess.vert", "Data/Shaders/Postprocess.frag");

    bloom_shader_program_.reset(new ShaderProgram("bloom_shader_program"));
    bloom_shader_program_->loadShaders(
        "Data/Shaders/Postprocess.vert", "Data/Shaders/PostprocessGlowBloom.frag");
}

void DefaultPostprocessRenderer::setShadersUniforms(CameraPtr camera) {
    default_shader_program_->setUniform(
        "color.effect", static_cast<GLint>(postprocess_->getEffect()));
    default_shader_program_->setUniform("color.kernel_size", postprocess_->getKernelSize());
    default_shader_program_->setUniform("color.tint_color", postprocess_->getTintColor());
    default_shader_program_->setUniform("color.screen_texture", 0);
    default_shader_program_->setUniform("color.depth_texture", 1);
    default_shader_program_->setUniform("color.gamma", postprocess_-> getGamma());
    default_shader_program_->setUniform("color.exposure", postprocess_->getExposure());
    default_shader_program_->setUniform(
        "color.glow_bloom_enabled", postprocess_->isGlowBloomEnabled());
    default_shader_program_->setUniform("color.dof_enabled", postprocess_->isDepthOfFieldEnabled());

    if (postprocess_->isGlowBloomEnabled()) {
        default_shader_program_->setUniform("color.glow_bloom_texture", 2);
    }

    if (postprocess_->isDepthOfFieldEnabled()) {
        default_shader_program_->setUniform("color.aperture", postprocess_->getAperture());
        default_shader_program_->setUniform(
            "color.focus_distance", postprocess_->getFocusDistance());
        default_shader_program_->setUniform(
            "color.dof_max_blur", postprocess_->getDepthOfFieldMaxBlur());
        default_shader_program_->setUniform("color.camera_aspect", camera->getAspect());
    }

    default_shader_program_->setUniform("chromatic_aberration.enabled", postprocess_->isChromaticAberrationEnabled());

    if (postprocess_->isChromaticAberrationEnabled()) {
        default_shader_program_->setUniform("chromatic_aberration.lens_texture", 3);
        default_shader_program_->setUniform("chromatic_aberration.max_channels_offset",
            postprocess_->getChromaticAberrationMaxChannelsOffset());
    }
}

void DefaultPostprocessRenderer::createScreenMesh() {
    screen_mesh_.reset(new Mesh());
    // clang-format off
    std::vector<GLfloat> positions = {
        -1.0f,  1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f,
         1.0f, -1.0f, 0.0f,
         1.0f,  1.0f, 0.0f,
    };

    std::vector<GLfloat> texture_coords = {
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
    };
    // clang-format on

    std::vector<GLuint> indices = {0, 1, 3, 3, 1, 2};

    screen_mesh_->bind();
    screen_mesh_->setMeshData(positions, 0, 3);
    screen_mesh_->setMeshData(texture_coords, 1, 2);
    screen_mesh_->setMeshIndices(indices);
}

void DefaultPostprocessRenderer::drawMesh(MeshPtr mesh) {
    if (!mesh) {
        logError("DefaultPostprocessRenderer::drawMesh()", PUFFIN_MSG_NULL_OBJECT);
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

void DefaultPostprocessRenderer::render(FrameBufferPtr frame_buffer, ScenePtr scene) {
    if (!frame_buffer) {
        logError("DefaultPostprocessRenderer::render()", PUFFIN_MSG_NULL_OBJECT);
        return;
    }

    if (postprocess_->isGlowBloomEnabled()) {
        renderGlowBloom(frame_buffer);
    }

    FrameBuffer::unbindAll();
    FrameBuffer::setViewportSize(
        InitConfig::instance().getFrameWidth(), InitConfig::instance().getFrameHeight());
    FrameBuffer::clear(FrameBufferClearType::DepthAndColor, glm::vec3(0.0f, 0.0f, 0.0f));

    default_shader_program_->activate();
    setShadersUniforms(scene->camera());

    Texture::setTextureSlot(0);
    frame_buffer->getTextureBuffer(0)->bind();
    Texture::setTextureSlot(1);
    frame_buffer->getDepthTextureBuffer()->bind();
    Texture::setTextureSlot(2);
    if (postprocess_->isGlowBloomEnabled()) {
        bloom_frame_buffer_[0]->getTextureBuffer(0)->bind();
    }
    else {
        Texture::unbindTextureType(TextureType::Texture2D);
    }

    Texture::setTextureSlot(3);
    postprocess_->getChromaticAberrationLensTexture()->bind();

    DepthTest::instance().enable(false);
    DepthTest::instance().enableDepthMask(false);

    drawMesh(screen_mesh_);
}