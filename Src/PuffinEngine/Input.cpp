#include "PuffinEngine/Input.hpp"

#include "PuffinEngine/Exception.hpp"
#include "PuffinEngine/Logger.hpp"

using namespace puffin;

Input::Input(WindowPtr window) {
    if (!window) {
        throw Exception("Input::Input()", "Not initialized object.");
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
        auto pressed = glfwGetKey(target_window_->handle_, 
            static_cast<GLint>(key));

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
    if (glfwGetMouseButton(target_window_->handle_, 
        static_cast<GLint>(button))) {
        return true;
    }
    
    return false;
}

void Input::setCursorPosition(GLdouble x, GLdouble y) {
    glfwSetCursorPos(target_window_->handle_, x, y);
}

void Input::getCursorPosition(GLfloat &x_pos, GLfloat &y_pos) {
    GLdouble cur_x = 0;
    GLdouble cur_y = 0;

    glfwGetCursorPos(target_window_->handle_, &cur_x, &cur_y);
    x_pos = static_cast<GLfloat>(cur_x);
    y_pos = static_cast<GLfloat>(cur_y);
}