/*
* Puffin OpenGL Engine ver. 2.0
* Coded by: Sebastian 'qbranchmaster' Tabaka
*/

#ifndef PUFFIN_RENDER_SETTINGS_HPP
#define PUFFIN_RENDER_SETTINGS_HPP

#include "PuffinEngine/Fog.hpp"
#include "PuffinEngine/Lighting.hpp"
#include "PuffinEngine/Postprocess.hpp"

namespace puffin {
    class RenderSettings {
    public:
        RenderSettings() {
            fog_.reset(new Fog());
            lighting_.reset(new Lighting());
            postprocess_.reset(new Postprocess());
        }

        FogPtr fog() const {
            return fog_;
        }

        LightingPtr lighting() const {
            return lighting_;
        }

        PostprocessPtr postprocess() const {
            return postprocess_;
        }

        void setGamma(GLfloat gamma) {
            if (gamma <= 0.0f) {
                logError("RenderSettings::setGamma()", "Invalid value.");
                return;
            }

            gamma_ = gamma;
        }

        GLfloat getGamma() const {
            return gamma_;
        }

    private:
        GLfloat gamma_{2.2f};

        FogPtr fog_;
        LightingPtr lighting_;
        PostprocessPtr postprocess_;
    };

    using RenderSettingsPtr = std::shared_ptr<RenderSettings>;
} // namespace puffin

#endif // PUFFIN_RENDER_SETTINGS_HPP
