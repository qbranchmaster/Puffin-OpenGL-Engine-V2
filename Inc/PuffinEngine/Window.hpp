/*
 * Puffin OpenGL Engine ver. 2.0.1
 * Coded by: Sebastian 'qbranchmaster' Tabaka
 * Contact: sebastian.tabaka@outlook.com
 */

#ifndef PUFFIN_WINDOW_HPP
#define PUFFIN_WINDOW_HPP

#ifdef WIN32 // Prevents APIENTRY redefinition
#include <Windows.h>
#endif // WIN32
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <memory>
#include <string>

#include "PuffinEngine/InitConfig.hpp"
#include "PuffinEngine/Logger.hpp"
#include "PuffinEngine/System.hpp"
#include "PuffinEngine/Texture.hpp"

namespace puffin {
    class Window {
        friend class DefaultGuiRenderer;
        friend class Input;
        friend class MasterRenderer;

    public:
        Window();
        ~Window();

        void setCaption(std::string caption);

        std::string getCaption() const {
            return caption_;
        }

        void setWindowIcon(std::string path);

        void setPosition(GLint x, GLint y);
        std::pair<GLint, GLint> getPosition() const;

    private:
        GLboolean isClosing() {
            if (glfwWindowShouldClose(handle_)) {
                return true;
            }

            return false;
        }

        void pollEvents() {
            glfwPollEvents();
        }

        void swapBuffers() {
            glfwSwapBuffers(handle_);
        }

        GLFWwindow *handle_{nullptr};
        std::string caption_{"Puffin Engine"};
    };

    using WindowPtr = std::shared_ptr<Window>;
} // namespace puffin

#endif // PUFFIN_WINDOW_HPP