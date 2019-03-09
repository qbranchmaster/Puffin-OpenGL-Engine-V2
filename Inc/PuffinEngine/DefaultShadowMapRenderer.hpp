/*
* Puffin OpenGL Engine ver. 2.0
* Coded by: Sebastian 'qbranchmaster' Tabaka
*/

#ifndef PUFFIN_DEFAULT_SHADOW_MAP_RENDERER_HPP
#define PUFFIN_DEFAULT_SHADOW_MAP_RENDERER_HPP

#include "PuffinEngine/AlphaBlend.hpp"
#include "PuffinEngine/Camera.hpp"
#include "PuffinEngine/CameraFrustum.hpp"
#include "PuffinEngine/DepthTest.hpp"
#include "PuffinEngine/FaceCull.hpp"
#include "PuffinEngine/FrameBuffer.hpp"
#include "PuffinEngine/RenderSettings.hpp"
#include "PuffinEngine/ShaderProgram.hpp"
#include "PuffinEngine/ShadowMapRenderer.hpp"

namespace puffin {
    class DefaultShadowMapRenderer : public ShadowMapRenderer {
    public:
        DefaultShadowMapRenderer(RenderSettingsPtr render_settings,
            CameraPtr camera);

        void render(MeshPtr mesh);

    private:
        void loadShaders();
        void createDirectionalLightFrameBuffer();
        void renderDirectionalLightShadowMap(MeshPtr mesh);
        glm::mat4 calculateDirectionalLightSpaceMatrix();

        CameraPtr camera_;
        CameraFrustumPtr camera_frustum_;
        RenderSettingsPtr render_settings_;
        ShaderProgramPtr directional_light_shader_program_;

        FrameBufferPtr directional_light_shadow_map_frame_bufer_;
    };

    using DefaultShadowMapRendererPtr =
        std::shared_ptr<DefaultShadowMapRenderer>;
} // namespace puffin

#endif // PUFFIN_DEFAULT_SHADOW_MAP_RENDERER_HPP