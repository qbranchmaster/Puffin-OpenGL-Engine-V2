/*
* Puffin OpenGL Engine ver. 2.0
* Coded by: Sebastian 'qbranchmaster' Tabaka
*/

#include "PuffinEngine/Window.hpp"

using namespace puffin;

Window::Window() {
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,
        Configuration::instance().getOpenGLVersion().first);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,
        Configuration::instance().getOpenGLVersion().second);
    glfwWindowHint(GLFW_SAMPLES, Configuration::instance().getMsaaSamples());
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLint count = 0;
    auto monitors = glfwGetMonitors(&count);
    auto target_monitor = monitors[Configuration::instance().
        getTargetMonitorIndex()];
    auto wnd_width = Configuration::instance().getFrameWidth();
    auto wnd_height = Configuration::instance().getFrameHeight();
    handle_ = glfwCreateWindow(wnd_width, wnd_height, caption_.c_str(),
        Configuration::instance().isFullscreenEnabled() ? target_monitor :
        nullptr, nullptr);
    if (!handle_) {
        throw Exception("Window::Window()", "Creating window error.");
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
        logError("Window::setWindowIcon()", "Invalid value.");
        return;
    }

    icon.swapRedBlue();
    icon.flipHorizontal();
    icon.flipVertical();

    glfwSetWindowIcon(handle_, 1, &icon.toGlfwImage());
}