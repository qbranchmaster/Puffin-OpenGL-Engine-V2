/*
 * Puffin OpenGL Engine ver. 2.1
 * Coded by: Sebastian 'qbranchmaster' Tabaka
 * Contact: sebastian.tabaka@outlook.com
 */

#ifndef PUFFIN_FACE_CULL_HPP
#define PUFFIN_FACE_CULL_HPP

#ifdef WIN32 // Prevents APIENTRY redefinition
#include <Windows.h>
#endif // WIN32
#include <GL/glew.h>

namespace puffin {
    enum class CulledSide {
        Back,
        Front,
        FrontAndBack,
    };

    class FaceCull {
        friend class Core;

    public:
        static FaceCull &instance() {
            static FaceCull singleton;
            return singleton;
        }

        void enable(GLboolean state) {
            if (state == enabled_) {
                return;
            }

            if (state) {
                glEnable(GL_CULL_FACE);
            }
            else {
                glDisable(GL_CULL_FACE);
            }

            enabled_ = state;
        }

        GLboolean isEnabled() const {
            return enabled_;
        }

        void setCulledSide(CulledSide culled_side) {
            if (culled_side == culled_side_) {
                return;
            }

            switch (culled_side) {
            case CulledSide::Back:
                glCullFace(GL_BACK);
                break;
            case CulledSide::Front:
                glCullFace(GL_FRONT);
                break;
            case CulledSide::FrontAndBack:
                glCullFace(GL_FRONT_AND_BACK);
                break;
            }

            culled_side_ = culled_side;
        }

        CulledSide getCulledSide() const {
            return culled_side_;
        }

    private:
        FaceCull() {}
        FaceCull(const FaceCull &) = delete;
        void operator=(const FaceCull &) = delete;

        /**
         * Reads initial state.
         */
        void fetchState() {
            glGetBooleanv(GL_CULL_FACE, &enabled_);

            GLint value = 0;
            glGetIntegerv(GL_CULL_FACE_MODE, &value);
            switch (value) {
            case GL_BACK:
                culled_side_ = CulledSide::Back;
                break;
            case GL_FRONT:
                culled_side_ = CulledSide::Front;
                break;
            case GL_FRONT_AND_BACK:
                culled_side_ = CulledSide::FrontAndBack;
                break;
            }
        }

        GLboolean enabled_{false};
        CulledSide culled_side_{CulledSide::Back};
    };
} // namespace puffin

#endif // PUFFIN_FACE_CULL_HPP