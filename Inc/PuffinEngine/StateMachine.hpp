/*
* Puffin OpenGL Engine ver. 2.0
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
        friend class ShaderProgram;

    public:
        static StateMachine &instance() {
            static StateMachine singleton;
            return singleton;
        }

    private:
        StateMachine() {}
        StateMachine(const StateMachine &) = delete;
        void operator=(const StateMachine &) = delete;

        GLuint bound_shader_program_{0};
    };
} // namespace puffin

#endif // PUFFIN_STATE_MACHINE_HPP