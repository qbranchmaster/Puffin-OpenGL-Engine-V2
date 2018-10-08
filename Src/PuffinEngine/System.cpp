#include "PuffinEngine/System.hpp"

using namespace puffin;

std::string System::getGpuVendor() const {
    const GLubyte *data = glGetString(GL_VENDOR);
    return std::string(reinterpret_cast<const char*>(data));
}

std::string System::getGpuName() const {
    const GLubyte *data = glGetString(GL_RENDERER);
    return std::string(reinterpret_cast<const char*>(data));
}

std::string System::getGlslVersion() const {
    const GLubyte *data = glGetString(GL_SHADING_LANGUAGE_VERSION);
    return std::string(reinterpret_cast<const char*>(data));
}