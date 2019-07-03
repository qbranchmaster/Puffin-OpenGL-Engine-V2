/*
 * Puffin OpenGL Engine ver. 2.1
 * Coded by: Sebastian 'qbranchmaster' Tabaka
 * Contact: sebastian.tabaka@outlook.com
 */

#include "PuffinEngine/Core.hpp"

using namespace puffin;

Core::Core() {
    System::instance().initGlfw();
}

Core::~Core() {
    System::instance().terminateGlfw();
}

void Core::initialize() {
    window_.reset(new Window());
    System::instance().initGl();

    AlphaBlend::instance().fetchState();
    DepthTest::instance().fetchState();
    FaceCull::instance().fetchState();

    input_.reset(new Input(window_));
    postprocess_.reset(new Postprocess());

    master_renderer_.reset(new MasterRenderer(window_));
}

void Core::createDefaultRenderers() {
    default_postprocess_renderer_.reset(new DefaultPostprocessRenderer(postprocess_));
    default_shadow_map_renderer_.reset(new DefaultShadowMapRenderer());
    default_skybox_renderer_.reset(new DefaultSkyboxRenderer(postprocess_));
    default_mesh_renderer_.reset(
        new DefaultMeshRenderer(default_shadow_map_renderer_, postprocess_));
    default_font_renderer_.reset(new DefaultFontRenderer("DemoData/Fonts/unispace/unispace.ttf"));
    default_gui_renderer_.reset(
        new DefaultGuiRenderer(window_, master_renderer_, postprocess_));
    default_water_renderer_.reset(new DefaultWaterRenderer(
        default_mesh_renderer_, default_skybox_renderer_, postprocess_));
    default_gizmo_renderer_.reset(new DefaultGizmoRenderer());

    master_renderer_->assignPostprocessRenderer(
        std::static_pointer_cast<PostprocessRenderer>(default_postprocess_renderer_));
    master_renderer_->assignSkyboxRenderer(
        std::static_pointer_cast<SkyboxRenderer>(default_skybox_renderer_));
    master_renderer_->assignMeshRenderer(
        std::static_pointer_cast<MeshRenderer>(default_mesh_renderer_));
    master_renderer_->assignShadowMapRenderer(
        std::static_pointer_cast<ShadowMapRenderer>(default_shadow_map_renderer_));
    master_renderer_->assignFontRenderer(
        std::static_pointer_cast<FontRenderer>(default_font_renderer_));
    master_renderer_->assignGuiRenderer(
        std::static_pointer_cast<GuiRenderer>(default_gui_renderer_));
    master_renderer_->assignWaterRenderer(
        std::static_pointer_cast<WaterRenderer>(default_water_renderer_));
    master_renderer_->assignGizmoRenderer(
        std::static_pointer_cast<GizmoRenderer>(default_gizmo_renderer_));
}

void Core::start() {
    master_renderer_->start();
}

void Core::stop() {
    master_renderer_->stop();
}