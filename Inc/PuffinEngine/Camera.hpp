/*
* Puffin OpenGL Engine ver. 2.0
* Created by: Sebastian 'qbranchmaster' Tabaka
*/

#ifndef PUFFIN_CAMERA_HPP
#define PUFFIN_CAMERA_HPP

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <memory>

#include "PuffinEngine/Logger.hpp"

namespace puffin {
    enum class CameraMoveDirection {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT,
    };

    class Camera {
        friend class MasterRenderer;

    public:
        Camera();

        void setRotation(GLfloat horizontal, GLfloat vertical) {
            horizontal_angle_ = horizontal;
            vertical_angle_ = vertical;

            calculateRotationMatrix();
            calculateViewVectors();
            calculateViewMatrix();
        }

        void rotate(GLfloat vertical, GLfloat horizontal) {
            horizontal_angle_ += horizontal;
            vertical_angle_ += vertical;

            calculateRotationMatrix();
            calculateViewVectors();
            calculateViewMatrix();
        }

        GLfloat getHorizontalAngle() const {
            return horizontal_angle_;
        }

        GLfloat getVerticalAngle() const {
            return vertical_angle_;
        }

        glm::mat4 getProjectionMatrix() const {
            return projection_matrix_;
        }

        glm::mat4 getProjectionMatrixInverted() const {
            return projection_matrix_inverted_;
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

        void setFov(GLfloat fov) {
            setProjection(fov, aspect_, near_plane_, far_plane_);
        }

        GLfloat getFov() const {
            return fov_;
        }

        void setAspect(GLfloat aspect) {
            setProjection(fov_, aspect, near_plane_, far_plane_);
        }

        GLfloat getAspect() const {
            return aspect_;
        }

        GLfloat getNearPlane() const {
            return near_plane_;
        }

        GLfloat getFarPlane() const {
            return far_plane_;
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

        void setPosition(const glm::vec3 &position) {
            position_ = position;
            calculateViewMatrix();
        }

        glm::vec3 getPosition() const {
            return position_;
        }

        void translate(const glm::vec3 &translation) {
            position_ += translation;
            calculateViewMatrix();
        }

        void setProjection(GLfloat fov, GLfloat aspect, GLfloat near_plane,
            GLfloat far_plane);

        void move(CameraMoveDirection direction) {
            switch (direction) {
            case CameraMoveDirection::FORWARD:
                ahead_speed_ = camera_move_speed_;
                break;
            case CameraMoveDirection::BACKWARD:
                ahead_speed_ = -camera_move_speed_;
                break;
            case CameraMoveDirection::LEFT:
                side_speed_ = -camera_move_speed_;
                break;
            case CameraMoveDirection::RIGHT:
                side_speed_ = camera_move_speed_;
                break;
            }
        }

        void setMoveSpeed(GLfloat move_speed) {
            if (move_speed <= 0.0f) {
                logError("Camera::setMoveSpeed()",
                    "Invalid camera move speed value.");
                return;
            }

            camera_move_speed_ = move_speed;
        }

        GLfloat getMoveSpeed() const {
            return camera_move_speed_;
        }

        void setMoveResistanceFactor(GLfloat resistance_factor) {
            if (resistance_factor < 0.0f || resistance_factor > 1.0f) {
                logError("Camera::setMoveResistanceFactor()",
                    "Invalid camera move resistance factor value.");
                return;
            }

            move_resistance_factor_ = resistance_factor;
        }

        GLfloat getMoveResistanceFactor() const {
            return move_resistance_factor_;
        }

    private:
        void calculateProjectionMatrix() {
            projection_matrix_ = glm::perspective(fov_, aspect_, near_plane_,
                far_plane_);

            //projection_matrix_inverted_ = glm::inverse(projection_matrix_);
        }

        void calculateViewMatrix() {
            view_matrix_ = glm::lookAt(position_, position_ + direction_, up_);

            // This is also correct calculation of view matrix.
            /*view_matrix_ = rotation_matrix_ * glm::translate(glm::mat4(1.0f),
                -position_);*/

            view_matrix_static_ = glm::mat4(glm::mat3(view_matrix_));
            //view_matrix_inverted_ = glm::inverse(view_matrix_);
        }

        void calculateViewVectors() {
            direction_ = glm::vec3(rotation_matrix_inverted_ *
                glm::vec4(0.0f, 0.0f, -1.0f, 0.0f));
            right_ = glm::vec3(rotation_matrix_inverted_ *
                glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
            up_ = glm::vec3(rotation_matrix_inverted_ *
                glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
        }

        void calculateRotationMatrix() {
            rotation_matrix_ = glm::mat4(1.0f);
            rotation_matrix_ = glm::rotate(rotation_matrix_, vertical_angle_,
                glm::vec3(1.0f, 0.0f, 0.0f));
            rotation_matrix_ = glm::rotate(rotation_matrix_, horizontal_angle_,
                glm::vec3(0.0f, 1.0f, 0.0f));

            rotation_matrix_inverted_ = glm::inverse(rotation_matrix_);
        }

        void update(GLdouble delta_time) {
            translate(direction_ * ahead_speed_ *
                static_cast<float>(delta_time));
            translate(right_ * side_speed_ *
                static_cast<float>(delta_time));

            ahead_speed_ *= move_resistance_factor_;
            side_speed_ *= move_resistance_factor_;
        }

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
        GLfloat near_plane_{0.01f};
        GLfloat far_plane_{100.0f};
        GLfloat fov_{1.05f};

        GLfloat camera_move_speed_{1.0f};
        GLfloat move_resistance_factor_{0.85f};
        GLfloat ahead_speed_{0.0f};
        GLfloat side_speed_{0.0f};
    };

    using CameraPtr = std::shared_ptr<Camera>;
} // namespace puffin

#endif // PUFFIN_CAMERA_HPP