/*
* Puffin OpenGL Engine ver. 2.0
* Coded by: Sebastian 'qbranchmaster' Tabaka
* Contact: sebastian.tabaka@outlook.com
*/

#ifndef PUFFIN_INPUT_HPP
#define PUFFIN_INPUT_HPP

#include <map>
#include <memory>

#include "PuffinEngine/Exception.hpp"
#include "PuffinEngine/KeyCodes.hpp"
#include "PuffinEngine/Window.hpp"

namespace puffin {
    class Input {
    public:
        Input(WindowPtr window);

        GLboolean keyPressed(Key key, GLboolean sticky = true);
        GLboolean mouseKeyPressed(MouseButton button);

        void setCursorPosition(GLfloat x, GLfloat y);
        void getCursorPosition(GLfloat &x_pos, GLfloat &y_pos) const;

    private:
        std::map<Key, GLboolean> key_states_;

        WindowPtr target_window_;
    };

    using InputPtr = std::shared_ptr<Input>;
} // namespace puffin

#endif // PUFFIN_INPUT_HPP