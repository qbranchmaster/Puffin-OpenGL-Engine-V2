/*
* Puffin OpenGL Engine ver. 2.0
* Created by: Sebastian 'qbranchmaster' Tabaka
*/

#ifndef PUFFIN_DEPTH_TEST_HPP
#define PUFFIN_DEPTH_TEST_HPP

#include <GL/glew.h>

#include <memory>

namespace puffin {
    class DepthTest {
    public:
        DepthTest() {
            glGetBooleanv(GL_DEPTH_TEST, &enabled_);
            glGetBooleanv(GL_DEPTH_WRITEMASK, &depth_mask_enabled_);
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

    protected:
        GLboolean enabled_{false};
        GLboolean depth_mask_enabled_{false};
    };

    using DepthTestPtr = std::shared_ptr<DepthTest>;
} // namespace puffin

#endif // PUFFIN_DEPTH_TEST_HPP