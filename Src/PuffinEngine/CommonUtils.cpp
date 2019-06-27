/*
 * Puffin OpenGL Engine ver. 2.1
 * Coded by: Sebastian 'qbranchmaster' Tabaka
 * Contact: sebastian.tabaka@outlook.com
 */

#include "PuffinEngine/CommonUtils.hpp"

using namespace puffin;

std::ostream &puffin::operator<<(std::ostream &os, CameraPtr camera) {
    if (!camera) {
        return os;
    }

    os << camera->horizontal_angle_ << ';';
    os << camera->vertical_angle_ << ';';

    os << camera->projection_matrix_ << ';';
    os << camera->projection_matrix_inverted_ << ';';

    os << camera->view_matrix_ << ';';
    os << camera->view_matrix_static_ << ';';
    os << camera->view_matrix_inverted_ << ';';

    os << camera->rotation_matrix_ << ';';
    os << camera->rotation_matrix_inverted_ << ';';

    os << camera->position_ << ';';
    os << camera->direction_ << ';';
    os << camera->right_ << ';';
    os << camera->up_ << ';';

    os << camera->aspect_ << ';';
    os << camera->near_plane_ << ';';
    os << camera->far_plane_ << ';';
    os << camera->fov_ << ';';

    os << camera->camera_move_speed_ << ';';
    os << camera->move_resistance_factor_ << ';';

    return os;
}

std::istream &puffin::operator>>(std::istream &is, CameraPtr camera) {
    if (!camera) {
        return is;
    }

    is >> camera->horizontal_angle_;
    is >> camera->vertical_angle_;

    is >> camera->projection_matrix_;
    is >> camera->projection_matrix_inverted_;

    is >> camera->view_matrix_;
    is >> camera->view_matrix_static_;
    is >> camera->view_matrix_inverted_;

    is >> camera->rotation_matrix_;
    is >> camera->rotation_matrix_inverted_;

    is >> camera->position_;
    is >> camera->direction_;
    is >> camera->right_;
    is >> camera->up_;

	is >> camera->aspect_;
    is >> camera->near_plane_;
    is >> camera->far_plane_;
    is >> camera->fov_;

	is >> camera->camera_move_speed_;
    is >> camera->move_resistance_factor_;

    return is;
}

std::ostream &puffin::operator<<(std::ostream &os, SkyboxPtr skybox) {
    if (!skybox) {
        return;
	}

    return os;
}

std::istream &puffin::operator>>(std::istream &is, SkyboxPtr skybox) {
    return is;
}

std::ostream &puffin::operator<<(std::ostream &os, glm::vec3 &vec) {
    os << '[' << vec.x << ',' << vec.y << ',' << vec.z << ']';
    return os;
}

std::istream &puffin::operator>>(std::istream &is, glm::vec3 &vec) {
    constexpr GLushort size = 3;

    GLfloat vals[size] = {0.0f};

    is.ignore(3, '[');
    for (GLushort i = 0; i < size; i++) {
        is >> vals[i];
        if (i < size - 1) {
            is.ignore(3, ',');
        }
    }

    is.ignore(3, ']');
    vec = glm::make_vec3(vals);

    return is;
}

std::ostream &puffin::operator<<(std::ostream &os, glm::mat4 &mat) {
    constexpr GLushort size = 16;
    const GLfloat *mat_ptr = (const GLfloat *)glm::value_ptr(mat);

    os << '[';

    for (GLushort i = 0; i < size; i++) {
        os << mat_ptr[i];
        if (i < size - 1) {
            os << ',';
        }
    }

    os << ']';

    return os;
}

std::istream &puffin::operator>>(std::istream &is, glm::mat4 &mat) {
    constexpr GLushort size = 16;

    GLfloat vals[size] = {0.0f};

    is.ignore(3, '[');
    for (GLushort i = 0; i < size; i++) {
        is >> vals[i];
        if (i < size - 1) {
            is.ignore(3, ',');
        }
    }

    is.ignore(3, ']');
    mat = glm::make_mat4(vals);

    return is;
}