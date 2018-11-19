/*
* Puffin OpenGL Engine ver. 2.0
* Created by: Sebastian 'qbranchmaster' Tabaka
*/

#include "PuffinEngine/Window.hpp"

#include "PuffinEngine/Configuration.hpp"
#include "PuffinEngine/Exception.hpp"
#include "PuffinEngine/Logger.hpp"
#include "PuffinEngine/System.hpp"
#include "PuffinEngine/Texture.hpp"

using namespace puffin;

void Window::setCaption(std::string caption) {
    if (!handle_) {
        logError("Window::setCaption()", "Window not created yet.");
        return;
    }

    caption_ = caption;
    glfwSetWindowTitle(handle_, caption_.c_str());
}

std::string Window::getCaption() const {
    return caption_;
}

void Window::setPosition(GLint x, GLint y) {
    glfwSetWindowPos(handle_, x, y);
}

std::pair<GLint, GLint> Window::getPosition() const {
    GLint x = 0, y = 0;
    glfwGetWindowPos(handle_, &x, &y);
    return std::make_pair(x, y);
}

void Window::setWindowIcon(std::string path) const {
    Texture icon;
    if (!icon.loadImage(path)) {
        logError("Window::setWindowIcon()", "Could not set window icon.");
        return;
    }

    icon.swapRedBlue();
    icon.flipHorizontal();
    icon.flipVertical();

    glfwSetWindowIcon(handle_, 1, &icon.toGlfwImage());
}

void Window::createWindow() {
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
    auto wnd_size = Configuration::instance().getFrameResolution();
    handle_ = glfwCreateWindow(wnd_size.first, wnd_size.second,
        caption_.c_str(), Configuration::instance().isFullscreenEnabled() ?
        target_monitor : nullptr, nullptr);
    if (!handle_) {
        throw Exception("Window::createWindow()", "Creating window error.");
    }

    glfwMakeContextCurrent(handle_);
}

void Window::destroyWindow() {
    if (handle_) {
        glfwDestroyWindow(handle_);
    }
}