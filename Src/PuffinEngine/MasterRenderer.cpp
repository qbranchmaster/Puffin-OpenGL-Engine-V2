/*
* Puffin OpenGL Engine ver. 2.0
* Coded by: Sebastian 'qbranchmaster' Tabaka
*/

#include "PuffinEngine/MasterRenderer.hpp"

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

    createDefaultFrameBuffer();
}

void MasterRenderer::createDefaultFrameBuffer() {
    default_frame_buffer_.reset(new FrameBuffer());

    auto w = Configuration::instance().getFrameWidth();
    auto h = Configuration::instance().getFrameHeight();
    default_frame_buffer_->addTextureBuffer(w, h);
    default_frame_buffer_->addTextureBuffer(w, h);

    default_frame_buffer_->bind();

    if (!default_frame_buffer_->isComplete()) {
        throw Exception("MasterRenderer::createDefaultFrameBuffer()",
            "Error creating default frame buffer.");
    }

    default_frame_buffer_->unbind();
}

void MasterRenderer::stop() {
    rendering_enabled_ = false;
}

void MasterRenderer::start() {
    rendering_enabled_ = true;
    while (rendering_enabled_) {
        Time::instance().startDeltaMeasure();

        clearDefaultFrameBuffer();

        if (rendering_function_) {
            rendering_function_();
        }

        if (postprocess_renderer_) {
            postprocess_renderer_->render(default_frame_buffer_);
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

void MasterRenderer::assignRenderingFunction(std::function<void()> function) {
    if (!function) {
        logError("MasterRenderer::assignRenderingFunction()", "Null input.");
        return;
    }

    rendering_function_ = function;
}

void MasterRenderer::assignPostprocessRenderer(
    PostprocessRendererPtr renderer) {
    if (!renderer) {
        logError("MasterRenderer::assignPostprocessRenderer()", "Null input.");
        return;
    }

    postprocess_renderer_ = renderer;
}

void MasterRenderer::assignSkyboxRenderer(SkyboxRendererPtr renderer) {
    if (!renderer) {
        logError("MasterRenderer::assignSkyboxRenderer()", "Null input.");
        return;
    }

    skybox_renderer_ = renderer;
}

void MasterRenderer::drawScene(ScenePtr scene) {
    if (!scene) {
        logError("MasterRenderer::drawScene()", "Null input.");
    }

    if (skybox_renderer_) {
        skybox_renderer_->render(default_frame_buffer_, scene->getSkybox());
    }
}

void MasterRenderer::clearDefaultFrameBuffer() {
    default_frame_buffer_->bind();

    setClearColor(default_frame_buffer_->getBackgroundColor());
    clearFrameBuffer(true);

    default_frame_buffer_->unbind();
}