/*
 * Puffin OpenGL Engine ver. 2.0.1
 * Coded by: Sebastian 'qbranchmaster' Tabaka
 * Contact: sebastian.tabaka@outlook.com
 */

#ifndef PUFFIN_ALPHA_BLEND_HPP
#define PUFFIN_ALPHA_BLEND_HPP

#include <GL/glew.h>

namespace puffin {
    enum class BlendFunction {
        Additive,
        Normal,
    };

    class AlphaBlend {
        friend class Core;

    public:
        static AlphaBlend &instance() {
            static AlphaBlend singleton;
            return singleton;
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
            case BlendFunction::Additive:
                glBlendFunc(GL_SRC_ALPHA, GL_ONE);
                break;
            case BlendFunction::Normal:
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

        void fetchState() {
            glGetBooleanv(GL_BLEND, &enabled_);

            GLint value = 0;
            glGetIntegerv(GL_BLEND_SRC_ALPHA, &value);
            switch (value) {
            case GL_ONE:
                blend_function_ = BlendFunction::Additive;
                break;
            case GL_ONE_MINUS_SRC_ALPHA:
                blend_function_ = BlendFunction::Normal;
                break;
            }
        }

        GLboolean enabled_{false};
        BlendFunction blend_function_{BlendFunction::Normal};
    };
} // namespace puffin

#endif // PUFFIN_ALPHA_BLEND_HPP