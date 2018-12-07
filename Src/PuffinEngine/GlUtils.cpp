/*
* Puffin OpenGL Engine ver. 2.0
* Coded by: Sebastian 'qbranchmaster' Tabaka
*/

#include "PuffinEngine/GlUtils.hpp"

using namespace puffin;

void puffin::setClearColor(const glm::vec3 & color) {
    glClearColor(color.r, color.g, color.b, 1.0f);
}

void puffin::clearBuffer(GLboolean depth_clear) {
    glClear(GL_COLOR_BUFFER_BIT | (depth_clear ? GL_DEPTH_BUFFER_BIT : 0));
}