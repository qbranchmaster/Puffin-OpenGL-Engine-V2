/*
 * Puffin OpenGL Engine ver. 2.1
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

#include "PuffinEngine/Camera.hpp"
#include "PuffinEngine/DepthTest.hpp"
#include "PuffinEngine/FontRenderer.hpp"
#include "PuffinEngine/FrameBuffer.hpp"
#include "PuffinEngine/GuiRenderer.hpp"
#include "PuffinEngine/Mesh.hpp"
#include "PuffinEngine/MeshRenderer.hpp"
#include "PuffinEngine/PostprocessRenderer.hpp"
#include "PuffinEngine/RenderSettings.hpp"
#include "PuffinEngine/ShadowMapRenderer.hpp"
#include "PuffinEngine/SkyboxRenderer.hpp"
#include "PuffinEngine/System.hpp"
#include "PuffinEngine/Time.hpp"
#include "PuffinEngine/Timer.hpp"
#include "PuffinEngine/WaterRenderer.hpp"
#include "PuffinEngine/Window.hpp"

namespace puffin {
    class MasterRenderer {
    public:
        MasterRenderer(WindowPtr window, CameraPtr camera, RenderSettingsPtr render_settings);

        void start();
        void stop();

        void assignRenderingFunction(std::function<ScenePtr()> function);

        void assignPostprocessRenderer(PostprocessRendererPtr renderer);
        void assignSkyboxRenderer(SkyboxRendererPtr renderer);
        void assignMeshRenderer(MeshRendererPtr renderer);
        void assignShadowMapRenderer(ShadowMapRendererPtr renderer);
        void assignFontRenderer(FontRendererPtr renderer);
        void assignGuiRenderer(GuiRendererPtr renderer);
        void assignWaterRenderer(WaterRendererPtr renderer);

        void drawScene(ScenePtr scene);
        void captureScreen(std::string file_name, GLboolean add_timestamp);

        void addTimer(TimerPtr timer);

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

    private:
        void createDefaultFrameBuffer();
        void clearDefaultFrameBuffer();

        void checkGlErrors();
        void saveFrameToFile();

        GLboolean rendering_enabled_{false};
        std::function<ScenePtr()> rendering_function_{nullptr};

        CameraPtr camera_{nullptr};
        FrameBufferPtr default_frame_buffer_{nullptr};
        FrameBufferPtr default_frame_buffer_multisample_{nullptr};
        RenderSettingsPtr render_settings_{nullptr};
        WindowPtr target_window_{nullptr};

        std::vector<TimerPtr> timers_;

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
    };

    using MasterRendererPtr = std::shared_ptr<MasterRenderer>;
} // namespace puffin

#endif // PUFFIN_MASTER_RENDERER_HPP