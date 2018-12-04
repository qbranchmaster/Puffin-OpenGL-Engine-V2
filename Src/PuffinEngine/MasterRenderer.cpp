/*
* Puffin OpenGL Engine ver. 2.0
* Created by: Sebastian 'qbranchmaster' Tabaka
*/

#include "PuffinEngine/MasterRenderer.hpp"

#include "PuffinEngine/Configuration.hpp"
#include "PuffinEngine/Exception.hpp"
#include "PuffinEngine/Logger.hpp"
#include "PuffinEngine/System.hpp"
#include "PuffinEngine/Time.hpp"

using namespace puffin;

MasterRenderer::MasterRenderer(WindowPtr window, CameraPtr camera,
    RenderSettingsPtr render_settings) {
    if (!window || !camera || !render_settings) {
        throw Exception("MasterRenderer::MasterRenderer()",
            "Not initialized object.");
    }

    camera_ = camera;
    target_window_ = window;
    render_settings_ = render_settings;

    logInfo("MasterRenderer::MasterRenderer()", "GPU Vendor: " +
        System::instance().getGpuVendor());
    logInfo("MasterRenderer::MasterRenderer()", "GPU Name: " +
        System::instance().getGpuName());
    logInfo("MasterRenderer::MasterRenderer()", "GLSL Version: " +
        System::instance().getGlslVersion());

    for (GLushort i = 0; i < System::instance().getMonitorsCount(); i++) {
        logInfo("MasterRenderer::MasterRenderer()", "Monitor #" +
            std::to_string(i) + " name: " +
            System::instance().getMonitorName(i) + ", size = " +
            std::to_string(System::instance().getMonitorSize(i).first) +
            "x" + std::to_string(System::instance().getMonitorSize(i).second));
    }

    skybox_renderer_.reset(new SkyboxRenderer(render_settings_, camera_));

    createFrameBuffer();
    loadShaders();

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

    screen_mesh_->setMeshData(positions, 0, 3);
    screen_mesh_->setMeshData(texture_coords, 1, 2);

    auto entity = screen_mesh_->addEntity();
    entity->setVerticesCount(6);
}

void MasterRenderer::start() {
    rendering_enabled_ = true;
    while (rendering_enabled_) {
        Time::instance().startDeltaMeasure();

        if (rendering_function_) {
            rendering_function_();
        }

        if (postprocess_renderer_) {
            postprocess_renderer_->render();
        }

        target_window_->swapBuffers();
        target_window_->pollEvents();

        Time::instance().endDeltaMeasure();
        Time::instance().update();

        camera_->updateSpeed(Time::instance().getDelta());

        if (target_window_->isClosing()) {
            stop();
        }
    }
}

void MasterRenderer::stop() {
    rendering_enabled_ = false;
}

void MasterRenderer::assignRenderingFunction(std::function<void()> function) {
    if (!function) {
        logError("MasterRenderer::assignRenderingFunction()",
            "Null rendering function.");
        return;
    }

    rendering_function_ = function;
}

void MasterRenderer::drawScene(ScenePtr scene) {
    if (!scene) {
        return;
    }

    default_frame_buffer_->bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    skybox_renderer_->render(scene->getSkybox());

    default_frame_buffer_->unbind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    default_shader_program_->activate();
    setShadersUniforms();
    glBindTexture(GL_TEXTURE_2D, default_frame_buffer_->getRgbBufferTexture()->getHandle());
    screen_mesh_->bind();
    screen_mesh_->draw(0);
}

void MasterRenderer::createFrameBuffer() {
    default_frame_buffer_.reset(new FrameBuffer());
    auto w = Configuration::instance().getFrameWidth();
    auto h = Configuration::instance().getFrameHeight();
    default_frame_buffer_->addTextureBuffer(w, h);
    default_frame_buffer_->addTextureBuffer(w, h);

    if (!default_frame_buffer_->isComplete()) {
        throw Exception("MasterRenderer::createFrameBuffer()",
            "Error creating default frame buffer.");
    }
}

void MasterRenderer::setShadersUniforms() {
    if (1) {
        default_shader_program_->setUniform("color.effect",
            static_cast<GLint>(render_settings_->postprocess()->getEffect()));
        default_shader_program_->setUniform("color.kernel_size",
            render_settings_->postprocess()->getKernelSize());
        default_shader_program_->setUniform("color.tint_color",
            render_settings_->postprocess()->getTintColor());
        default_shader_program_->setUniform("color.screen_texture", 0);
    }
}

void MasterRenderer::loadShaders() {
    default_shader_program_.reset(new ShaderProgram());
    default_shader_program_->loadShaders("Shaders/Master.vert",
        "Shaders/Master.frag");
}