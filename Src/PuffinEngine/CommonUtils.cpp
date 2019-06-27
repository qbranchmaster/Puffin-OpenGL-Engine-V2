/*
 * Puffin OpenGL Engine ver. 2.1
 * Coded by: Sebastian 'qbranchmaster' Tabaka
 * Contact: sebastian.tabaka@outlook.com
 */

#include "PuffinEngine/CommonUtils.hpp"

using namespace puffin;

std::ostream &puffin::operator<<(std::ostream &os, const glm::vec3 &vec) {
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

std::ostream &puffin::operator<<(std::ostream &os, const glm::mat4 &mat) {
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