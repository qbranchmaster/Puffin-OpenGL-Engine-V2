/*
 * Puffin OpenGL Engine ver. 2.1
 * Coded by: Sebastian 'qbranchmaster' Tabaka
 * Contact: sebastian.tabaka@outlook.com
 */

#ifndef PUFFIN_RENDER_SETTINGS_HPP
#define PUFFIN_RENDER_SETTINGS_HPP

#include "PuffinEngine/Fog.hpp"
#include "PuffinEngine/Lighting.hpp"
#include "PuffinEngine/Postprocess.hpp"
#include "PuffinEngine/Wireframe.hpp"

namespace puffin {
    class RenderSettings {
    public:
        RenderSettings() {
            fog_.reset(new Fog());
            lighting_.reset(new Lighting());
            postprocess_.reset(new Postprocess());
            wireframe_.reset(new Wireframe());
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

        WireframePtr wireframe() const {
            return wireframe_;
        }

    private:
        FogPtr fog_{nullptr};
        LightingPtr lighting_{nullptr};
        PostprocessPtr postprocess_{nullptr};
        WireframePtr wireframe_{nullptr};
    };

    using RenderSettingsPtr = std::shared_ptr<RenderSettings>;
} // namespace puffin

#endif // PUFFIN_RENDER_SETTINGS_HPP