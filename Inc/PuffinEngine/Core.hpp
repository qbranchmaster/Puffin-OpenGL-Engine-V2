/*
* Puffin OpenGL Engine ver. 2.0
* Coded by: Sebastian 'qbranchmaster' Tabaka
* Contact: sebastian.tabaka@outlook.com
*/

#ifndef PUFFIN_CORE_HPP
#define PUFFIN_CORE_HPP

#include "PuffinEngine/AlphaBlend.hpp"
#include "PuffinEngine/DefaultFontRenderer.hpp"
#include "PuffinEngine/DefaultGuiRenderer.hpp"
#include "PuffinEngine/DefaultMeshRenderer.hpp"
#include "PuffinEngine/DefaultPostprocessRenderer.hpp"
#include "PuffinEngine/DefaultShadowMapRenderer.hpp"
#include "PuffinEngine/DefaultSkyboxRenderer.hpp"
#include "PuffinEngine/DefaultWaterRenderer.hpp"
#include "PuffinEngine/DepthTest.hpp"
#include "PuffinEngine/FaceCull.hpp"
#include "PuffinEngine/Input.hpp"
#include "PuffinEngine/MasterRenderer.hpp"
#include "PuffinEngine/RenderersSharedData.hpp"
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
        void createDefaultRenderers();
        void start();
        void stop();

    private:
        CameraPtr camera_{nullptr};
        InputPtr input_{nullptr};
        WindowPtr window_{nullptr};

        MasterRendererPtr master_renderer_{nullptr};
        RenderersSharedDataPtr renderers_shared_data_{nullptr};
        RenderSettingsPtr render_settings_{nullptr};

        DefaultFontRendererPtr default_font_renderer_{nullptr};
        DefaultGuiRendererPtr default_gui_renderer_{nullptr};
        DefaultMeshRendererPtr default_mesh_renderer_{nullptr};
        DefaultPostprocessRendererPtr default_postprocess_renderer_{nullptr};
        DefaultShadowMapRendererPtr default_shadow_map_renderer_{nullptr};
        DefaultSkyboxRendererPtr default_skybox_renderer_{nullptr};
        DefaultWaterRendererPtr default_water_renderer_{nullptr};
    };
} // namespace puffin

#endif // PUFFIN_CORE_HPP