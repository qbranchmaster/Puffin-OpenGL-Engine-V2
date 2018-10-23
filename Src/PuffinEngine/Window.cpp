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

    if (glfwInit() != GLFW_TRUE) {
        throw Exception("Window::Window()",
            "Window manager initialization error.");
    }
}

Window::~Window() {
    glfwTerminate();
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
        configuration_->isFullscreenEnabled() ? glfwGetPrimaryMonitor() :
        nullptr, nullptr);
    if (!handle_) {
        throw Exception("Window::createWindow()", "Creating window error.");
    }

    glfwMakeContextCurrent(handle_);
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

    GLFWimage img;
    auto size = icon.getSize();
    img.width = size.first;
    img.height = size.second;
    img.pixels = icon.getRawData();

    glfwSetWindowIcon(handle_, 1, &img);
}