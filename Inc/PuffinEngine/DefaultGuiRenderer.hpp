/*
* Puffin OpenGL Engine ver. 2.0
* Coded by: Sebastian 'qbranchmaster' Tabaka
*/

#ifndef PUFFIN_DEFAULT_GUI_RENDERER_HPP
#define PUFFIN_DEFAULT_GUI_RENDERER_HPP

#ifdef WIN32 // Prevents APIENTRY redefinition
#include <Windows.h>
#endif // WIN32
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <memory>

#include "imgui.h"
#include "examples/imgui_impl_glfw.h"
#include "examples/imgui_impl_opengl3.h"

#include "PuffinEngine/Camera.hpp"
#include "PuffinEngine/GuiRenderer.hpp"
#include "PuffinEngine/MasterRenderer.hpp"
#include "PuffinEngine/RenderersSharedData.hpp"
#include "PuffinEngine/RenderSettings.hpp"
#include "PuffinEngine/Window.hpp"

namespace puffin {
    class DefaultGuiRenderer : public GuiRenderer {
    public:
        DefaultGuiRenderer(RenderSettingsPtr render_settings,
            RenderersSharedDataPtr renderers_shared_data, WindowPtr window,
            CameraPtr camera, MasterRendererPtr master_renderer);

        void render();

        void enable(GLboolean state) {
            enabled_ = state;
        }

        GLboolean isCapturingMouse() const;

    private:
        void setupImGui();

        void renderMainMenuBar();
        void renderAboutDialog();


        void postprocessDialog();
        void lightingDialog();
        void shadowMappingDialog();
        void debugDialog();
        void cameraDialog();

        bool render_about_dialog_{false};

        GLboolean enabled_{true};

        CameraPtr camera_;
        MasterRendererPtr master_renderer_;
        RenderSettingsPtr render_settings_;
        RenderersSharedDataPtr renderers_shared_data_;
        WindowPtr target_window_;
    };

    using DefaultGuiRendererPtr = std::shared_ptr<DefaultGuiRenderer>;
} // namespace puffin

#endif // PUFFIN_DEFAULT_GUI_RENDERER_HPP