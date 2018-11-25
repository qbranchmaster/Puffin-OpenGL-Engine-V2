/*
* Puffin OpenGL Engine ver. 2.0 Demo
* Created by: Sebastian 'qbranchmaster' Tabaka
*/

#ifndef ENGINE_DEMO_HPP
#define ENGINE_DEMO_HPP

#include "PuffinEngine/Configuration.hpp"
#include "PuffinEngine/Core.hpp"
#include "PuffinEngine/Exception.hpp"
#include "PuffinEngine/Logger.hpp"
#include "PuffinEngine/Mesh.hpp"
#include "PuffinEngine/ShaderProgram.hpp"
#include "PuffinEngine/Skybox.hpp"
#include "PuffinEngine/Texture.hpp"
#include "PuffinEngine/Time.hpp"
#include "PuffinEngine/Timer.hpp"

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
    void pollMouse();
    void moveCamera();
    void rotateCamera();

    puffin::TimerPtr fps_update_timer_;

    // ---
    puffin::MeshPtr my_mesh_;
    puffin::SkyboxPtr skybox_;
    puffin::ShaderProgramPtr basic_shader_;
    puffin::TexturePtr basic_texture_;
    puffin::TexturePtr skybox_texture_;
    // ---
};

#endif // ENGINE_DEMO_HPP