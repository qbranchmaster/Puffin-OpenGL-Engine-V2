/*
 * Puffin OpenGL Engine ver. 2.0.1
 * Coded by: Sebastian 'qbranchmaster' Tabaka
 * Contact: sebastian.tabaka@outlook.com
 */

#ifndef PUFFIN_DEFAULT_SHADOW_MAP_RENDERER_HPP
#define PUFFIN_DEFAULT_SHADOW_MAP_RENDERER_HPP

#include "PuffinEngine/AlphaBlend.hpp"
#include "PuffinEngine/Camera.hpp"
#include "PuffinEngine/CameraFrustum.hpp"
#include "PuffinEngine/DepthTest.hpp"
#include "PuffinEngine/FaceCull.hpp"
#include "PuffinEngine/FrameBuffer.hpp"
#include "PuffinEngine/InitConfig.hpp"
#include "PuffinEngine/ShaderProgram.hpp"
#include "PuffinEngine/ShadowMapRenderer.hpp"

namespace puffin {
    struct ShadowMapRendererOutputData {
        glm::mat4 dir_light_space_matrix;
        DepthTextureBufferPtr shadow_map_texture;
        std::array<CubeTextureBufferPtr, InitConfig::getMaxPointLightsCount()>
            point_light_texture_buffer;
    };

    class DefaultShadowMapRenderer : public ShadowMapRenderer {
    public:
        DefaultShadowMapRenderer();

        void render(ScenePtr scene);

        ShadowMapRendererOutputData getOutputData() const {
            return output_data_;
        }

    private:
        void loadShaders();
        void createDirectionalLightFrameBuffer();
        void renderDirectionalLightShadowMap(ScenePtr scene);
        glm::mat4 calculateDirectionalLightSpaceMatrix(LightingPtr lighting, CameraPtr camera);

        void createPointLightFrameBuffer(GLushort light_index);
        void renderPointLightShadowMap(ScenePtr scene);

        CameraFrustumPtr camera_frustum_{nullptr};
        ShaderProgramPtr directional_light_shader_program_{nullptr};
        ShaderProgramPtr point_light_shader_program_{nullptr};

        ShadowMapRendererOutputData output_data_{};

        FrameBufferPtr directional_light_shadow_map_frame_bufer_{nullptr};
        std::array<FrameBufferPtr, InitConfig::getMaxPointLightsCount()>
            point_light_shadow_map_frame_buffer_;
    };

    using DefaultShadowMapRendererPtr = std::shared_ptr<DefaultShadowMapRenderer>;
} // namespace puffin

#endif // PUFFIN_DEFAULT_SHADOW_MAP_RENDERER_HPP