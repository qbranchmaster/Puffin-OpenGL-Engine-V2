/*
* Puffin OpenGL Engine ver. 2.0
* Created by: Sebastian 'qbranchmaster' Tabaka
*/

#include "PuffinEngine/Camera.hpp"

#include "PuffinEngine/Configuration.hpp"
#include "PuffinEngine/Logger.hpp"

using namespace puffin;

Camera::Camera() {
    auto fs = Configuration::instance().getFrameResolution();
    aspect_ = static_cast<GLfloat>(fs.first) / static_cast<GLfloat>(fs.second);

    calculateRotationMatrix();
    calculateViewVectors();
    calculateViewMatrix();
    calculateProjectionMatrix();
}

void Camera::setProjection(GLfloat fov, GLfloat aspect, GLfloat near_plane,
    GLfloat far_plane) {
    if (fov <= 0.0f || aspect <= 0.0f) {
        logError("Camera::setProjection()", "Invalid input values.");
        return;
    }

    fov_ = fov;
    aspect_ = aspect;
    near_plane_ = near_plane;
    far_plane_ = far_plane;

    calculateProjectionMatrix();
}