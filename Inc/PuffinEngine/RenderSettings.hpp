/*
* Puffin OpenGL Engine ver. 2.0
* Coded by: Sebastian 'qbranchmaster' Tabaka
*/

#ifndef PUFFIN_RENDER_SETTINGS_HPP
#define PUFFIN_RENDER_SETTINGS_HPP

#include "PuffinEngine/Lighting.hpp"
#include "PuffinEngine/Postprocess.hpp"

namespace puffin {
    class RenderSettings {
    public:
        RenderSettings() {
            lighting_.reset(new Lighting());
            postprocess_.reset(new Postprocess());
        }

        LightingPtr lighting() const {
            return lighting_;
        }

        PostprocessPtr postprocess() const {
            return postprocess_;
        }

    private:
        LightingPtr lighting_;
        PostprocessPtr postprocess_;
    };

    using RenderSettingsPtr = std::shared_ptr<RenderSettings>;
} // namespace puffin

#endif // PUFFIN_RENDER_SETTINGS_HPP
