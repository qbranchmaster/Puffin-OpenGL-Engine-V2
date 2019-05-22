/*
 * Puffin OpenGL Engine ver. 2.1
 * Coded by: Sebastian 'qbranchmaster' Tabaka
 * Contact: sebastian.tabaka@outlook.com
 */

#include "PuffinEngine/InitConfig.hpp"

using namespace puffin;

void InitConfig::setFrameResolution(GLuint width, GLuint height) {
    frame_width_ = width;
    frame_height_ = height;
}

void InitConfig::setMsaaSamples(GLushort samples) {
    auto supported_msaa = System::instance().getSupportedMsaaSamples();
    if (std::find(supported_msaa.begin(), supported_msaa.end(), samples) == supported_msaa.end()) {
        logError("Configuration::setMsaaSamples()", "Invalid value.");
        return;
    }

    msaa_samples_ = samples;
}

void InitConfig::setOpenGLVersion(GLushort major, GLushort minor) {
    // TODO: Verify if version is correct
    gl_version_ = std::make_pair(major, minor);
}

void InitConfig::enableFullscreen(GLboolean enabled) {
    fullscreen_ = enabled;
}

void InitConfig::setTargetMonitorIndex(GLushort index) {
    if (index >= System::instance().getMonitorsCount()) {
        logError("Configuration::setTargetMonitorIndex()", "Invalid value.");
        return;
    }

    monitor_index_ = index;
}