/*
* Puffin OpenGL Engine ver. 2.0
* Coded by: Sebastian 'qbranchmaster' Tabaka
*/

#ifndef PUFFIN_ALPHA_BLEND_HPP
#define PUFFIN_ALPHA_BLEND_HPP

#ifdef WIN32 // Prevents APIENTRY redefinition
#include <Windows.h>
#endif // WIN32
#include <GL/glew.h>

namespace puffin {
    enum class BlendFunction {
        ADDITIVE,
        NORMAL,
    };

    class AlphaBlend {
    public:
        static AlphaBlend& instance() {
            static AlphaBlend singleton;
            return singleton;
        }

        void fetchState() {
            glGetBooleanv(GL_BLEND, &enabled_);

            GLint value = 0;
            glGetIntegerv(GL_BLEND_SRC_ALPHA, &value);
            switch (value) {
            case GL_ONE:
                blend_function_ = BlendFunction::ADDITIVE;
                break;
            case GL_ONE_MINUS_SRC_ALPHA:
                blend_function_ = BlendFunction::NORMAL;
                break;
            }
        }

        void enable(GLboolean state) {
            if (state == enabled_) {
                return;
            }

            if (state) {
                glEnable(GL_BLEND);
            }
            else {
                glDisable(GL_BLEND);
            }

            enabled_ = state;
        }

        GLboolean isEnabled() const {
            return enabled_;
        }

        void setBlendFunction(BlendFunction function) {
            if (function == blend_function_) {
                return;
            }

            switch (function) {
            case BlendFunction::ADDITIVE:
                glBlendFunc(GL_SRC_ALPHA, GL_ONE);
                break;
            case BlendFunction::NORMAL:
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                break;
            }

            blend_function_ = function;
        }

        BlendFunction getBlendFunction() const {
            return blend_function_;
        }

    private:
        AlphaBlend() {}
        AlphaBlend(const AlphaBlend &) = delete;
        void operator=(const AlphaBlend &) = delete;

        GLboolean enabled_{false};
        BlendFunction blend_function_{BlendFunction::NORMAL};
    };
} // namespace puffin

#endif // PUFFIN_ALPHA_BLEND_HPP