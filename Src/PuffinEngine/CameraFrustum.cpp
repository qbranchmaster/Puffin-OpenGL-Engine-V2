/*
* Puffin OpenGL Engine ver. 2.0
* Coded by: Sebastian 'qbranchmaster' Tabaka
*/

#include "PuffinEngine/CameraFrustum.hpp"

using namespace puffin;

void CameraFrustum::setCameraVectors(const glm::vec3 &direction,
    const glm::vec3 &right, const glm::vec3 &up) {
    direction_vector_ = glm::normalize(direction);
    right_vector_ = glm::normalize(right);
    up_vector_ = glm::normalize(up);
}

void CameraFrustum::setCameraParameters(GLfloat near_distance,
    GLfloat far_distance, GLfloat aspect, GLfloat fov) {
    if (aspect <= 0.0f || fov <= 0.0f) {
        logError("CameraFrustum::setCameraParameters()", "Invalid value.");
        return;
    }

    near_plane_ = near_distance;
    far_plane_ = far_distance;
    aspect_ = aspect;
    fov_ = fov;
}

void CameraFrustum::calculateFrustumPoints() {
    calculatePlanesSizes();

    glm::vec3 to_near_center = direction_vector_ * near_plane_;
    glm::vec3 to_far_center = direction_vector_ * far_plane_;

    glm::vec3 far_top = to_far_center + up_vector_ * (far_height_ / 2.0f);
    glm::vec3 far_bottom = to_far_center - up_vector_ * (far_height_ / 2.0f);
    glm::vec3 near_top = to_near_center + up_vector_ * (near_height_ / 2.0f);
    glm::vec3 near_bottom = to_near_center - up_vector_ * (near_height_ / 2.0f);

    frustum_points_[0] = calculatePointPosition(far_top, right_vector_,
        far_width_);
    frustum_points_[1] = calculatePointPosition(far_top, -right_vector_,
        far_width_);
    frustum_points_[2] = calculatePointPosition(far_bottom, right_vector_,
        far_width_);
    frustum_points_[3] = calculatePointPosition(far_bottom, -right_vector_,
        far_width_);
    frustum_points_[4] = calculatePointPosition(near_top, right_vector_,
        near_width_);
    frustum_points_[5] = calculatePointPosition(near_top, -right_vector_,
        near_width_);
    frustum_points_[6] = calculatePointPosition(near_bottom, right_vector_,
        near_width_);
    frustum_points_[7] = calculatePointPosition(near_bottom, -right_vector_,
        near_width_);
}

void CameraFrustum::recalculateToFrame(const glm::mat4 & frame) {
    for (auto &point : frustum_points_) {
        point = glm::vec3(frame * glm::vec4(point, 1.0f));
    }
}

void CameraFrustum::calculatePlanesSizes() {
    near_height_ = near_plane_ * std::tan(fov_ / 2.0f) * 2.0f;
    near_width_ = near_height_ * aspect_;

    far_height_ = far_plane_ * std::tan(fov_ / 2.0f) * 2.0f;
    far_width_ = far_height_ * aspect_;
}

glm::vec3 CameraFrustum::calculatePointPosition(const glm::vec3 &start,
    const glm::vec3 &direction, GLfloat width) {
    glm::vec3 point = start + direction * (width / 2.0f);
    return point;
}

void CameraFrustum::calculateBoundingBoxSize() {
    min_box_ = frustum_points_[0];
    max_box_ = frustum_points_[0];

    for (GLushort i = 0; i < frustum_points_.size(); i++) {
        min_box_.x = glm::min(min_box_.x, frustum_points_[i].x);
        min_box_.y = glm::min(min_box_.y, frustum_points_[i].y);
        min_box_.z = glm::min(min_box_.z, frustum_points_[i].z);

        max_box_.x = glm::max(max_box_.x, frustum_points_[i].x);
        max_box_.y = glm::max(max_box_.y, frustum_points_[i].y);
        max_box_.z = glm::max(max_box_.z, frustum_points_[i].z);
    }

    center_ = glm::vec3((min_box_.x + max_box_.x) / 2.0f,
        (min_box_.y + max_box_.y) / 2.0f, (min_box_.z + max_box_.z) / 2.0f);
}