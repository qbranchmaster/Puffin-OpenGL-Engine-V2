/*
* Puffin OpenGL Engine ver. 2.0
* Coded by: Sebastian 'qbranchmaster' Tabaka
*/

#ifndef PUFFIN_DEFAULT_POSTPROCESS_RENDERER_HPP
#define PUFFIN_DEFAULT_POSTPROCESS_RENDERER_HPP

#include "PuffinEngine/PostprocessRenderer.hpp"

namespace puffin {
    class DefaultPostprocessRenderer : public PostprocessRenderer {
    public:
        void render() {

        }

    private:
    };

    using DefaultPostprocessRendererPtr =
        std::shared_ptr<DefaultPostprocessRenderer>;
} // namespace puffin

#endif // PUFFIN_DEFAULT_POSTPROCESS_RENDERER_HPP