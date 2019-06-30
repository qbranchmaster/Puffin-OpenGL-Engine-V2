/*
 * Puffin OpenGL Engine ver. 2.1 Demo
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

    puffin::MeshPtr test_mesh_{nullptr};
    puffin::ScenePtr scene_{nullptr};
    puffin::SkyboxPtr skybox_{nullptr};
    puffin::TextPtr text_demo_{nullptr};
    puffin::TexturePtr skybox_texture_{nullptr};
    puffin::WaterTilePtr water_tile_{nullptr};
    puffin::PointLightPtr point_light_{nullptr};

    puffin::TimerPtr fps_caption_timer_{nullptr};
    puffin::TimerPtr skybox_rotate_timer_{nullptr};

    puffin::DefaultGuiRendererPtr gui_renderer_{nullptr};
};

#endif // ENGINE_DEMO_HPP