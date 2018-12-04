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

namespace puffin {
    class Renderer {

    };

    using RendererPtr = std::shared_ptr<Renderer>;
} // namespace puffin

#endif // PUFFIN_RENDERER_HPP