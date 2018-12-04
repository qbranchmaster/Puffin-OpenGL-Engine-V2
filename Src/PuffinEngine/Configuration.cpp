/*
* Puffin OpenGL Engine ver. 2.0
* Coded by: Sebastian 'qbranchmaster' Tabaka
*/

#include "PuffinEngine/Configuration.hpp"

using namespace puffin;

void Configuration::setFrameResolution(GLuint width, GLuint height) {
    frame_width_ = width;
    frame_height_ = height;
}

void Configuration::setMsaaSamples(GLushort samples) {
    auto supported_msaa = System::instance().getSupportedMsaaSamples();
    if (std::find(supported_msaa.begin(), supported_msaa.end(), samples) ==
        supported_msaa.end()) {
        logError("Configuration::setMsaaSamples()", "Invalid value.");
        return;
    }

    msaa_samples_ = samples;
}

void Configuration::setOpenGLVersion(GLushort major, GLushort minor) {
    // TODO: Verify if version is correct.
    gl_version_ = std::make_pair(major, minor);
}

void Configuration::enableFullscreen(GLboolean enabled) {
    fullscreen_ = enabled;
}

void Configuration::setTargetMonitorIndex(GLushort index) {
    if (index >= System::instance().getMonitorsCount()) {
        logError("Configuration::setTargetMonitorIndex()", "Invalid value.");
        return;
    }

    monitor_index_ = index;
}