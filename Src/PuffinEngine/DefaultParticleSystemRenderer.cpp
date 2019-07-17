/*
 * Puffin OpenGL Engine ver. 2.0.1
 * Coded by: Sebastian 'qbranchmaster' Tabaka
 * Contact: sebastian.tabaka@outlook.com
 */

#include "PuffinEngine/DefaultParticleSystemRenderer.hpp"

using namespace puffin;

DefaultParticleSystemRenderer::DefaultParticleSystemRenderer() {
    loadShaders();
}

void DefaultParticleSystemRenderer::render(FrameBufferPtr frame_buffer, ScenePtr scene) {

}

void DefaultParticleSystemRenderer::loadShaders() {
    default_shader_program_.reset(new ShaderProgram("particle_system_shader_program"));
    default_shader_program_->loadShaders(
        "Data/Shaders/Particle.vert", "Data/Shaders/Particle.frag");
}