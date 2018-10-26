#ifndef ENGINE_DEMO_HPP
#define ENGINE_DEMO_HPP

#include "PuffinEngine/Core.hpp"
#include "PuffinEngine/Exception.hpp"
#include "PuffinEngine/Logger.hpp"
#include "PuffinEngine/Mesh.hpp"
#include "PuffinEngine/ShaderProgram.hpp"
#include "PuffinEngine/Timer.h"

class EngineDemo : public puffin::Core {
public:
    EngineDemo();
    ~EngineDemo();

    void render();

private:
    void updateWindowCaption();
    void createTimers();
    void stopTimers();

    puffin::MeshPtr my_mesh;
    puffin::TimerPtr fps_update_timer_;
    puffin::ShaderProgramPtr basic_shader;
};

#endif // ENGINE_DEMO_HPP