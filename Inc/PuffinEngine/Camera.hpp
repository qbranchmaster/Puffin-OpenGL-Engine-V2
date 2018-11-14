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

namespace puffin {
    class Camera {
    public:
        Camera();

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

    private:
        void calculateProjectionMatrix() {
            projection_matrix_ = glm::perspective(fov_, aspect_, near_plane_,
                far_plane_);

            //projection_matrix_inverted_ = glm::inverse(projection_matrix_);
        }

        void calculateViewMatrix() {
            view_matrix_ = rotation_matrix_ * glm::translate(glm::mat4(1.0f),
                -position_);

            // TODO: use glm::look_at

            //view_matrix_static_ = glm::mat4(glm::mat3(view_matrix_));
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
    };

    using CameraPtr = std::shared_ptr<Camera>;
} // namespace puffin

#endif // PUFFIN_CAMERA_HPP