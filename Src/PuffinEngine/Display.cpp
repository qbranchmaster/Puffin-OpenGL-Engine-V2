#include "PuffinEngine/Display.hpp"

#include "PuffinEngine/Exception.hpp"
#include "PuffinEngine/Logger.hpp"

using namespace puffin;

Display::Display(ConfigurationPtr configuration, SystemPtr system) {
    if (!configuration || !system) {
        throw Exception("Display::Display()", NULL_OBJ_MSG);
    }

    system_ = system;

    system_->initializeGlfw();

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 
        configuration->getOpenGLVersion().first);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,
        configuration->getOpenGLVersion().second);
    glfwWindowHint(GLFW_SAMPLES, configuration->getMsaaSamples());
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    handle_ = glfwCreateWindow(configuration->getDisplaySize().first,
        configuration->getDisplaySize().second,
        configuration->getWindowCaption().c_str(),
        configuration->getDisplayFullscreen() ? glfwGetPrimaryMonitor() :
        nullptr, nullptr);
    if (!handle_) {
        throw Exception("Display::Display()", "Creating window error.");
    }

    glfwMakeContextCurrent(handle_);
    system_->initializeGlCore();
    
    logInfo("Display::Display()", "GPU Vendor: " + system->getGpuVendor());
    logInfo("Display::Display()", "GPU Name: " + system->getGpuName());
    logInfo("Display::Display()", "GLSL Version: " + system->getGlslVersion());
}

Display::~Display() {
    system_->terminateGlfw();
}