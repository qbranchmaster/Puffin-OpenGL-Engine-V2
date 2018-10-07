#include "PuffinEngine/System.hpp"

#include "PuffinEngine/Exception.hpp"

using namespace puffin;

void System::initializeGlfw() {
    if (glfwInit() != GLFW_TRUE) {
        throw Exception("System::initializeGlfw()", 
            "Window manager initialization error.");
    }
}

void System::terminateGlfw() {
    glfwTerminate();
}

void System::initializeGlCore() {
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        throw Exception("System::initializeGlCore()", 
            "OpenGL core initialization error.");
    }
}

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