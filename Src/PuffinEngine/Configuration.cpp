#include "PuffinEngine/Configuration.hpp"

#include "PuffinEngine/Exception.hpp"
#include "PuffinEngine/Logger.hpp"

using namespace puffin;

Configuration::Configuration(SystemPtr system) {
    if (!system) {
        throw Exception("Configuration::Configuration()",
            "Not initialized object.");
    }

    system_ = system;
}

std::vector<GLushort> Configuration::getSupportedMsaaSamples() const {
    return {2, 4, 8, 16};
}

GLushort Configuration::getMsaaSamples() const {
    return msaa_samples_;
}

void Configuration::setMsaaSamples(GLushort samples) {
    auto supported_msaa = getSupportedMsaaSamples();
    if (std::find(supported_msaa.begin(), supported_msaa.end(), samples) ==
        supported_msaa.end()) {
        logError("Configuration::setMsaaSamples()",
            "Invalid MSAA samples value.");
        return;
    }

    msaa_samples_ = samples;
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
    if (index >= system_->getMonitorsCount()) {
        logError("Configuration::setTargetMonitorIndex()", 
            "Monitor index out of range.");
        return;
    }

    monitor_index_ = index;
}

GLushort Configuration::getTargetMonitorIndex() const {
    return monitor_index_;
}

void Configuration::setFrameResolution(GLuint width, GLuint height) {
    frame_width_ = width;
    frame_height_ = height;
}

std::pair<GLuint, GLuint> Configuration::getFrameResolution() const {
    return std::make_pair(frame_width_, frame_height_);
}