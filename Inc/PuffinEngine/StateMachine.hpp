/*
* Puffin OpenGL Engine ver. 2.0
* Coded by: Sebastian 'qbranchmaster' Tabaka
*/

#ifndef PUFFIN_STATE_MACHINE_HPP
#define PUFFIN_STATE_MACHINE_HPP

#include <GL/glew.h>

#include <unordered_map>

namespace puffin {
    class StateMachine {
        friend class FrameBuffer;
        friend class Mesh;
        friend class ShaderProgram;
        friend class Texture;

    public:
        static StateMachine& instance() {
            static StateMachine singleton;
            return singleton;
        }

    private:
        StateMachine() {}
        StateMachine(const StateMachine &) = delete;
        void operator=(const StateMachine &) = delete;

        GLuint bound_mesh_{0};
        GLuint bound_shader_program_{0};
        GLuint bound_frame_buffer_{0};
        GLuint bound_frame_buffer_write_{0};
        GLuint bound_frame_buffer_read_{0};

        std::unordered_map<GLushort, GLuint> bound_texture_;
    };
} // namespace puffin

#endif // PUFFIN_STATE_MACHINE_HPP