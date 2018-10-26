#ifndef PUFFIN_INPUT_HPP
#define PUFFIN_INPUT_HPP

#include <map>
#include <memory>

#include "PuffinEngine/KeyCodes.hpp"
#include "PuffinEngine/Window.hpp"

namespace puffin {
    class Input {
    public:
        Input(WindowPtr window);

        GLboolean keyPressed(Key key, GLboolean sticky = true);
        GLboolean mouseKeyPressed(MouseButton button);
        void setCursorPosition(GLdouble x, GLdouble y);
        void getCursorPosition(GLfloat &x_pos, GLfloat &y_pos);

    private:
        WindowPtr target_window_;

        std::map<Key, GLboolean> key_states_;
    };

    using InputPtr = std::shared_ptr<Input>;
} // namespace puffin

#endif // PUFFIN_INPUT_HPP