/*
* Puffin OpenGL Engine ver. 2.0
* Coded by: Sebastian 'qbranchmaster' Tabaka
*/

#ifndef PUFFIN_GL_UTILS_HPP
#define PUFFIN_GL_UTILS_HPP

#ifdef WIN32 // Prevents APIENTRY redefinition
#include <Windows.h>
#endif // WIN32
#include <GL/glew.h>

#include <glm/glm.hpp>

namespace puffin {
    void setClearColor(const glm::vec3 &color);
    void clearFrameBuffer(GLboolean depth_clear = true);
} // namespace puffin

#endif // PUFFIN_GL_UTILS_HPP