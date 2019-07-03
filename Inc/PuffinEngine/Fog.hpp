/*
 * Puffin OpenGL Engine ver. 2.0.1
 * Coded by: Sebastian 'qbranchmaster' Tabaka
 * Contact: sebastian.tabaka@outlook.com
 */

#ifndef PUFFIN_FOG_HPP
#define PUFFIN_FOG_HPP

#include <GL/glew.h>

#include <memory>

#include "PuffinEngine/Logger.hpp"

namespace puffin {
    class Fog {
        friend class SceneLoader;

    public:
        void enable(GLboolean state) {
            enabled_ = state;
        }

        GLboolean isEnabled() const {
            return enabled_;
        }

        void setDensity(GLfloat density) {
            if (density < 0.0f || density > 1.0f) {
                logError("Fog::setDensity()", PUFFIN_MSG_INVALID_VALUE);
                return;
            }

            density_ = density;
        }

        GLfloat getDensity() const {
            return density_;
        }

        void setColor(const glm::vec3 &color) {
            color_ = glm::vec3(glm::clamp(color.r, 0.0f, 1.0f), glm::clamp(color.g, 0.0f, 1.0f),
                glm::clamp(color.b, 0.0f, 1.0f));
        }

        glm::vec3 getColor() const {
            return color_;
        }

        void setSkyboxFog(GLfloat overall_density, GLfloat transition_power, GLfloat height) {
            if (overall_density < 0.0f || overall_density > 1.0f) {
                logError("Fog::setSkyboxFog()", PUFFIN_MSG_INVALID_VALUE);
                return;
            }

            if (transition_power < 0.0f) {
                logError("Fog::setSkyboxFog()", PUFFIN_MSG_INVALID_VALUE);
                return;
            }

            if (height < 0.0f || height > 1.0f) {
                logError("Fog::setSkyboxFog()", PUFFIN_MSG_INVALID_VALUE);
                return;
            }

            skybox_fog_overall_density_ = overall_density;
            skybox_fog_transition_power_ = transition_power;
            skybox_fog_height_ = height;
        }

        GLfloat getSkyboxFogOverallDensity() const {
            return skybox_fog_overall_density_;
        }

        GLfloat getSkyboxFogTransitionPower() const {
            return skybox_fog_transition_power_;
        }

        GLfloat getSkyboxFogHeight() const {
            return skybox_fog_height_;
        }

    private:
        GLboolean enabled_{false};

        GLfloat density_{0.05f};
        glm::vec3 color_{0.5f, 0.5f, 0.5f};

        GLfloat skybox_fog_overall_density_{0.0f};
        GLfloat skybox_fog_transition_power_{1.0f};
        GLfloat skybox_fog_height_{0.6f};
    };

    using FogPtr = std::shared_ptr<Fog>;
} // namespace puffin

#endif // PUFFIN_FOG_HPP