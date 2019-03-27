/*
* Puffin OpenGL Engine ver. 2.0
* Coded by: Sebastian 'qbranchmaster' Tabaka
*/

#ifndef PUFFIN_RENDERER_HPP
#define PUFFIN_RENDERER_HPP

#ifdef WIN32 // Prevents APIENTRY redefinition
#include <Windows.h>
#endif // WIN32
#include <GL/glew.h>

#include <memory>

#include "PuffinEngine/Logger.hpp"
#include "PuffinEngine/RenderersSharedData.hpp"

namespace puffin {
    class Renderer {
    public:
        void setSharedDataPtr(RenderersSharedDataPtr data_ptr) {
            if (!data_ptr) {
                logError("Renderer::setSharedDataPtr()", "Null input.");
                return;
            }

            renderers_shared_data_ = data_ptr;
        }

    protected:
        RenderersSharedDataPtr renderers_shared_data_;
    };

    using RendererPtr = std::shared_ptr<Renderer>;
} // namespace puffin

#endif // PUFFIN_RENDERER_HPP