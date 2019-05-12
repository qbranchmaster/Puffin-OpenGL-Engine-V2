/*
* Puffin OpenGL Engine ver. 2.0
* Coded by: Sebastian 'qbranchmaster' Tabaka
* Contact: sebastian.tabaka@outlook.com
*/

#ifndef PUFFIN_CAMERA_FRUSTUM_HPP
#define PUFFIN_CAMERA_FRUSTUM_HPP

#ifdef WIN32 // Prevents APIENTRY redefinition
#include <Windows.h>
#endif // WIN32
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <array>
#include <memory>

#include "PuffinEngine/Logger.hpp"

namespace puffin {
    class CameraFrustum {
    public:
        void setCameraVectors(const glm::vec3 &direction, const glm::vec3 &right,
            const glm::vec3 &up, const glm::vec3 &position);
        void setCameraParameters(GLfloat near_distance, GLfloat far_distance, GLfloat aspect,
            GLfloat fov);

        void calculateFrustumPoints();
        void recalculateToFrame(const glm::mat4 &frame);
        void calculateBoundingBoxSize();

        glm::vec3 getCenter() const {
            return center_;
        }

        glm::vec3 getSize() const {
            return (max_box_ - min_box_) * 0.5f;
        }

    private:
        void calculatePlanesSizes();
        glm::vec3 calculatePointPosition(const glm::vec3 & start, const glm::vec3 & direction,
            GLfloat width);

        GLfloat near_plane_{1.0f};
        GLfloat far_plane_{10.0f};
        GLfloat aspect_{16.0f / 9.0f};
        GLfloat fov_{1.05f};

        GLfloat far_width_{0.0f};
        GLfloat far_height_{0.0f};
        GLfloat near_width_{0.0f};
        GLfloat near_height_{0.0f};

        glm::vec3 direction_vector_{0.0f, 0.0f, -1.0f};
        glm::vec3 right_vector_{1.0f, 0.0f, 0.0f};
        glm::vec3 up_vector_{0.0f, 1.0f, 0.0f};
        glm::vec3 camera_position_{0.0f, 0.0f, 0.0f};

        glm::vec3 min_box_{0.0f, 0.0f, 0.0f};
        glm::vec3 max_box_{0.0f, 0.0f, 0.0f};
        glm::vec3 center_{0.0f, 0.0f, 0.0f};

        std::array<glm::vec3, 8> frustum_points_;
    };

    using CameraFrustumPtr = std::shared_ptr<CameraFrustum>;
} // namespace puffin

#endif // PUFFIN_CAMERA_FRUSTUM_HPP