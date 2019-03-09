/*
* Puffin OpenGL Engine ver. 2.0
* Coded by: Sebastian 'qbranchmaster' Tabaka
*/

#ifndef PUFFIN_SHADOW_MAP_RENDERER_HPP
#define PUFFIN_SHADOW_MAP_RENDERER_HPP

#include "PuffinEngine/Mesh.hpp"
#include "PuffinEngine/Renderer.hpp"

namespace puffin {
    class ShadowMapRenderer : public Renderer {
    public:
        virtual void render(MeshPtr mesh) = 0;
    };

    using ShadowMapRendererPtr = std::shared_ptr<ShadowMapRenderer>;
} // namespace puffin

#endif // PUFFIN_SHADOW_MAP_RENDERER_HPP