/*
* Puffin OpenGL Engine
* Created by: Sebastian 'qbranchmaster' Tabaka
*/

#include "PuffinEngine/Core.hpp"

using namespace puffin;

Core::Core() {
    window_.reset(new Window());
    input_.reset(new Input(window_));
}

void Core::createRenderer() {
    master_renderer_.reset(new MasterRenderer(window_));
}

void Core::start() const {
    master_renderer_->start();
}

void Core::stop() const {
    master_renderer_->stop();
}