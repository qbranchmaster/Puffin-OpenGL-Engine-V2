/*
 * Puffin OpenGL Engine ver. 2.1
 * Coded by: Sebastian 'qbranchmaster' Tabaka
 * Contact: sebastian.tabaka@outlook.com
 */

#ifndef PUFFIN_LIGHTING_HPP
#define PUFFIN_LIGHTING_HPP

#ifdef WIN32 // Prevents APIENTRY redefinition
#include <Windows.h>
#endif // WIN32
#include <GL/glew.h>

#include <glm/glm.hpp>

#include <memory>
#include <vector>

#include "PuffinEngine/DirectionalLight.hpp"
#include "PuffinEngine/PointLight.hpp"

namespace puffin {
    class Lighting {
        friend class SceneLoader;

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

        void enableBlinnPhong(GLboolean state) {
            blinn_phong_enabled_ = state;
        }

        GLboolean isBlinnPhongEnabled() const {
            return blinn_phong_enabled_;
        }

        void setSkyboxLightingColor(const glm::vec3 &color) {
            skybox_light_color_ = glm::vec3(glm::clamp(color.r, 0.0f, 1.0f),
                glm::clamp(color.g, 0.0f, 1.0f), glm::clamp(color.b, 0.0f, 1.0f));
        }

        glm::vec3 getSkyboxLightColor() const {
            return skybox_light_color_;
        }

        DirectionalLightPtr directionalLight() const {
            return directional_light_;
        }

        void enableShadowMapping(GLboolean state) {
            shadow_mapping_enabled_ = state;
        }

        GLboolean isShadowMappingEnabled() const {
            return shadow_mapping_enabled_;
        }

        void setShadowDistance(GLfloat distance) {
            if (distance <= 0.0f) {
                logError("Lighting::setShadowDistance()", PUFFIN_MSG_INVALID_VALUE);
                return;
            }

            shadow_distance_ = distance;
        }

        GLfloat getShadowDistance() const {
            return shadow_distance_;
        }

        void setEmissionFactor(GLfloat factor) {
            if (factor < 0.0f) {
                logError("Lighting::setEmissionFactor()", PUFFIN_MSG_INVALID_VALUE);
                return;
            }

            emission_factor_ = factor;
        }

        GLfloat getEmissionFactor() const {
            return emission_factor_;
        }

        void setShadowMappingPcfsamplesCount(GLuint samples) {
            shadows_pcf_samples_count_ = samples;
        }

        GLuint getShadowMappingPcfSamplesCount() const {
            return shadows_pcf_samples_count_;
        }

        void addPointLight(PointLightPtr point_light) {
            if (!point_light) {
                logError("Lighting::addPointLight()", PUFFIN_MSG_NULL_OBJECT);
                return;
            }

			if (point_lights_.size() >= max_point_lights_count_) {
                logError("Lighting::addPointLight()",
                    PUFFIN_MSG_OUT_OF_RANGE(0, max_point_lights_count_));
                return;
			}

            point_lights_.push_back(point_light);
        }

        PointLightPtr getPointLight(GLuint index) {
            if (index >= point_lights_.size()) {
                logError(
                    "Lighting::getPointLight()", PUFFIN_MSG_OUT_OF_RANGE(0, point_lights_.size()));
                return nullptr;
            }

            return point_lights_[index];
        }

        GLuint getPointLightsCount() const {
            return point_lights_.size();
        }

		GLushort getMaxPointLightsCount() const {
            return max_point_lights_count_;
		}

    private:
        GLboolean enabled_{false};
        GLboolean blinn_phong_enabled_{false};

        GLboolean shadow_mapping_enabled_{true};
        GLfloat shadow_distance_{10.0f};
        GLfloat emission_factor_{10.0f};
        GLuint shadows_pcf_samples_count_{1};

        glm::vec3 skybox_light_color_{1.0f, 1.0f, 1.0f};

        DirectionalLightPtr directional_light_{nullptr};
        std::vector<PointLightPtr> point_lights_;

        constexpr static GLushort max_point_lights_count_{4};
    };

    using LightingPtr = std::shared_ptr<Lighting>;
} // namespace puffin

#endif // PUFFIN_LIGHTING_HPP