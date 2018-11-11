/*
* Puffin OpenGL Engine ver. 2.0
* Created by: Sebastian 'qbranchmaster' Tabaka
*/

#ifndef PUFFIN_WINDOW_HPP
#define PUFFIN_WINDOW_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <memory>
#include <string>

namespace puffin {
    class Window {
        friend class Input;
        friend class MasterRenderer;

    public:
        void setCaption(std::string caption);
        std::string getCaption() const;

        void setWindowIcon(std::string path) const;

        void setPosition(GLint x, GLint y);
        std::pair<GLint, GLint> getPosition() const;

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
    };

    using WindowPtr = std::shared_ptr<Window>;
} // namespace puffin

#endif // PUFFIN_WINDOW_HPP