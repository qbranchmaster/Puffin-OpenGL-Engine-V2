/*
 * Puffin OpenGL Engine ver. 2.1
 * Coded by: Sebastian 'qbranchmaster' Tabaka
 * Contact: sebastian.tabaka@outlook.com
 */

#include "PuffinEngine/Window.hpp"

using namespace puffin;

Window::Window() {
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    auto gl_version = InitConfig::instance().getOpenGLVersion();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, gl_version.first);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, gl_version.second);
    glfwWindowHint(GLFW_SAMPLES, InitConfig::instance().getMsaaSamples());
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLint count = 0;
    auto monitors = glfwGetMonitors(&count);
    auto target_monitor = monitors[InitConfig::instance().getTargetMonitorIndex()];
    auto wnd_width = InitConfig::instance().getFrameWidth();
    auto wnd_height = InitConfig::instance().getFrameHeight();
    handle_ = glfwCreateWindow(wnd_width, wnd_height, caption_.c_str(),
        InitConfig::instance().isFullscreenEnabled() ? target_monitor : nullptr, nullptr);
    if (!handle_) {
        throw Exception("Window::Window()", "Creating renderer window error.");
    }

    glfwMakeContextCurrent(handle_);
}

Window::~Window() {
    if (handle_) {
        glfwDestroyWindow(handle_);
    }
}

void Window::setCaption(std::string caption) {
    caption_ = caption;
    glfwSetWindowTitle(handle_, caption_.c_str());
}

void Window::setPosition(GLint x, GLint y) {
    glfwSetWindowPos(handle_, x, y);
}

std::pair<GLint, GLint> Window::getPosition() const {
    GLint x = 0, y = 0;
    glfwGetWindowPos(handle_, &x, &y);
    return std::make_pair(x, y);
}

void Window::setWindowIcon(std::string path) {
    Texture icon;
    if (!icon.loadImageRaw(path)) {
        logError("Window::setWindowIcon()", PUFFIN_MSG_FILE_CANNOT_OPEN(path));
        return;
    }

    icon.swapRedBlue();
    icon.flipHorizontal();
    icon.flipVertical();
    glfwSetWindowIcon(handle_, 1, &icon.toGlfwImage());
}