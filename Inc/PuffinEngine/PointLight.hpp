/*
 * Puffin OpenGL Engine ver. 2.1
 * Coded by: Sebastian 'qbranchmaster' Tabaka
 * Contact: sebastian.tabaka@outlook.com
 */

#ifndef PUFFIN_POINT_LIGHT_H
#define PUFFIN_POINT_LIGHT_H

#ifdef WIN32 // Prevents APIENTRY redefinition
#include <Windows.h>
#endif // WIN32
#include <GL/glew.h>

#include <glm/glm.hpp>

#include <memory>

#include "PuffinEngine/Logger.hpp"

namespace puffin {
    class PointLight {
        friend class SceneLoader;

    public:
		PointLight(std::string name = "") {
            name_ = name;
		}

		std::string getName() const {
            return name_;
		}

        void enable(GLboolean state) {
            enabled_ = state;
        }

        GLboolean isEnabled() const {
            return enabled_;
        }

        void setAttenuation(GLfloat linear, GLfloat quadratic) {
            if (linear <= 0) {
                logError("PointLight::setAttenuation()", PUFFIN_MSG_INVALID_VALUE);
            }

            if (quadratic <= 0) {
                logError("PointLight::setAttenuation()", PUFFIN_MSG_INVALID_VALUE);
            }

            linear_factor_ = linear;
            quadratic_factor_ = quadratic;
        }

        GLfloat getLinearAttenuationFactor() const {
            return linear_factor_;
        }

        GLfloat getQuadraticAttenuationFactor() const {
            return quadratic_factor_;
        }

        void setPosition(const glm::vec3 &position) {
            position_ = position;
        }

        glm::vec3 getPosition() const {
            return position_;
        }

        void setColor(const glm::vec3 &color) {
            color_ = glm::vec3(color.r, color.g, color.b);
        }

        glm::vec3 getColor() const {
            return color_;
        }

    private:
        std::string name_;

        GLboolean enabled_{false};

        GLfloat linear_factor_{0.14f};
        GLfloat quadratic_factor_{0.07f};

        glm::vec3 color_{1.0f, 1.0f, 1.0f};
        glm::vec3 position_{0.0f, 0.0f, 0.0f};
    };

    using PointLightPtr = std::shared_ptr<PointLight>;
} // namespace puffin

#endif // PUFFIN_POINT_LIGHT_H