/*
* Puffin OpenGL Engine ver. 2.0
* Coded by: Sebastian 'qbranchmaster' Tabaka
*/

#include "PuffinEngine/Core.hpp"

#include <memory>

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

    camera_.reset(new Camera());
    input_.reset(new Input(window_));

    render_settings_.reset(new RenderSettings());
    master_renderer_.reset(new MasterRenderer(window_, camera_,
        render_settings_));

    default_postprocess_renderer_.reset(new DefaultPostprocessRenderer(
        render_settings_));
    default_skybox_renderer_.reset(new DefaultSkyboxRenderer(render_settings_,
        camera_));
    default_mesh_renderer_.reset(new DefaultMeshRenderer(render_settings_,
        camera_));

    master_renderer_->assignPostprocessRenderer(std::static_pointer_cast
        <PostprocessRenderer>(default_postprocess_renderer_));
    master_renderer_->assignSkyboxRenderer(std::static_pointer_cast
        <SkyboxRenderer>(default_skybox_renderer_));
    master_renderer_->assignMeshRenderer(std::static_pointer_cast
        <MeshRenderer>(default_mesh_renderer_));
}

void Core::start() {
    master_renderer_->start();
}

void Core::stop() {
    master_renderer_->stop();
}