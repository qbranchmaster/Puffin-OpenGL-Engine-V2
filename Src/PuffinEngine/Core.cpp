#include "PuffinEngine/Core.hpp"

using namespace puffin;

Core::Core() {
    configuration_.reset(new Configuration());
    system_.reset(new System());
}

void Core::createDispaly() {
    display_.reset(new Display(configuration_, system_));
}