#include "PuffinEngine/Core.hpp"

using namespace puffin;

Core::Core() {
    configuration_.reset(new Configuration());
    system_.reset(new System());
    time_.reset(new Time());

    mesh_manager_.reset(new MeshManager());

    window_.reset(new Window(configuration_));
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