#include "PuffinEngine/Core.hpp"

using namespace puffin;

Core::Core() {
    system_.reset(new System());
    time_.reset(new Time());

    configuration_.reset(new Configuration(system_));
    window_.reset(new Window(configuration_));
    input_.reset(new Input(window_));
}

void Core::createRenderer() {
    master_renderer_.reset(new MasterRenderer(window_, time_, system_));
}

void Core::start() const {
    master_renderer_->start();
}

void Core::stop() const {
    master_renderer_->stop();
}