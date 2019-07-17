/*
 * Puffin OpenGL Engine ver. 2.0.1
 * Coded by: Sebastian 'qbranchmaster' Tabaka
 * Contact: sebastian.tabaka@outlook.com
 */

#ifndef PUFFIN_DEFAULT_PARTICLE_SYSTEM_RENDERER_HPP
#define PUFFIN_DEFAULT_PARTICLE_SYSTEM_RENDERER_HPP

#include "PuffinEngine/ParticleSystemRenderer.hpp"
#include "PuffinEngine/ShaderProgram.hpp"

namespace puffin {
    class DefaultParticleSystemRenderer : public ParticleSystemRenderer {
    public:
        DefaultParticleSystemRenderer();

        void render(FrameBufferPtr frame_buffer, ScenePtr scene);

    private:
        void loadShaders();

        ShaderProgramPtr default_shader_program_{nullptr};
    };
} // namespace puffin

#endif // PUFFIN_DEFAULT_PARTICLE_SYSTEM_RENDERER_HPP