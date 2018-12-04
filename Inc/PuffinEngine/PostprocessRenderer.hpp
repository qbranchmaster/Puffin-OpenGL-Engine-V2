/*
* Puffin OpenGL Engine ver. 2.0
* Coded by: Sebastian 'qbranchmaster' Tabaka
*/

#ifndef PUFFIN_POSTPROCESS_RENDERER_HPP
#define PUFFIN_POSTPROCESS_RENDERER_HPP

#include "PuffinEngine/Renderer.hpp"

namespace puffin {
    class PostprocessRenderer : public Renderer {
    public:
        virtual void render() = 0;
    };

    using PostprocessRendererPtr = std::shared_ptr<PostprocessRenderer>;
} // namespace puffin

#endif // PUFFIN_POSTPROCESS_RENDERER_HPP