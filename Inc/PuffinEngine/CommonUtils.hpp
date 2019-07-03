/*
 * Puffin OpenGL Engine ver. 2.0.1
 * Coded by: Sebastian 'qbranchmaster' Tabaka
 * Contact: sebastian.tabaka@outlook.com
 */

#ifndef PUFFIN_COMMON_UTILS_HPP
#define PUFFIN_COMMON_UTILS_HPP

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

namespace puffin {
    std::ostream &operator<<(std::ostream &os, const glm::vec3 &vec);
    std::istream &operator>>(std::istream &is, glm::vec3 &vec);

    std::ostream &operator<<(std::ostream &os, const glm::mat4 &mat);
    std::istream &operator>>(std::istream &is, glm::mat4 &mat);

    std::ostream &operator<<(std::ostream &os, const glm::mat3 &mat);
    std::istream &operator>>(std::istream &is, glm::mat3 &mat);

    std::ostream &operator<<(std::ostream &os, GLboolean val);
    std::istream &operator>>(std::istream &is, GLboolean &val);
} // namespace puffin

#endif // PUFFIN_COMMON_UTILS_HPP