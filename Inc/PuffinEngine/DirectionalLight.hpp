/*
 * Puffin OpenGL Engine ver. 2.0.1
 * Coded by: Sebastian 'qbranchmaster' Tabaka
 * Contact: sebastian.tabaka@outlook.com
 */

#ifndef PUFFIN_DIRECTIONAL_LIGHT_HPP
#define PUFFIN_DIRECTIONAL_LIGHT_HPP

#ifdef WIN32 // Prevents APIENTRY redefinition
#include <Windows.h>
#endif // WIN32
#include <GL/glew.h>

#include <glm/glm.hpp>

#include <memory>

#include "PuffinEngine/Logger.hpp"

namespace puffin {
    class DirectionalLight {
        friend class SceneLoader;

    public:
        void enable(GLboolean state) {
            enabled_ = state;
        }

        GLboolean isEnabled() const {
            return enabled_;
        }

        void setAmbientColor(const glm::vec3 &color) {
            ambient_color_ = color;
        }

        void setDiffuseColor(const glm::vec3 &color) {
            diffuse_color_ = color;
        }

        void setSpecularColor(const glm::vec3 &color) {
            specular_color_ = color;
        }

        glm::vec3 getAmbientColor() const {
            return ambient_color_;
        }

        glm::vec3 getDiffuseColor() const {
            return diffuse_color_;
        }

        glm::vec3 getSpecularColor() const {
            return specular_color_;
        }

        void setDirection(const glm::vec3 &direction) {
            if (direction.x == 0.0f && direction.y == 0.0f && direction.z == 0.0f) {
                logError("DirectionalLight::setDirection()", PUFFIN_MSG_INVALID_VALUE);
                return;
            }

            direction_ = glm::normalize(direction);
        }

        glm::vec3 getDirection() const {
            return direction_;
        }

        void reset() {
            enabled_ = false;

            ambient_color_ = glm::vec3(0.2f, 0.2f, 0.2f);
            diffuse_color_ = glm::vec3(0.6f, 0.6f, 0.6f);
            specular_color_ = glm::vec3(1.0f, 1.0f, 1.0f);

            direction_ = glm::vec3(-1.0f, -1.0f, -1.0f);
        }

    private:
        GLboolean enabled_{false};

        glm::vec3 ambient_color_{0.2f, 0.2f, 0.2f};
        glm::vec3 diffuse_color_{0.6f, 0.6f, 0.6f};
        glm::vec3 specular_color_{1.0f, 1.0f, 1.0f};

        glm::vec3 direction_{-1.0f, -1.0f, -1.0f};
    };

    using DirectionalLightPtr = std::shared_ptr<DirectionalLight>;
} // namespace puffin

#endif // PUFFIN_DIRECTIONAL_LIGHT_HPP