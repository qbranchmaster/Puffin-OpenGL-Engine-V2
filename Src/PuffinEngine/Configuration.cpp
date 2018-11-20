/*
* Puffin OpenGL Engine ver. 2.0
* Created by: Sebastian 'qbranchmaster' Tabaka
*/

#include "PuffinEngine/Configuration.hpp"

#include "PuffinEngine/Logger.hpp"
#include "PuffinEngine/System.hpp"

using namespace puffin;

void Configuration::setFrameResolution(GLuint width, GLuint height) {
    frame_width_ = width;
    frame_height_ = height;
}

std::pair<GLuint, GLuint> Configuration::getFrameResolution() const {
    return std::make_pair(frame_width_, frame_height_);
}

void Configuration::setMsaaSamples(GLushort samples) {
    auto supported_msaa = System::instance().getSupportedMsaaSamples();
    if (std::find(supported_msaa.begin(), supported_msaa.end(), samples) ==
        supported_msaa.end()) {
        logError("Configuration::setMsaaSamples()",
            "Invalid MSAA samples value.");
        return;
    }

    msaa_samples_ = samples;
}

GLushort Configuration::getMsaaSamples() const {
    return msaa_samples_;
}

void Configuration::setOpenGLVersion(GLushort major, GLushort minor) {
    // TODO: Verify if version is correct.
    gl_version_ = std::make_pair(major, minor);
}

std::pair<GLushort, GLushort> Configuration::getOpenGLVersion() const {
    return gl_version_;
}

void Configuration::enableFullscreen(GLboolean enabled) {
    fullscreen_ = enabled;
}

GLboolean Configuration::isFullscreenEnabled() const {
    return fullscreen_;
}

void Configuration::setTargetMonitorIndex(GLushort index) {
    if (index >= System::instance().getMonitorsCount()) {
        logError("Configuration::setTargetMonitorIndex()",
            "Monitor index out of range.");
        return;
    }

    monitor_index_ = index;
}

GLushort Configuration::getTargetMonitorIndex() const {
    return monitor_index_;
}

GLfloat Configuration::getFrameAspect() const {
    return static_cast<GLfloat>(frame_width_) /
        static_cast<GLfloat>(frame_height_);
}