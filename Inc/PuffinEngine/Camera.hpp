/*
* Puffin OpenGL Engine ver. 2.0
* Coded by: Sebastian 'qbranchmaster' Tabaka
* Contact: sebastian.tabaka@outlook.com
*/

#ifndef PUFFIN_CAMERA_HPP
#define PUFFIN_CAMERA_HPP

#ifdef WIN32 // Prevents APIENTRY redefinition
#include <Windows.h>
#endif // WIN32
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <memory>

#include "PuffinEngine/Configuration.hpp"
#include "PuffinEngine/Logger.hpp"

namespace puffin {
    enum class CameraMoveDirection {
        Forward,
        Backward,
        Left,
        Right,
    };

    class Camera {
    public:
        Camera();

        void setRotation(GLfloat horizontal, GLfloat vertical);
        void rotate(GLfloat vertical, GLfloat horizontal);

        GLfloat getHorizontalAngle() const {
            return horizontal_angle_;
        }

        GLfloat getVerticalAngle() const {
            return vertical_angle_;
        }

        void flipPitch() {
            setRotation(horizontal_angle_, -vertical_angle_);
        }

        void setPosition(const glm::vec3 &position);
        void translate(const glm::vec3 &translation);

        glm::vec3 getPosition() const {
            return position_;
        }

        void move(CameraMoveDirection direction);
        void setMoveSpeed(GLfloat move_speed);
        void setMoveResistanceFactor(GLfloat resistance_factor);
        void updateSpeed(GLdouble delta_time);

        GLfloat getMoveSpeed() const {
            return camera_move_speed_;
        }

        GLfloat getMoveResistanceFactor() const {
            return move_resistance_factor_;
        }

        void setFov(GLfloat fov);

        GLfloat getFov() const {
            return fov_;
        }

        void setAspect(GLfloat aspect);

        GLfloat getAspect() const {
            return aspect_;
        }

        void setProjection(GLfloat fov, GLfloat aspect, GLfloat near_plane, GLfloat far_plane);

        glm::mat4 getProjectionMatrix() const {
            return projection_matrix_;
        }

        glm::mat4 getProjectionMatrixInverted() const {
            return projection_matrix_inverted_;
        }

        GLfloat getNearPlane() const {
            return near_plane_;
        }

        GLfloat getFarPlane() const {
            return far_plane_;
        }

        glm::mat4 getViewMatrix() const {
            return view_matrix_;
        }

        glm::mat4 getViewMatrixStatic() const {
            return view_matrix_static_;
        }

        glm::mat4 getViewMatrixInverted() const {
            return view_matrix_inverted_;
        }

        glm::vec3 getDirectionVector() const {
            return direction_;
        }

        glm::vec3 getRightVector() const {
            return right_;
        }

        glm::vec3 getUpVector() const {
            return up_;
        }

    private:
        void calculateProjectionMatrix();
        void calculateViewMatrix();
        void calculateViewVectors();
        void calculateRotationMatrix();

        GLfloat horizontal_angle_{0.0f};
        GLfloat vertical_angle_{0.0f};

        glm::mat4 projection_matrix_{1.0f};
        glm::mat4 projection_matrix_inverted_{1.0f};

        glm::mat4 view_matrix_{1.0f};
        glm::mat4 view_matrix_static_{1.0f};
        glm::mat4 view_matrix_inverted_{1.0f};

        glm::mat4 rotation_matrix_{1.0f};
        glm::mat4 rotation_matrix_inverted_{1.0f};

        glm::vec3 position_{0.0f, 0.0f, 0.0f};
        glm::vec3 direction_{0.0f, 0.0f, -1.0f};
        glm::vec3 right_{1.0f, 0.0f, 0.0f};
        glm::vec3 up_{0.0f, 1.0f, 0.0f};

        GLfloat aspect_{16.0f / 9.0f};
        GLfloat near_plane_{0.1f};
        GLfloat far_plane_{10.0f};
        GLfloat fov_{1.05f};

        GLfloat camera_move_speed_{1.0f};
        GLfloat move_resistance_factor_{0.90f};
        GLfloat ahead_speed_{0.0f};
        GLfloat side_speed_{0.0f};
    };

    using CameraPtr = std::shared_ptr<Camera>;
} // namespace puffin

#endif // PUFFIN_CAMERA_HPP