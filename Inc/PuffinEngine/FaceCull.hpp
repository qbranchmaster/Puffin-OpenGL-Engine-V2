/*
* Puffin OpenGL Engine ver. 2.0
* Coded by: Sebastian 'qbranchmaster' Tabaka
*/

#ifndef PUFFIN_FACE_CULL_HPP
#define PUFFIN_FACE_CULL_HPP

#ifdef WIN32 // Prevents APIENTRY redefinition
#include <Windows.h>
#endif // WIN32
#include <GL/glew.h>

#include <memory>

namespace puffin {
    enum class CulledSide {
        BACK,
        FRONT,
        FRONT_AND_BACK,
    };

    class FaceCull {
    public:
        FaceCull() {
            glGetBooleanv(GL_CULL_FACE, &enabled_);

            GLint value = 0;
            glGetIntegerv(GL_CULL_FACE_MODE, &value);
            switch (value) {
            case GL_BACK:
                culled_side_ = CulledSide::BACK;
                break;
            case GL_FRONT:
                culled_side_ = CulledSide::FRONT;
                break;
            case GL_FRONT_AND_BACK:
                culled_side_ = CulledSide::FRONT_AND_BACK;
                break;
            }
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
            case CulledSide::BACK:
                glCullFace(GL_BACK);
                break;
            case CulledSide::FRONT:
                glCullFace(GL_FRONT);
                break;
            case CulledSide::FRONT_AND_BACK:
                glCullFace(GL_FRONT_AND_BACK);
                break;
            }

            culled_side_ = culled_side;
        }

        CulledSide getCulledSide() const {
            return culled_side_;
        }

    private:
        GLboolean enabled_{false};
        CulledSide culled_side_{CulledSide::BACK};
    };

    using FaceCullPtr = std::shared_ptr<FaceCull>;
} // namespace puffin

#endif // PUFFIN_FACE_CULL_HPP