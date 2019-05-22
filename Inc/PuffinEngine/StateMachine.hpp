/*
 * Puffin OpenGL Engine ver. 2.1
 * Coded by: Sebastian 'qbranchmaster' Tabaka
 * Contact: sebastian.tabaka@outlook.com
 */

#ifndef PUFFIN_STATE_MACHINE_HPP
#define PUFFIN_STATE_MACHINE_HPP

#ifdef WIN32 // Prevents APIENTRY redefinition
#include <Windows.h>
#endif // WIN32
#include <GL/glew.h>

namespace puffin {
    class StateMachine {
        friend class BaseMesh;
        friend class DepthTextureBuffer;
        friend class FrameBuffer;
        friend class RenderBuffer;
        friend class ShaderProgram;
        friend class Texture;
        friend class TextureBuffer;

    public:
        static StateMachine &instance() {
            static StateMachine singleton;
            return singleton;
        }

        GLuint getBoundFrameBuffer() const {
            return bound_frame_buffer_;
        }

        GLuint getBoundMeshHandle() const {
            return bound_mesh_;
        }

        GLuint getBoundRenderBuffer() const {
            return bound_render_buffer_;
        }

        GLuint getActiveShaderProgramHandle() const {
            return bound_shader_program_;
        }

        GLuint getBoundTexture() const {
            return bound_texture_;
        }

    private:
        StateMachine() {}
        StateMachine(const StateMachine &) = delete;
        void operator=(const StateMachine &) = delete;

        GLuint bound_frame_buffer_ = 0;
        GLuint bound_frame_buffer_read_ = 0;
        GLuint bound_frame_buffer_write_ = 0;
        GLuint bound_mesh_{0};
        GLuint bound_shader_program_{0};
        GLuint bound_texture_{0};
        GLuint bound_render_buffer_{0};
    };
} // namespace puffin

#endif // PUFFIN_STATE_MACHINE_HPP