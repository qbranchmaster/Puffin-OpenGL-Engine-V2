/*
* Puffin OpenGL Engine ver. 2.0
* Coded by: Sebastian 'qbranchmaster' Tabaka
*/

#ifndef PUFFIN_RENDERERS_SHARED_DATA_HPP
#define PUFFIN_RENDERERS_SHARED_DATA_HPP

#ifdef WIN32 // Prevents APIENTRY redefinition
#include <Windows.h>
#endif // WIN32
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <memory>

#include "PuffinEngine/DepthTextureBuffer.hpp"

namespace puffin {
    struct RenderersSharedData {
        glm::mat4 dir_light_space_matrix;
        DepthTextureBufferPtr shadow_map_texture;
    };

    using RenderersSharedDataPtr = std::shared_ptr<RenderersSharedData>;
} // namespace puffin

#endif // PUFFIN_RENDERERS_SHARED_DATA_HPP