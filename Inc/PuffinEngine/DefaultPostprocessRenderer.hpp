/*
* Puffin OpenGL Engine ver. 2.0
* Coded by: Sebastian 'qbranchmaster' Tabaka
*/

#ifndef PUFFIN_DEFAULT_POSTPROCESS_RENDERER_HPP
#define PUFFIN_DEFAULT_POSTPROCESS_RENDERER_HPP

#include "PuffinEngine/GlUtils.hpp"
#include "PuffinEngine/Mesh.hpp"
#include "PuffinEngine/PostprocessRenderer.hpp"
#include "PuffinEngine/RenderSettings.hpp"
#include "PuffinEngine/ShaderProgram.hpp"

namespace puffin {
    class DefaultPostprocessRenderer : public PostprocessRenderer {
    public:
        DefaultPostprocessRenderer(RenderSettingsPtr render_settings);

        void render(FrameBufferPtr frame_buffer);

    private:
        void loadShaders();
        void setShadersUniforms();
        void createScreenMesh();
        void drawMesh(MeshPtr mesh);

        FrameBufferPtr target_frame_buffer_;
        MeshPtr screen_mesh_;
        RenderSettingsPtr render_settings_;
        ShaderProgramPtr default_shader_program_;
    };

    using DefaultPostprocessRendererPtr =
        std::shared_ptr<DefaultPostprocessRenderer>;
} // namespace puffin

#endif // PUFFIN_DEFAULT_POSTPROCESS_RENDERER_HPP