/*
* Puffin OpenGL Engine ver. 2.0 Demo
* Coded by: Sebastian 'qbranchmaster' Tabaka
*/

#ifndef ENGINE_DEMO_HPP
#define ENGINE_DEMO_HPP

#include "PuffinEngine/Core.hpp"

class EngineDemo : public puffin::Core {
public:
    EngineDemo();
    ~EngineDemo();

	puffin::ScenePtr render();

private:
    void updateWindowCaption();
    void pollKeyboard();
    void pollMouse();
    void moveCamera();
    void rotateCamera();

    void createScene();

    puffin::DefaultGuiRendererPtr gui_renderer_;
    puffin::MeshPtr test_mesh_;
    puffin::ScenePtr scene_;
    puffin::SkyboxPtr skybox_;
	puffin::TextPtr text_demo_;
    puffin::TexturePtr skybox_texture_;
};

#endif // ENGINE_DEMO_HPP