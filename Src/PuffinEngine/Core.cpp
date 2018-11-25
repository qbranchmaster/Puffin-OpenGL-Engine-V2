/*
* Puffin OpenGL Engine ver. 2.0
* Created by: Sebastian 'qbranchmaster' Tabaka
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

    camera_.reset(new Camera());
    input_.reset(new Input(window_));

    render_settings_.reset(new RenderSettings());

    master_renderer_.reset(new MasterRenderer(window_, camera_));
    skybox_renderer_.reset(new SkyboxRenderer(render_settings_, camera_));
}

void Core::start() const {
    master_renderer_->start();
}

void Core::stop() const {
    master_renderer_->stop();
}