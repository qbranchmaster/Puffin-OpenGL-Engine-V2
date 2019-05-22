/*
 * Puffin OpenGL Engine ver. 2.1
 * Coded by: Sebastian 'qbranchmaster' Tabaka
 * Contact: sebastian.tabaka@outlook.com
 */

#ifndef PUFFIN_WATER_TILE_HPP
#define PUFFIN_WATER_TILE_HPP

#include "PuffinEngine/BaseMesh.hpp"

namespace puffin {
    class WaterTile : public BaseMesh {
    public:
        WaterTile() {
            // clang-format off
            std::vector<GLfloat> positions = {
                -1.0f, 0.0f, -1.0f,
                -1.0f, 0.0f,  1.0f,
                 1.0f, 0.0f, -1.0f,
                 1.0f, 0.0f, -1.0f,
                -1.0f, 0.0f,  1.0f,
                 1.0f, 0.0f,  1.0f
            };
            // clang-format on

            bind();
            setMeshData(positions, 0, 3);
        }

        void setWaterColor(const glm::vec3 &color) {
            water_color_ = glm::vec3(glm::clamp(color.r, 0.0f, 1.0f),
                glm::clamp(color.g, 0.0f, 1.0f), glm::clamp(color.b, 0.0f, 1.0f));
        }

        glm::vec3 getWaterColor() const {
            return water_color_;
        }

        void setWaveStrength(GLfloat strength) {
            if (strength < 0.0f) {
                logError("WaterTile::setWaveStrength()", PUFFIN_MSG_INVALID_VALUE);
                return;
            }

            wave_strength_ = strength;
        }

        GLfloat getWaveStrength() const {
            return wave_strength_;
        }

        void setWaveSpeed(GLfloat speed) {
            if (speed < 0.0f) {
                logError("WaterTile::setWaveSpeed()", PUFFIN_MSG_INVALID_VALUE);
                return;
            }

            wave_speed_ = speed;
        }

        GLfloat getWaveSpeed() const {
            return wave_speed_;
        }

        void setShininess(GLuint shininess) {
            if (shininess <= 0) {
                logError("WaterTile::setShininess()", PUFFIN_MSG_INVALID_VALUE);
                return;
            }

            shininess_ = shininess;
        }

        GLint getShininess() const {
            return shininess_;
        }

    private:
        glm::vec3 water_color_{0.0f, 0.3f, 0.5f};
        GLuint shininess_{10};

        GLfloat wave_strength_{0.04f};
        GLfloat wave_speed_{0.01f};
    };

    using WaterTilePtr = std::shared_ptr<WaterTile>;
} // namespace puffin

#endif // PUFFIN_WATER_TILE_HPP