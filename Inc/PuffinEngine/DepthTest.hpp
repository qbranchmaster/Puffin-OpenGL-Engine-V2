/*
 * Puffin OpenGL Engine ver. 2.1
 * Coded by: Sebastian 'qbranchmaster' Tabaka
 * Contact: sebastian.tabaka@outlook.com
 */

#ifndef PUFFIN_DEPTH_TEST_HPP
#define PUFFIN_DEPTH_TEST_HPP

#ifdef WIN32 // Prevents APIENTRY redefinition
#include <Windows.h>
#endif // WIN32
#include <GL/glew.h>

namespace puffin {
    class DepthTest {
        friend class Core;

    public:
        static DepthTest &instance() {
            static DepthTest singleton;
            return singleton;
        }

        void enable(GLboolean state) {
            if (state == enabled_) {
                return;
            }

            if (state) {
                glEnable(GL_DEPTH_TEST);
            }
            else {
                glDisable(GL_DEPTH_TEST);
            }

            enabled_ = state;
        }

        GLboolean isEnabled() const {
            return enabled_;
        }

        void enableDepthMask(GLboolean state) {
            if (state == depth_mask_enabled_) {
                return;
            }

            glDepthMask(state ? GL_TRUE : GL_FALSE);
            depth_mask_enabled_ = state;
        }

        GLboolean isDepthMaskEnabled() const {
            return depth_mask_enabled_;
        }

    private:
        DepthTest() {}
        DepthTest(const DepthTest &) = delete;
        void operator=(const DepthTest &) = delete;

        /**
         * Reads initial state.
         */
        void fetchState() {
            glGetBooleanv(GL_DEPTH_TEST, &enabled_);
            glGetBooleanv(GL_DEPTH_WRITEMASK, &depth_mask_enabled_);
        }

        GLboolean enabled_{false};
        GLboolean depth_mask_enabled_{false};
    };
} // namespace puffin

#endif // PUFFIN_DEPTH_TEST_HPP