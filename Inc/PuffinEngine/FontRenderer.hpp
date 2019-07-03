/*
 * Puffin OpenGL Engine ver. 2.0.1
 * Coded by: Sebastian 'qbranchmaster' Tabaka
 * Contact: sebastian.tabaka@outlook.com
 */

#ifndef PUFFIN_FONT_RENDERER_HPP
#define PUFFIN_FONT_RENDERER_HPP

#include "PuffinEngine/Renderer.hpp"
#include "PuffinEngine/Text.hpp"

namespace puffin {
    class FontRenderer : public Renderer {
    public:
        virtual void render(TextPtr text) = 0;
    };

    using FontRendererPtr = std::shared_ptr<FontRenderer>;
} // namespace puffin

#endif // PUFFIN_FONT_RENDERER_HPP