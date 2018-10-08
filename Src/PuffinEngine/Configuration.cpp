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

void Configuration::setFrameResolution(GLuint width, GLuint height) {
    frame_width_ = width;
    frame_height_ = height;
}

std::pair<GLuint, GLuint> Configuration::getFrameResolution() const {
    return std::make_pair(frame_width_, frame_height_);
}