/*
* Puffin OpenGL Engine ver. 2.0
* Coded by: Sebastian 'qbranchmaster' Tabaka
* Contact: sebastian.tabaka@outlook.com
*/

#include "PuffinEngine/System.hpp"

using namespace puffin;

std::string System::getGpuName() const {
    const GLubyte *data = glGetString(GL_RENDERER);
    return std::string(reinterpret_cast<const char*>(data));
}

std::string System::getGpuVendor() const {
    const GLubyte *data = glGetString(GL_VENDOR);
    return std::string(reinterpret_cast<const char*>(data));
}

std::string System::getGlslVersion() const {
    const GLubyte *data = glGetString(GL_SHADING_LANGUAGE_VERSION);
    return std::string(reinterpret_cast<const char*>(data));
}

GLushort System::getMonitorsCount() const {
    GLint count = 0;
    glfwGetMonitors(&count);
    return static_cast<GLushort>(count);
}

std::string System::getMonitorName(GLushort index) const {
    GLint count = 0;
    GLFWmonitor** monitors = glfwGetMonitors(&count);
    if (index >= count) {
        logError("System::getMonitorName()", "Invalid value.");
        return std::string("");
    }

    auto tmp_name = glfwGetMonitorName(monitors[index]);
    return std::string(tmp_name);
}

std::pair<GLuint, GLuint> System::getMonitorSize(GLushort index) const {
    GLint count = 0;
    GLFWmonitor** monitors = glfwGetMonitors(&count);
    if (index >= count) {
        logError("System::getMonitorSize()", "Invalid value.");
        return std::make_pair(0, 0);
    }

    auto vid_mode = glfwGetVideoMode(monitors[index]);
    return std::make_pair(vid_mode->width, vid_mode->height);
}

std::vector<GLushort> System::getSupportedMsaaSamples() const {
    return {2, 4, 8, 16};
}

void System::initGlfw() const {
    if (glfwInit() != GLFW_TRUE) {
        throw Exception("System::initGlfw()", "Window manager initialization error.");
    }
}

void System::terminateGlfw() const {
    glfwTerminate();
}

void System::initGl() const {
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        throw Exception("System::initGl()", "OpenGL core initialization error.");
    }
}