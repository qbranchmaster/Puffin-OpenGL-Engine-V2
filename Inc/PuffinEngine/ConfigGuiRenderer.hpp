/*
* Puffin OpenGL Engine ver. 2.0
* Coded by: Sebastian 'qbranchmaster' Tabaka
*/

#ifndef PUFFIN_CONFIG_GUI_RENDERER_HPP
#define PUFFIN_CONFIG_GUI_RENDERER_HPP

#ifdef WIN32 // Prevents APIENTRY redefinition
#include <Windows.h>
#endif // WIN32
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <memory>

#include "imgui.h"
#include "examples/imgui_impl_glfw.h"
#include "examples/imgui_impl_opengl3.h"

#include "PuffinEngine/Window.hpp"

namespace puffin {
    class ConfigGuiRenderer {
    public:
        ConfigGuiRenderer(WindowPtr window);

        void render();

    private:
        WindowPtr target_window_;
    };

    using ConfigGuiRendererPtr = std::shared_ptr<ConfigGuiRenderer>;
} // namespace puffin

#endif // PUFFIN_CONFIG_GUI_RENDERER_HPP