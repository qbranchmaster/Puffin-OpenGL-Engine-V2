/*
 * Puffin OpenGL Engine ver. 2.0.1
 * Coded by: Sebastian 'qbranchmaster' Tabaka
 * Contact: sebastian.tabaka@outlook.com
 */

#ifndef PUFFIN_MASTER_RENDERER_HPP
#define PUFFIN_MASTER_RENDERER_HPP

#ifdef WIN32 // Prevents APIENTRY redefinition
#include <Windows.h>
#endif // WIN32
#include <GL/glew.h>

#include <functional>
#include <memory>

#include "PuffinEngine/DefaultFontRenderer.hpp"
#include "PuffinEngine/DefaultGizmoRenderer.hpp"
#include "PuffinEngine/DefaultGuiRenderer.hpp"
#include "PuffinEngine/DefaultMeshRenderer.hpp"
#include "PuffinEngine/DefaultPostprocessRenderer.hpp"
#include "PuffinEngine/DefaultShadowMapRenderer.hpp"
#include "PuffinEngine/DefaultSkyboxRenderer.hpp"
#include "PuffinEngine/DefaultWaterRenderer.hpp"
#include "PuffinEngine/Postprocess.hpp"
#include "PuffinEngine/System.hpp"
#include "PuffinEngine/Time.hpp"
#include "PuffinEngine/Timer.hpp"
#include "PuffinEngine/Window.hpp"

namespace puffin {
    class MasterRenderer {
    public:
        explicit MasterRenderer(WindowPtr window);

        void assignRenderingFunction(std::function<ScenePtr()> function);

        void start();
        void stop();
        void drawScene(ScenePtr scene);

        void captureScreen(std::string file_name, bool add_timestamp);

        GuiRendererPtr guiRenderer() const {
            return gui_renderer_;
        }

        MeshRendererPtr meshRenderer() const {
            return mesh_renderer_;
        }

        PostprocessRendererPtr postprocessRenderer() const {
            return postprocess_renderer_;
        }

        SkyboxRendererPtr skyboxRenderer() const {
            return skybox_renderer_;
        }

        ShadowMapRendererPtr shadowMapRenderer() const {
            return shadow_map_renderer_;
        }

        FontRendererPtr fontRenderer() const {
            return font_renderer_;
        }

        WaterRendererPtr waterRenderer() const {
            return water_renderer_;
        }

        GizmoRendererPtr gizmoRenderer() const {
            return gizmo_renderer_;
        }

        PostprocessPtr postprocess() const {
            return postprocess_;
        }

        void addTimer(TimerPtr timer);

    private:
        void createRenderers();
        void createDefaultFrameBuffer();
        void clearDefaultFrameBuffer();

        void checkGlErrors();
        void saveFrameToFile();

        GLboolean rendering_enabled_{false};
        std::function<ScenePtr()> rendering_function_{nullptr};

        FrameBufferPtr default_frame_buffer_{nullptr};
        FrameBufferPtr default_frame_buffer_multisample_{nullptr};

        WindowPtr target_window_{nullptr};
        PostprocessPtr postprocess_{nullptr};

        GLboolean capture_screen_flag_{false};
        GLboolean capture_add_timestamp_{false};
        std::string capture_file_name_;

        GuiRendererPtr gui_renderer_{nullptr};
        MeshRendererPtr mesh_renderer_{nullptr};
        PostprocessRendererPtr postprocess_renderer_{nullptr};
        SkyboxRendererPtr skybox_renderer_{nullptr};
        ShadowMapRendererPtr shadow_map_renderer_{nullptr};
        FontRendererPtr font_renderer_{nullptr};
        WaterRendererPtr water_renderer_{nullptr};
        GizmoRendererPtr gizmo_renderer_{nullptr};

        std::vector<TimerPtr> timers_;
    };

    using MasterRendererPtr = std::shared_ptr<MasterRenderer>;
} // namespace puffin

#endif // PUFFIN_MASTER_RENDERER_HPP