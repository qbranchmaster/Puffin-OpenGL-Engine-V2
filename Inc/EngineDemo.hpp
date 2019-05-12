/*
* Puffin OpenGL Engine ver. 2.0 Demo
* Coded by: Sebastian 'qbranchmaster' Tabaka
* Contact: sebastian.tabaka@outlook.com
*/

#ifndef ENGINE_DEMO_HPP
#define ENGINE_DEMO_HPP

#include "PuffinEngine/Core.hpp"
#include "PuffinEngine/Timer.hpp"

class EngineDemo : public puffin::Core {
public:
    EngineDemo();

private:
    puffin::ScenePtr render();

    void pollKeyboard();
    void pollMouse();
    void moveCamera();
    void rotateCamera();
    void createScene();

    void updateWindowCaption();
    void rotateSkybox();

    puffin::DefaultGuiRendererPtr gui_renderer_;
    puffin::MeshPtr test_mesh_;
    puffin::ScenePtr scene_;
    puffin::SkyboxPtr skybox_;
    puffin::TextPtr text_demo_;
    puffin::TexturePtr skybox_texture_;
    puffin::WaterTilePtr water_tile_;

    puffin::TimerPtr fps_caption_timer_;
    puffin::TimerPtr skybox_rotate_timer_;
};

#endif // ENGINE_DEMO_HPP