#include "PuffinEngine/Configuration.hpp"

#include "PuffinEngine/Logger.hpp"

using namespace puffin;

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
    gl_version_ = std::make_pair(major, minor);
}

std::pair<GLushort, GLushort> Configuration::getOpenGLVersion() const {
    return gl_version_;
}

void Configuration::setWindowCaption(std::string caption) {
    window_caption_ = caption;
}

std::string Configuration::getWindowCaption() const {
    return window_caption_;
}

void Configuration::setDisplayFullscreen(GLboolean state) {
    display_fullscreen_ = state;
}

GLboolean Configuration::getDisplayFullscreen() const {
    return display_fullscreen_;
}

void Configuration::setDisplaySize(GLuint width, GLuint height) {
    display_width_ = width;
    display_height_ = height;
}

std::pair<GLuint, GLuint> Configuration::getDisplaySize() const {
    return std::make_pair(display_width_, display_height_);
}