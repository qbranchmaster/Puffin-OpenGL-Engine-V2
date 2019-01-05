/*
* Puffin OpenGL Engine ver. 2.0
* Coded by: Sebastian 'qbranchmaster' Tabaka
*/

#ifndef PUFFIN_LIGHTING_HPP
#define PUFFIN_LIGHTING_HPP

#ifdef WIN32 // Prevents APIENTRY redefinition
#include <Windows.h>
#endif // WIN32
#include <GL/glew.h>

#include <glm/glm.hpp>

#include <memory>

#include "PuffinEngine/DirectionalLight.hpp"

namespace puffin {
    class Lighting {
    public:
        Lighting() {
            directional_light_.reset(new DirectionalLight());
        }

        void enable(GLboolean state) {
            enabled_ = state;
        }

        GLboolean isEnabled() const {
            return enabled_;
        }

        void setSkyboxLightingColor(const glm::vec3 &color) {
            skybox_light_color_ = glm::vec3(
                glm::clamp(color.r, 0.0f, 1.0f),
                glm::clamp(color.g, 0.0f, 1.0f),
                glm::clamp(color.b, 0.0f, 1.0f)
            );
        }

        glm::vec3 getSkyboxLightColor() const {
            return skybox_light_color_;
        }

        DirectionalLightPtr directionalLight() const {
            return directional_light_;
        }

    private:
        GLboolean enabled_{false};

        glm::vec3 skybox_light_color_{1.0f, 1.0f, 1.0f};

        DirectionalLightPtr directional_light_;
    };

    using LightingPtr = std::shared_ptr<Lighting>;
} // namespace puffin

#endif // PUFFIN_LIGHTING_HPP