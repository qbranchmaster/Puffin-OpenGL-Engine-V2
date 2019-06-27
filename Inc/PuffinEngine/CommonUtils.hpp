/*
 * Puffin OpenGL Engine ver. 2.1
 * Coded by: Sebastian 'qbranchmaster' Tabaka
 * Contact: sebastian.tabaka@outlook.com
 */

#ifndef PUFFIN_COMMON_UTILS_HPP
#define PUFFIN_COMMON_UTILS_HPP

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <ios>

#include "PuffinEngine/Camera.hpp"
#include "PuffinEngine/Skybox.hpp"

namespace puffin {
    std::ostream &operator<<(std::ostream &os, CameraPtr camera);
    std::istream &operator>>(std::istream &is, CameraPtr camera);

	std::ostream &operator<<(std::ostream &os, SkyboxPtr skybox);
    std::istream &operator>>(std::istream &is, SkyboxPtr skybox);

    std::ostream &operator<<(std::ostream &os, glm::vec3 &vec);
    std::istream &operator>>(std::istream &is, glm::vec3 &vec);

    std::ostream &operator<<(std::ostream &os, glm::mat4 &mat);
    std::istream &operator>>(std::istream &is, glm::mat4 &mat);
} // namespace puffin

#endif // PUFFIN_COMMON_UTILS_HPP