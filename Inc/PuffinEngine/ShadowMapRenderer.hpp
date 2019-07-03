/*
 * Puffin OpenGL Engine ver. 2.0.1
 * Coded by: Sebastian 'qbranchmaster' Tabaka
 * Contact: sebastian.tabaka@outlook.com
 */

#ifndef PUFFIN_SHADOW_MAP_RENDERER_HPP
#define PUFFIN_SHADOW_MAP_RENDERER_HPP

#include "PuffinEngine/Renderer.hpp"
#include "PuffinEngine/Scene.hpp"

namespace puffin {
    class ShadowMapRenderer : public Renderer {
    public:
        virtual void render(ScenePtr scene) = 0;
    };

    using ShadowMapRendererPtr = std::shared_ptr<ShadowMapRenderer>;
} // namespace puffin

#endif // PUFFIN_SHADOW_MAP_RENDERER_HPP