#include "PuffinEngine/Window.hpp"

#include "PuffinEngine/Exception.hpp"
#include "PuffinEngine/Logger.hpp"
#include "PuffinEngine/Texture.hpp"

using namespace puffin;

Window::Window(ConfigurationPtr configuration) {
    if (!configuration) {
        throw Exception("Window::Window()", "Not initialized object.");
    }

    configuration_ = configuration;
}

void Window::createWindow() {
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, configuration_->
        getOpenGLVersion().first);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, configuration_->
        getOpenGLVersion().second);
    glfwWindowHint(GLFW_SAMPLES, configuration_->getMsaaSamples());
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLint count = 0;
    auto monitors = glfwGetMonitors(&count);
    auto target_monitor = monitors[configuration_->getTargetMonitorIndex()];
    handle_ = glfwCreateWindow(configuration_->getFrameResolution().first,
        configuration_->getFrameResolution().second, caption_.c_str(),
        configuration_->isFullscreenEnabled() ? target_monitor : nullptr,
        nullptr);
    if (!handle_) {
        throw Exception("Window::createWindow()", "Creating window error.");
    }

    glfwMakeContextCurrent(handle_);
    getPosition();
}

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

void Window::setWindowIcon(std::string path) {
    Texture icon;
    if (!icon.loadImage(path)) {
        return;
    }

    icon.swapRedBlue();
    icon.flipHorizontal();
    icon.flipVertical();

    GLFWimage img;
    auto size = icon.getSize();
    img.width = size.first;
    img.height = size.second;
    img.pixels = icon.getRawData();

    glfwSetWindowIcon(handle_, 1, &img);
}

void Window::setWindowCursor(std::string path) {
    Texture cursor;
    if (!cursor.loadImage(path)) {
        return;
    }

    // TODO: Finish this function, cursor is not working correctly.

    //cursor.flipHorizontal();
    cursor.flipVertical();

    GLFWimage cur;
    auto size = cursor.getSize();
    cur.width = size.first;
    cur.height = size.second;
    cur.pixels = cursor.getRawData();

    double x_pos, y_pos;
    glfwGetCursorPos(handle_, &x_pos, &y_pos);
    GLFWcursor *c = glfwCreateCursor(&cur, x_pos, y_pos);
    glfwSetCursor(handle_, c);
}

void Window::setPosition(GLint x, GLint y) {
    glfwSetWindowPos(handle_, x, y);
}

std::pair<GLint, GLint> Window::getPosition() {
    GLint x, y;
    glfwGetWindowPos(handle_, &x, &y);
    position_ = std::make_pair(x, y);
    return position_;
}