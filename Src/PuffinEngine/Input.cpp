/*
 * Puffin OpenGL Engine ver. 2.0.1
 * Coded by: Sebastian 'qbranchmaster' Tabaka
 * Contact: sebastian.tabaka@outlook.com
 */

#include "PuffinEngine/Input.hpp"

using namespace puffin;

Input::Input(WindowPtr window) {
    if (!window) {
        throw Exception("Input::Input()", PUFFIN_MSG_NULL_OBJECT);
    }

    target_window_ = window;
}

GLboolean Input::keyPressed(Key key, GLboolean sticky) {
    GLboolean result = false;
    if (sticky) {
        if (glfwGetKey(target_window_->handle_, static_cast<GLint>(key))) {
            result = true;
        }
    }
    else {
        auto pressed = glfwGetKey(target_window_->handle_, static_cast<GLint>(key));

        if (pressed && key_states_[key] == false) {
            result = true;
            key_states_[key] = true;
        }
        else if (!pressed) {
            key_states_[key] = false;
        }
    }

    return result;
}

GLboolean Input::mouseKeyPressed(MouseButton button) {
    if (glfwGetMouseButton(target_window_->handle_, static_cast<GLint>(button))) {
        return true;
    }

    return false;
}

void Input::setCursorPosition(GLdouble x, GLdouble y) {
    glfwSetCursorPos(target_window_->handle_, x, y);
}

void Input::getCursorPosition(GLdouble &x_pos, GLdouble &y_pos) const {
    glfwGetCursorPos(target_window_->handle_, &x_pos, &y_pos);
}