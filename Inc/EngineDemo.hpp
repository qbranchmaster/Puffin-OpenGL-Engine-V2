#ifndef ENGINE_DEMO_HPP
#define ENGINE_DEMO_HPP

#include "PuffinEngine/Core.hpp"

class EngineDemo : public puffin::Core {
public:
    EngineDemo();

    void render();
};

#endif // ENGINE_DEMO_HPP