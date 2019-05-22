/*
 * Puffin OpenGL Engine ver. 2.1
 * Coded by: Sebastian 'qbranchmaster' Tabaka
 * Contact: sebastian.tabaka@outlook.com
 */

#ifndef PUFFIN_GUI_RENDERER_HPP
#define PUFFIN_GUI_RENDERER_HPP

#include "PuffinEngine/Renderer.hpp"

namespace puffin {
    class GuiRenderer : public Renderer {
    public:
        virtual void render() = 0;
    };

    using GuiRendererPtr = std::shared_ptr<GuiRenderer>;
} // namespace puffin

#endif // PUFFIN_GUI_RENDERER_HPP