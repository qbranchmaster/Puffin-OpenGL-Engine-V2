/*
 * Puffin OpenGL Engine ver. 2.0.1
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

#include <functional>

// clang-format off
#include "imgui.h" // Must be before 'examples'
#include "examples/imgui_impl_glfw.h"
#include "examples/imgui_impl_opengl3.h"
#include "misc/cpp/imgui_stdlib.h"
// clang-format on

#include "PuffinEngine/Camera.hpp"
#include "PuffinEngine/DefaultMeshRenderer.hpp"
#include "PuffinEngine/DefaultGizmoRenderer.hpp"
#include "PuffinEngine/DefaultShadowMapRenderer.hpp"
#include "PuffinEngine/DefaultSkyboxRenderer.hpp"
#include "PuffinEngine/DefaultWaterRenderer.hpp"
#include "PuffinEngine/GuiRenderer.hpp"
#include "PuffinEngine/SceneLoader.hpp"
#include "PuffinEngine/Window.hpp"
#include "PuffinEngine/Postprocess.hpp"

namespace puffin {
    class DefaultGuiRenderer : public GuiRenderer {
    public:
        DefaultGuiRenderer(WindowPtr window, PostprocessPtr postprocess,
            MeshRendererPtr mesh_renderer, SkyboxRendererPtr skybox_renderer,
            ShadowMapRendererPtr shadow_map_renderer, WaterRendererPtr water_renderer,
            GizmoRendererPtr gizmo_renderer);

        void render(ScenePtr scene);

        void bindQuitFunction(std::function<void()> function);
        void bindSaveCapture(std::function<void(std::string, bool)> function);

        GLboolean isCapturingMouse() const;
        GLboolean isCapturingKeyboard() const;

    private:
        void setupImGui();

        void renderMainMenuBar();
        void renderAboutDialog();
        void renderObjectInspectorDialog();
        void renderCameraDialog();
        void renderObjectInspectorMeshPart(BaseMeshPtr object);
        void renderObjectInspectorTransformPart(BaseMeshPtr object);
        void renderObjectInspectorWaterTilePart(BaseMeshPtr object);
        void renderAddSkyboxDialog();
        void renderAddMeshDialog();
        void renderAddWaterTileDialog();
        void renderFogDialog();
        void renderPostprocessDialog();
        void renderCaptureDialog();
        void renderRenderersDialog();
        void renderLightingDialog();
        void renderShadowMappingDialog();
        void renderSaveSceneDialog();
        void renderLoadSceneDialog();

        WindowPtr target_window_{nullptr};
        PostprocessPtr postprocess_{nullptr};
        MeshRendererPtr mesh_renderer_{nullptr};
        SkyboxRendererPtr skybox_renderer_{nullptr};
        ShadowMapRendererPtr shadow_map_renderer_{nullptr};
        WaterRendererPtr water_renderer_{nullptr};
        GizmoRendererPtr gizmo_renderer_{nullptr};

        SceneLoaderPtr scene_loader_{nullptr};

        ImFont *font_normal_{nullptr};
        ImFont *font_bold_{nullptr};

        ScenePtr current_scene_{nullptr};

        std::function<void()> quit_function_{nullptr};
        std::function<void(std::string, bool)> save_capture_function_{nullptr};

        bool render_about_dialog_{false};
        bool render_object_inspector_dialog_{false};
        bool render_camera_dialog_{false};
        bool render_fog_dialog_{false};
        bool render_postprocess_dialog_{false};
        bool render_capture_dialog_{false};
        bool render_renderers_dialog_{false};
        bool render_lighting_dialog_{false};
        bool render_shadow_map_dialog_{false};
        bool render_save_scene_dialog_{false};
        bool render_load_scene_dialog_{false};

        bool render_add_skybox_dialog_{false};
        bool render_add_mesh_dialog_{false};
        bool render_add_water_tile_dialog_{false};
    };

    using DefaultGuiRendererPtr = std::shared_ptr<DefaultGuiRenderer>;
} // namespace puffin

#endif // PUFFIN_DEFAULT_GUI_RENDERER_HPP