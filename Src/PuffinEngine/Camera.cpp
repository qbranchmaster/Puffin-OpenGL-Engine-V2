/*
* Puffin OpenGL Engine ver. 2.0
* Coded by: Sebastian 'qbranchmaster' Tabaka
*/

#include "PuffinEngine/Camera.hpp"

using namespace puffin;

Camera::Camera() {
    aspect_ = static_cast<GLfloat>(Configuration::instance().getFrameWidth())
        / static_cast<GLfloat>(Configuration::instance().getFrameHeight());

    calculateRotationMatrix();
    calculateViewVectors();
    calculateViewMatrix();
    calculateProjectionMatrix();
}

void Camera::setRotation(GLfloat horizontal, GLfloat vertical) {
    horizontal_angle_ = horizontal;
    vertical_angle_ = vertical;

    calculateRotationMatrix();
    calculateViewVectors();
    calculateViewMatrix();
}

void Camera::rotate(GLfloat vertical, GLfloat horizontal) {
    horizontal_angle_ += horizontal;
    vertical_angle_ += vertical;

    calculateRotationMatrix();
    calculateViewVectors();
    calculateViewMatrix();
}

void Camera::setPosition(const glm::vec3 &position) {
    position_ = position;
    calculateViewMatrix();
}

void Camera::translate(const glm::vec3 &translation) {
    position_ += translation;
    calculateViewMatrix();
}

void Camera::move(CameraMoveDirection direction) {
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

void Camera::setMoveSpeed(GLfloat move_speed) {
    if (move_speed <= 0.0f) {
        logError("Camera::setMoveSpeed()", "Invalid value.");
        return;
    }

    camera_move_speed_ = move_speed;
}

void Camera::setMoveResistanceFactor(GLfloat resistance_factor) {
    if (resistance_factor < 0.0f || resistance_factor > 1.0f) {
        logError("Camera::setMoveResistanceFactor()", "Invalid value.");
        return;
    }

    move_resistance_factor_ = resistance_factor;
}

void Camera::setFov(GLfloat fov) {
    setProjection(fov, aspect_, near_plane_, far_plane_);
}

void Camera::setAspect(GLfloat aspect) {
    setProjection(fov_, aspect, near_plane_, far_plane_);
}

void Camera::setProjection(GLfloat fov, GLfloat aspect, GLfloat near_plane,
    GLfloat far_plane) {
    if (aspect <= 0.0f) {
        logError("Camera::setProjection()", "Invalid value.");
        return;
    }

    fov_ = fov;
    aspect_ = aspect;
    near_plane_ = near_plane;
    far_plane_ = far_plane;

    calculateProjectionMatrix();
}

void Camera::calculateProjectionMatrix() {
    projection_matrix_ = glm::perspective(fov_, aspect_, near_plane_,
        far_plane_);

    //projection_matrix_inverted_ = glm::inverse(projection_matrix_);
}

void Camera::calculateViewMatrix() {
    view_matrix_ = glm::lookAt(position_, position_ + direction_, up_);

    // This is also correct calculation of view matrix.
    /*view_matrix_ = rotation_matrix_ * glm::translate(glm::mat4(1.0f),
        -position_);*/

    view_matrix_static_ = glm::mat4(glm::mat3(view_matrix_));
    view_matrix_inverted_ = glm::inverse(view_matrix_);
}

void Camera::calculateViewVectors() {
    direction_ = glm::vec3(rotation_matrix_inverted_ *
        glm::vec4(0.0f, 0.0f, -1.0f, 0.0f));
    right_ = glm::vec3(rotation_matrix_inverted_ *
        glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
    up_ = glm::vec3(rotation_matrix_inverted_ *
        glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
}

void Camera::calculateRotationMatrix() {
    rotation_matrix_ = glm::mat4(1.0f);
    rotation_matrix_ = glm::rotate(rotation_matrix_, vertical_angle_,
        glm::vec3(1.0f, 0.0f, 0.0f));
    rotation_matrix_ = glm::rotate(rotation_matrix_, horizontal_angle_,
        glm::vec3(0.0f, 1.0f, 0.0f));

    rotation_matrix_inverted_ = glm::inverse(rotation_matrix_);
}

void Camera::updateSpeed(GLdouble delta_time) {
    translate(direction_ * ahead_speed_ * static_cast<float>(delta_time));
    translate(right_ * side_speed_ * static_cast<float>(delta_time));

    ahead_speed_ *= move_resistance_factor_;
    side_speed_ *= move_resistance_factor_;
}