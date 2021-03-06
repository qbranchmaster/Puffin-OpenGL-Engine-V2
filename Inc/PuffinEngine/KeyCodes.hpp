/*
 * Puffin OpenGL Engine ver. 2.0.1
 * Coded by: Sebastian 'qbranchmaster' Tabaka
 * Contact: sebastian.tabaka@outlook.com
 */

#ifndef PUFFIN_KEY_CODES_HPP
#define PUFFIN_KEY_CODES_HPP

namespace puffin {
    enum class Key {
        // clang-format off
        Space           = 32,
        Apostrophe      = 39, /* ' */
        Comma           = 44, /* , */
        Minus           = 45, /* - */
        Period          = 46, /* . */
        Slash           = 47, /* / */
        Num0            = 48,
        Num1            = 49,
        Num2            = 50,
        Num3            = 51,
        Num4            = 52,
        Num5            = 53,
        Num6            = 54,
        Num7            = 55,
        Num8            = 56,
        Num9            = 57,
        Semicolon       = 59, /* ; */
        Equal           = 61, /* = */
        A               = 65,
        B               = 66,
        C               = 67,
        D               = 68,
        E               = 69,
        F               = 70,
        G               = 71,
        H               = 72,
        I               = 73,
        J               = 74,
        K               = 75,
        L               = 76,
        M               = 77,
        N               = 78,
        O               = 79,
        P               = 80,
        Q               = 81,
        R               = 82,
        S               = 83,
        T               = 84,
        U               = 85,
        V               = 86,
        W               = 87,
        X               = 88,
        Y               = 89,
        Z               = 90,
        LeftBracket     = 91, /* [ */
        Backslash       = 92, /* \ */
        RightBracket    = 93, /* ] */
        GraveAccent     = 96, /* ` */
        Escape          = 256,
        Enter           = 257,
        Tab             = 258,
        Backspace       = 259,
        Insert          = 260,
        Delete          = 261,
        ArrowRight      = 262,
        ArrowLeft       = 263,
        ArrowDown       = 264,
        ArrowUp         = 265,
        PageUp          = 266,
        PageDown        = 267,
        Home            = 268,
        End             = 269,
        CapsLock        = 280,
        ScrollLock      = 281,
        NumLock         = 282,
        PrintScreen     = 283,
        Pause           = 284,
        F1              = 290,
        F2              = 291,
        F3              = 292,
        F4              = 293,
        F5              = 294,
        F6              = 295,
        F7              = 296,
        F8              = 297,
        F9              = 298,
        F10             = 299,
        F11             = 300,
        F12             = 301,
        KeypadNum0      = 320,
        KeypadNum1      = 321,
        KeypadNum2      = 322,
        KeypadNum3      = 323,
        KeypadNum4      = 324,
        KeypadNum5      = 325,
        KeypadNum6      = 326,
        KeypadNum7      = 327,
        KeypadNum8      = 328,
        KeypadNum9      = 329,
        KeypadDecimal   = 330,
        KeypadDivide    = 331,
        KeypadMultiply  = 332,
        KeypadSubtract  = 333,
        KeypadAdd       = 334,
        KeypadEnter     = 335,
        KeypadEqual     = 336,
        LeftShift       = 340,
        LeftControl     = 341,
        LeftAlt         = 342,
        LeftSuper       = 343,
        RightShift      = 344,
        RightControl    = 345,
        RightAlt        = 346,
        RightSuper      = 347,
        Menu            = 348,
    };

    enum class MouseButton {
        Left   = 0,
        Right  = 1,
        Middle = 2,
    };
    // clang-format on
} // namespace puffin

#endif // PUFFIN_KEY_CODES_HPP