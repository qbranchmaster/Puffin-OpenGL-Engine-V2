/*
 * Puffin OpenGL Engine ver. 2.1
 * Coded by: Sebastian 'qbranchmaster' Tabaka
 * Contact: sebastian.tabaka@outlook.com
 */

#ifndef PUFFIN_RENDERER_HPP
#define PUFFIN_RENDERER_HPP

#ifdef WIN32 // Prevents APIENTRY redefinition
#include <Windows.h>
#endif // WIN32
#include <GL/glew.h>

#include <memory>

namespace puffin {
    class Renderer {
    public:
        void enable(GLboolean state) {
            enabled_ = state;
        }

        GLboolean isEnabled() const {
            return enabled_;
        }

    protected:
        GLboolean enabled_{true};
    };

    using RendererPtr = std::shared_ptr<Renderer>;
} // namespace puffin

#endif // PUFFIN_RENDERER_HPP