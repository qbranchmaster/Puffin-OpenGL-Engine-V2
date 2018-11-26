/*
* Puffin OpenGL Engine ver. 2.0
* Created by: Sebastian 'qbranchmaster' Tabaka
*/

#ifndef PUFFIN_RENDER_SETTINGS_HPP
#define PUFFIN_RENDER_SETTINGS_HPP

#include <memory>

#include "PuffinEngine/AlphaBlend.hpp"
#include "PuffinEngine/DepthTest.hpp"
#include "PuffinEngine/FaceCull.hpp"

namespace puffin {
    class RenderSettings {
    public:
        RenderSettings() {
            depth_test_.reset(new DepthTest());
            face_cull_.reset(new FaceCull());
        }

        AlphaBlendPtr alphaBlend() const {
            return alpha_blend_;
        }

        DepthTestPtr depthTest() const {
            return depth_test_;
        }

        FaceCullPtr faceCull() const {
            return face_cull_;
        }

    private:
        AlphaBlendPtr alpha_blend_;
        DepthTestPtr depth_test_;
        FaceCullPtr face_cull_;
    };

    using RenderSettingsPtr = std::shared_ptr<RenderSettings>;
} // namespace puffin

#endif // PUFFIN_RENDER_SETTINGS_HPP
