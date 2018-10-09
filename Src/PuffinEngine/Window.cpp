#include "PuffinEngine/Window.hpp"

#include "PuffinEngine/Exception.hpp"

using namespace puffin;

Window::Window(ConfigurationPtr configuration) {
    if (!configuration) {
        throw Exception("Window::Window()", NULL_OBJ_MSG);
    }

    configuration_ = configuration;

    if (glfwInit() != GLFW_TRUE) {
        throw Exception("Window::Window()", 
            "Window manager initialization error.");
    }
}

Window::~Window() {
    glfwTerminate();
    handle_ = nullptr;
}

void Window::createWindow() {
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, configuration_->
        getOpenGLVersion().first);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, configuration_->
        getOpenGLVersion().second);
    glfwWindowHint(GLFW_SAMPLES, configuration_->getMsaaSamples());
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    handle_ = glfwCreateWindow(configuration_->getFrameResolution().first,
        configuration_->getFrameResolution().second, caption_.c_str(),
        fullscreen_ ? glfwGetPrimaryMonitor() : nullptr, nullptr);
    if (!handle_) {
        throw Exception("Window::createWindow()", "Creating window error.");
    }

    glfwMakeContextCurrent(handle_);
}

void Window::setCaption(std::string caption) {
    caption_ = caption;
    if (handle_) {
        glfwSetWindowTitle(handle_, caption_.c_str());
    }
}

std::string Window::getCaption() const {
    return caption_;
}

void Window::enableFullscreen(GLboolean state) {
    fullscreen_ = state;
}

GLboolean Window::isFullscreen() const {
    return fullscreen_;
}