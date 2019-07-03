/*
 * Puffin OpenGL Engine ver. 2.0.1
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
    master_renderer_.reset(new MasterRenderer(window_));
}

void Core::start() {
    master_renderer_->start();
}

void Core::stop() {
    master_renderer_->stop();
}