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

namespace puffin {
    class Lighting {
    public:
        void enable(GLboolean state) {
            enabled_ = state;
        }

        GLboolean isEnabled() const {
            return enabled_;
        }

        void enableAmbientLight(GLboolean state) {
            ambient_enabled_ = state;
        }

        GLboolean isAmbientEnabled() const {
            return ambient_enabled_;
        }

        void setAmbientColor(const glm::vec3 &color) {
            ambient_color_ = glm::vec3(
                glm::clamp(color.r, 0.0f, 1.0f),
                glm::clamp(color.g, 0.0f, 1.0f),
                glm::clamp(color.b, 0.0f, 1.0f)
            );
        }

        glm::vec3 getAmbientColor() const {
            return ambient_color_;
        }

    private:
        GLboolean enabled_{false};

        GLboolean ambient_enabled_{false};
        glm::vec3 ambient_color_{1.0f, 1.0f, 1.0f};
    };

    using LightingPtr = std::shared_ptr<Lighting>;
} // namespace puffin

#endif // PUFFIN_LIGHTING_HPP