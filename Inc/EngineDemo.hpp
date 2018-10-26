#ifndef ENGINE_DEMO_HPP
#define ENGINE_DEMO_HPP

#include "PuffinEngine/Core.hpp"
#include "PuffinEngine/Exception.hpp"
#include "PuffinEngine/Logger.hpp"
#include "PuffinEngine/Mesh.hpp"
#include "PuffinEngine/ShaderProgram.hpp"
#include "PuffinEngine/Texture.hpp"
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
    void pollKeyboard();

    puffin::MeshPtr my_mesh;
    puffin::TimerPtr fps_update_timer_;
    puffin::ShaderProgramPtr basic_shader;
    puffin::TexturePtr basic_texture;
};

#endif // ENGINE_DEMO_HPP