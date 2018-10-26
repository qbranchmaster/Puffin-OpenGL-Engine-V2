#ifndef PUFFIN_WINDOW_HPP
#define PUFFIN_WINDOW_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <memory>
#include <string>

#include "PuffinEngine/Configuration.hpp"

namespace puffin {
    class Window {
        friend class Input;
        friend class MasterRenderer;

    public:
        Window(ConfigurationPtr configuration);
        ~Window();

        void setCaption(std::string caption);
        std::string getCaption() const;

        void setWindowIcon(std::string path);

    private:
        void createWindow();

        GLboolean isClosing() const {
            if (glfwWindowShouldClose(handle_)) {
                return true;
            }

            return false;
        }

        void pollEvents() const {
            glfwPollEvents();
        }

        void swapBuffers() const {
            glfwSwapBuffers(handle_);
        }

        GLFWwindow *handle_{nullptr};
        std::string caption_{"Puffin Window"};

        ConfigurationPtr configuration_;
    };

    using WindowPtr = std::shared_ptr<Window>;
} // namespace puffin 

#endif // PUFFIN_WINDOW_HPP