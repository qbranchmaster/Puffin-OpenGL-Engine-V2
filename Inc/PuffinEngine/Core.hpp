/*
* Puffin OpenGL Engine ver. 2.0
* Coded by: Sebastian 'qbranchmaster' Tabaka
*/

#ifndef PUFFIN_CORE_HPP
#define PUFFIN_CORE_HPP

#include "PuffinEngine/DefaultPostprocessRenderer.hpp"
#include "PuffinEngine/DefaultSkyboxRenderer.hpp"
#include "PuffinEngine/Input.hpp"
#include "PuffinEngine/MasterRenderer.hpp"
#include "PuffinEngine/System.hpp"

namespace puffin {
    class Core {
    public:
        Core();
        ~Core();

        CameraPtr camera() const {
            return camera_;
        }

        InputPtr input() const {
            return input_;
        }

        MasterRendererPtr masterRenderer() const {
            return master_renderer_;
        }

        RenderSettingsPtr renderSettings() const {
            return render_settings_;
        }

        WindowPtr window() const {
            return window_;
        }

        void initialize();
        void start();
        void stop();

    private:
        CameraPtr camera_;
        DefaultPostprocessRendererPtr default_postprocess_renderer_;
        DefaultSkyboxRendererPtr default_skybox_renderer_;
        InputPtr input_;
        MasterRendererPtr master_renderer_;
        RenderSettingsPtr render_settings_;
        WindowPtr window_;
    };
} // namespace puffin

#endif // PUFFIN_CORE_HPP