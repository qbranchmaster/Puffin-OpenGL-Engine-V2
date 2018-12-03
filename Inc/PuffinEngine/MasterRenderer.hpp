/*
* Puffin OpenGL Engine ver. 2.0
* Created by: Sebastian 'qbranchmaster' Tabaka
*/

#ifndef PUFFIN_MASTER_RENDERER_HPP
#define PUFFIN_MASTER_RENDERER_HPP

#include <GL/glew.h>

#include <functional>
#include <memory>

#include "PuffinEngine/Camera.hpp"
#include "PuffinEngine/FrameBuffer.hpp"
#include "PuffinEngine/Mesh.hpp"
#include "PuffinEngine/RenderSettings.hpp"
#include "PuffinEngine/Scene.hpp"
#include "PuffinEngine/ShaderProgram.hpp"
#include "PuffinEngine/SkyboxRenderer.hpp"
#include "PuffinEngine/Window.hpp"

namespace puffin {
    class MasterRenderer {
    public:
        MasterRenderer(WindowPtr window, CameraPtr camera,
            RenderSettingsPtr render_settings);

        void start();
        void stop();
        void assignRenderingFunction(std::function<void()> function);

        void drawScene(ScenePtr scene);

    private:
        void loadShaders();
        void createFrameBuffer();
        void setShadersUniforms();

        GLboolean rendering_enabled_{false};
        std::function<void()> rendering_function_{nullptr};

        CameraPtr camera_;
        FrameBufferPtr default_frame_buffer_;
        MeshPtr screen_mesh_;
        RenderSettingsPtr render_settings_;
        ShaderProgramPtr default_shader_program_;
        SkyboxRendererPtr skybox_renderer_;
        WindowPtr target_window_;
    };

    using MasterRendererPtr = std::shared_ptr<MasterRenderer>;
} // namespace puffin

#endif // PUFFIN_MASTER_RENDERER_HPP