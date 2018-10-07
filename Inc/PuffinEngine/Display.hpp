#ifndef PUFFIN_DISPLAY_HPP
#define PUFFIN_DISPLAY_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <memory>

#include "PuffinEngine/Configuration.hpp"
#include "PuffinEngine/System.hpp"

namespace puffin {
    class Display {
    public:
        Display(ConfigurationPtr configuration, SystemPtr system);
        ~Display();

    private:
        GLFWwindow *handle_{nullptr};

        SystemPtr system_;
    };

    using DisplayPtr = std::shared_ptr<Display>;
} // namespace puffin 

#endif // PUFFIN_DISPLAY_HPP