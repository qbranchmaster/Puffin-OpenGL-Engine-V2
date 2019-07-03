/*
 * Puffin OpenGL Engine ver. 2.0.1
 * Coded by: Sebastian 'qbranchmaster' Tabaka
 * Contact: sebastian.tabaka@outlook.com
 */

#ifndef PUFFIN_RENDERER_HPP
#define PUFFIN_RENDERER_HPP

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