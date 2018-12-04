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

    camera_.reset(new Camera());
    input_.reset(new Input(window_));

    render_settings_.reset(new RenderSettings());

    master_renderer_.reset(new MasterRenderer(window_, camera_,
        render_settings_));

    default_postprocess_renderer_.reset(new DefaultPostprocessRenderer());

    master_renderer_->assingPostprocessRenderer(std::static_pointer_cast<PostprocessRenderer>(default_postprocess_renderer_));
}

void Core::start() const {
    master_renderer_->start();
}

void Core::stop() const {
    master_renderer_->stop();
}