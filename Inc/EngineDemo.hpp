/*
 * Puffin OpenGL Engine ver. 2.0.1 Demo
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

    puffin::ScenePtr scene_{nullptr};

    puffin::TimerPtr fps_caption_timer_{nullptr};
    puffin::TimerPtr skybox_rotate_timer_{nullptr};

    puffin::DefaultGuiRendererPtr gui_renderer_{nullptr};
};

#endif // ENGINE_DEMO_HPP