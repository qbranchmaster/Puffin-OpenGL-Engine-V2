#include "PuffinEngine/MasterRenderer.hpp"

#include "PuffinEngine/Exception.hpp"
#include "PuffinEngine/Logger.hpp"

using namespace puffin;

MasterRenderer::MasterRenderer(WindowPtr window, TimePtr time,
    SystemPtr system) {
    if (!window || !time || !system) {
        throw Exception("MasterRenderer::MasterRenderer()",
            "Not initialized object.");
    }

    target_window_ = window;
    time_ = time;
    system_ = system;

    target_window_->createWindow();
    system_->initGl();

    logInfo("MasterRenderer::MasterRenderer()", "GPU Vendor: " +
        system_->getGpuVendor());
    logInfo("MasterRenderer::MasterRenderer()", "GPU Name: " +
        system_->getGpuName());
    logInfo("MasterRenderer::MasterRenderer()", "GLSL Version: " +
        system_->getGlslVersion());

    for (GLushort i = 0; i < system_->getMonitorsCount(); i++) {
        logInfo("MasterRenderer::MasterRenderer()", "Monitor #" +
            std::to_string(i) + " name: " + system_->getMonitorName(i) +
            ", size = " + std::to_string(system_->getMonitorSize(i).first) +
            "x" + std::to_string(system_->getMonitorSize(i).second));
    }
}

void MasterRenderer::start() {
    rendering_enabled_ = true;
    while (rendering_enabled_) {
        time_->startDeltaMeasure();

        if (rendering_function_) {
            rendering_function_();
        }

        target_window_->pollEvents();
        target_window_->swapBuffers();

        time_->endDeltaMeasure();
        time_->update();

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