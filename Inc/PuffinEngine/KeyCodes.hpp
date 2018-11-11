/*
* Puffin OpenGL Engine ver. 2.0
* Created by: Sebastian 'qbranchmaster' Tabaka
*/

#ifndef PUFFIN_KEY_CODES_HPP
#define PUFFIN_KEY_CODES_HPP

namespace puffin {
    enum class Key {
        NUM_0        = 48,
        NUM_1        = 49,
        NUM_2        = 50,
        NUM_3        = 51,
        NUM_4        = 52,
        NUM_5        = 53,
        NUM_6        = 54,
        NUM_7        = 55,
        NUM_8        = 56,
        NUM_9        = 57,
        A            = 65,
        D            = 68,
        S            = 83,
        T            = 84,
        W            = 87,
        ESCAPE       = 256,
        ARROW_RIGHT  = 262,
        ARROW_LEFT   = 263,
        ARROW_DOWN   = 264,
        ARROW_UP     = 265,
        F1           = 290,
        F2           = 291,
        F3           = 292,
        F4           = 293,
        F5           = 294,
        F6           = 295,
        F7           = 296,
        F8           = 297,
        F9           = 298,
        F10          = 299,
        F11          = 300,
        LEFT_SHIFT   = 340,
        LEFT_CONTROL = 341,
    };

    enum class MouseButton {
        LEFT   = 0,
        RIGHT  = 1,
        MIDDLE = 2,
    };
} // namespace puffin

#endif // PUFFIN_KEY_CODES_HPP