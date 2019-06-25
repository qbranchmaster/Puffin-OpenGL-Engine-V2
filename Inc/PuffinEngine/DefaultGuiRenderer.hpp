/*
 * Puffin OpenGL Engine ver. 2.1
 * Coded by: Sebastian 'qbranchmaster' Tabaka
 * Contact: sebastian.tabaka@outlook.com
 */

#ifndef PUFFIN_DEFAULT_GUI_RENDERER_HPP
#define PUFFIN_DEFAULT_GUI_RENDERER_HPP

#ifdef WIN32 // Prevents APIENTRY redefinition
#include <Windows.h>
#endif // WIN32
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <memory>

// clang-format off
#include "imgui.h" // Must be before 'examples'
#include "examples/imgui_impl_glfw.h"
#include "examples/imgui_impl_opengl3.h"
#include "misc/cpp/imgui_stdlib.h"
// clang-format on

#include "PuffinEngine/Camera.hpp"
#include "PuffinEngine/DefaultMeshRenderer.hpp"
#include "PuffinEngine/DefaultShadowMapRenderer.hpp"
#include "PuffinEngine/DefaultSkyboxRenderer.hpp"
#include "PuffinEngine/DefaultWaterRenderer.hpp"
#include "PuffinEngine/GuiRenderer.hpp"
#include "PuffinEngine/MasterRenderer.hpp"
#include "PuffinEngine/RenderSettings.hpp"
#include "PuffinEngine/SceneLoader.hpp"
#include "PuffinEngine/Window.hpp"

namespace puffin {
    class DefaultGuiRenderer : public GuiRenderer {
    public:
        DefaultGuiRenderer(RenderSettingsPtr render_settings, WindowPtr window, CameraPtr camera,
            MasterRendererPtr master_renderer);

        void render(ScenePtr scene);

        void enable(GLboolean state) {
            enabled_ = state;
        }

        GLboolean isCapturingMouse() const;
        GLboolean isCapturingKeyboard() const;

    private:
        void setupImGui();
        void setupCustomStyle();

        void renderMainMenuBar();
        void renderAboutDialog();
        void renderCameraDialog();
        void renderPostprocessDialog();
        void renderLightingDialog();
        void renderShadowMappingDialog();
        void renderFogDialog();
        void renderCaptureDialog();

        void renderWaterRendererDialog(ScenePtr scene);
        void renderSkyboxRendererDialog(ScenePtr scene);
        void renderMeshRendererDialog(ScenePtr scene);

        void renderSaveSceneDialog(ScenePtr scene);
        void renderLoadSceneDialog(ScenePtr scene);

        bool render_about_dialog_{false};
        bool render_camera_dialog_{false};
        bool render_postprocess_dialog_{false};
        bool render_lighting_dialog_{false};
        bool render_shadow_map_dialog_{false};
        bool render_fog_dialog_{false};
        bool render_capture_dialog_{false};
        bool render_save_scene_dialog_{false};
        bool render_load_scene_dialog_{false};

        bool render_water_renderer_dialog_{false};
        bool render_skybox_renderer_dialog_{false};
        bool render_mesh_renderer_dialog_{false};

        GLboolean enabled_{true};

        CameraPtr camera_{nullptr};
        MasterRendererPtr master_renderer_{nullptr};
        RenderSettingsPtr render_settings_{nullptr};
        WindowPtr target_window_{nullptr};

        SceneLoaderPtr scene_loader_{nullptr};
    };

    using DefaultGuiRendererPtr = std::shared_ptr<DefaultGuiRenderer>;
} // namespace puffin

#endif // PUFFIN_DEFAULT_GUI_RENDERER_HPP