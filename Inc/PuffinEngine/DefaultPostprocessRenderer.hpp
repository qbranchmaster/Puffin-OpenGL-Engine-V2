/*
 * Puffin OpenGL Engine ver. 2.1
 * Coded by: Sebastian 'qbranchmaster' Tabaka
 * Contact: sebastian.tabaka@outlook.com
 */

#ifndef PUFFIN_DEFAULT_POSTPROCESS_RENDERER_HPP
#define PUFFIN_DEFAULT_POSTPROCESS_RENDERER_HPP

#include "PuffinEngine/Camera.hpp"
#include "PuffinEngine/DepthTest.hpp"
#include "PuffinEngine/InitConfig.hpp"
#include "PuffinEngine/Mesh.hpp"
#include "PuffinEngine/PostprocessRenderer.hpp"
#include "PuffinEngine/RenderSettings.hpp"
#include "PuffinEngine/ShaderProgram.hpp"

namespace puffin {
    class DefaultPostprocessRenderer : public PostprocessRenderer {
    public:
        DefaultPostprocessRenderer(RenderSettingsPtr render_settings, CameraPtr camera);

        void render(FrameBufferPtr frame_buffer);

    private:
        void loadShaders();
        void setShadersUniforms();
        void createScreenMesh();
        void drawMesh(MeshPtr mesh);
        void renderGlowBloom(FrameBufferPtr frame_buffer);

        CameraPtr camera_{nullptr};
        FrameBufferPtr bloom_frame_buffer_[2]{nullptr};
        MeshPtr screen_mesh_{nullptr};
        RenderSettingsPtr render_settings_{nullptr};
        ShaderProgramPtr default_shader_program_{nullptr};
        ShaderProgramPtr bloom_shader_program_{nullptr};
    };

    using DefaultPostprocessRendererPtr = std::shared_ptr<DefaultPostprocessRenderer>;
} // namespace puffin

#endif // PUFFIN_DEFAULT_POSTPROCESS_RENDERER_HPP