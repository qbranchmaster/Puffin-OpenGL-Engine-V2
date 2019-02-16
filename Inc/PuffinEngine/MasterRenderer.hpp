/*
* Puffin OpenGL Engine ver. 2.0
* Coded by: Sebastian 'qbranchmaster' Tabaka
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
#include "PuffinEngine/ConfigGuiRenderer.hpp"
#include "PuffinEngine/Configuration.hpp"
#include "PuffinEngine/DepthTest.hpp"
#include "PuffinEngine/GlUtils.hpp"
#include "PuffinEngine/FrameBuffer.hpp"
#include "PuffinEngine/Mesh.hpp"
#include "PuffinEngine/MeshRenderer.hpp"
#include "PuffinEngine/PostprocessRenderer.hpp"
#include "PuffinEngine/RenderSettings.hpp"
#include "PuffinEngine/Scene.hpp"
#include "PuffinEngine/ShaderProgram.hpp"
#include "PuffinEngine/SkyboxRenderer.hpp"
#include "PuffinEngine/System.hpp"
#include "PuffinEngine/Time.hpp"
#include "PuffinEngine/Window.hpp"

namespace puffin {
    class MasterRenderer {
    public:
        MasterRenderer(WindowPtr window, CameraPtr camera,
            RenderSettingsPtr render_settings);

        void start();
        void stop();

        void assignRenderingFunction(std::function<void()> function);
        void assignPostprocessRenderer(PostprocessRendererPtr renderer);
        void assignSkyboxRenderer(SkyboxRendererPtr renderer);
        void assignMeshRenderer(MeshRendererPtr renderer);

        void drawScene(ScenePtr scene);

        void configGuiEnabled(GLboolean state);

    private:
        void createDefaultFrameBuffer();
        void clearDefaultFrameBuffer();
        void checkGlErrors();

        GLboolean rendering_enabled_{false};
        GLboolean config_gui_enabled_{true};
        std::function<void()> rendering_function_{nullptr};

        CameraPtr camera_;
        FrameBufferPtr default_frame_buffer_;
        FrameBufferPtr default_frame_buffer_multisample_;
        RenderSettingsPtr render_settings_;
        WindowPtr target_window_;

        ConfigGuiRendererPtr config_gui_renderer_;
        MeshRendererPtr mesh_renderer_;
        PostprocessRendererPtr postprocess_renderer_;
        SkyboxRendererPtr skybox_renderer_;
    };

    using MasterRendererPtr = std::shared_ptr<MasterRenderer>;
} // namespace puffin

#endif // PUFFIN_MASTER_RENDERER_HPP