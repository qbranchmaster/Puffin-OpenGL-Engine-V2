/*
* Puffin OpenGL Engine ver. 2.0
* Created by: Sebastian 'qbranchmaster' Tabaka
*/

#include "PuffinEngine/MasterRenderer.hpp"

#include "PuffinEngine/Exception.hpp"
#include "PuffinEngine/Logger.hpp"
#include "PuffinEngine/System.hpp"
#include "PuffinEngine/Time.hpp"

using namespace puffin;

MasterRenderer::MasterRenderer(WindowPtr window, CameraPtr camera) {
    if (!window || !camera) {
        throw Exception("MasterRenderer::MasterRenderer()",
            "Not initialized object.");
    }

    camera_ = camera;
    target_window_ = window;

    target_window_->createWindow();
    System::instance().initGl();

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
}

MasterRenderer::~MasterRenderer() {
    target_window_->destroyWindow();
}

void MasterRenderer::start() {
    rendering_enabled_ = true;
    while (rendering_enabled_) {
        Time::instance().startDeltaMeasure();

        if (rendering_function_) {
            rendering_function_();
        }

        target_window_->pollEvents();
        target_window_->swapBuffers();

        Time::instance().endDeltaMeasure();
        Time::instance().update();

        camera_->update(Time::instance().getDelta());

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