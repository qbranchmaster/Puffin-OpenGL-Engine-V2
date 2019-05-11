/*
* Puffin OpenGL Engine ver. 2.0 Demo
* Coded by: Sebastian 'qbranchmaster' Tabaka
* Contact: sebastian.tabaka@outlook.com
*/

#include "EngineDemo.hpp"

using namespace puffin;

EngineDemo::EngineDemo() : Core() {
    Logger::instance().enable(true, "puffin_engine.log");
    Logger::instance().enableTimeStamp(true);

    Configuration::instance().setFrameResolution(1280, 720);
    Configuration::instance().setMsaaSamples(2);
    Configuration::instance().setOpenGLVersion(4, 0);
    Configuration::instance().enableFullscreen(false);
    Configuration::instance().setTargetMonitorIndex(0);

    initialize();
    masterRenderer()->assignRenderingFunction(std::bind(&EngineDemo::render, this));

    window()->setCaption("Puffin Engine Demo");
    window()->setWindowIcon("Data/Icon.ico");

    camera()->setPosition(glm::vec3(0.0f, 2.0f, 8.0f));
    camera()->setProjection(1.05f, Configuration::instance().getFrameAspect(), 0.1f, 15.0f);
    camera()->setFov(0.85f);

    Texture::setDefaultTextureFilter(TextureType::Texture2D, TextureFilter::TRILINEAR);

    renderSettings()->postprocess()->setEffect(PostprocessEffect::None);
    renderSettings()->postprocess()->setTintColor(glm::vec3(0.6f, 0.2f, 0.2f));
    renderSettings()->postprocess()->enableGlowBloom(false);

    renderSettings()->lighting()->enable(true);
    renderSettings()->lighting()->setSkyboxLightingColor(glm::vec3(0.9f, 0.9f, 0.9f));

    renderSettings()->lighting()->enableShadowMapping(true);
    renderSettings()->lighting()->setDirectionalLightShadowMapSize(1024);
    renderSettings()->lighting()->setShadowDistance(20.0f);

    renderSettings()->lighting()->directionalLight()->enable(true);
    renderSettings()->lighting()->enableBlinnPhong(true);
    renderSettings()->lighting()->directionalLight()->setDirection(
        glm::vec3(0.5f, -0.5f, -0.5f));
    renderSettings()->lighting()->directionalLight()->setAmbientColor(
        glm::vec3(0.1f, 0.1f, 0.1f));
    renderSettings()->lighting()->directionalLight()->setDiffuseColor(
        glm::vec3(0.8f, 0.8f, 0.8f));
    renderSettings()->lighting()->directionalLight()->setSpecularColor(
        glm::vec3(5.5f, 5.5f, 5.5f));

    createScene();
    createDefaultRenderers();

    fps_caption_timer_.reset(new Timer(std::bind(&EngineDemo::updateWindowCaption, this)));
    fps_caption_timer_->start(1000);
    masterRenderer()->addTimer(fps_caption_timer_);

    gui_renderer_ = std::static_pointer_cast<DefaultGuiRenderer>(masterRenderer()->guiRenderer());
}

void EngineDemo::createScene() {
    scene_.reset(new Scene());
    skybox_.reset(new Skybox());

    skybox_texture_.reset(new Texture());
    skybox_texture_->loadTextureCube({
        "Demo/Skybox/right.jpg",
        "Demo/Skybox/left.jpg",
        "Demo/Skybox/up.jpg",
        "Demo/Skybox/down.jpg",
        "Demo/Skybox/back.jpg",
        "Demo/Skybox/front.jpg"});

    skybox_->setTexture(skybox_texture_);

    test_mesh_.reset(new Mesh());
    test_mesh_->loadFromFile("Demo/Models/pony-cartoon/Pony_cartoon.obj");
    test_mesh_->setScale(glm::vec3(0.01f, 0.01f, 0.01f));

    scene_->setSkybox(skybox_);
    scene_->addMesh(test_mesh_);

    text_demo_.reset(new Text(L"Puffin Engine Demo", glm::uvec2(10, 710), 48));
    text_demo_->setFontColor(glm::vec3(1.0f, 0.0f, 0.0f));
    text_demo_->setOutlineColor(glm::vec3(1.0f, 1.0f, 1.0f));
    text_demo_->setOutlineSize(2);
    text_demo_->setHorizontalSpacing(2);
    text_demo_->setFont("Data/Fonts/unispace/unispace.ttf");
    //scene_->addText(text_demo_);

    water_tile_.reset(new WaterTile());
    water_tile_->setPosition(glm::vec3(0.0f, 0.2f, 0.0f));
    water_tile_->setScale(glm::vec3(5.0f, 5.0f, 5.0f));
    scene_->addWaterTile(water_tile_);
}

void EngineDemo::pollKeyboard() {
    if (input()->keyPressed(Key::Escape, false)) {
        stop();
    }

    if (input()->keyPressed(Key::F12, false) && input()->keyPressed(Key::LeftShift)) {
        static GLboolean enabled = true;
        enabled = !enabled;
        gui_renderer_->enable(enabled);
    }

    moveCamera();
}

void EngineDemo::pollMouse() {
    if (!gui_renderer_->isCapturingMouse()) {
        rotateCamera();
    }
}

ScenePtr EngineDemo::render() {
    masterRenderer()->drawScene(scene_);

    pollKeyboard();
    pollMouse();

    return scene_;
}

void EngineDemo::updateWindowCaption() {
    window()->setCaption("Puffin Engine Demo [FPS: " + std::to_string(
        Time::instance().getFpsRate()) + "]");
}

void EngineDemo::moveCamera() {
    if (input()->keyPressed(Key::LeftShift, true)) {
        camera()->setMoveSpeed(2.0f);
    }
    else {
        camera()->setMoveSpeed(1.0f);
    }

    if (input()->keyPressed(Key::A, true)) {
        camera()->move(CameraMoveDirection::LEFT);
    }

    if (input()->keyPressed(Key::D, true)) {
        camera()->move(CameraMoveDirection::RIGHT);
    }

    if (input()->keyPressed(Key::W, true)) {
        camera()->move(CameraMoveDirection::FORWARD);
    }

    if (input()->keyPressed(Key::S, true)) {
        camera()->move(CameraMoveDirection::BACKWARD);
    }
}

void EngineDemo::rotateCamera() {
    static GLdouble cursor_x = 0.0;
    static GLdouble cursor_y = 0.0;

    constexpr GLdouble mouse_speed = 0.002;

    if (input()->mouseKeyPressed(MouseButton::Left)) {
        GLdouble cur_x = 0.0;
        GLdouble cur_y = 0.0;
        input()->getCursorPosition(cur_x, cur_y);

        GLdouble x_diff = cur_x - cursor_x;
        GLdouble y_diff = cur_y - cursor_y;

        camera()->rotate((GLfloat)(y_diff * mouse_speed), (GLfloat)(x_diff * mouse_speed));

        // Clamp vertical angle from 0 to 90 degrees.
        GLfloat v_angle = camera()->getVerticalAngle();
        if (v_angle > glm::half_pi<GLfloat>()) {
            v_angle = glm::half_pi<GLfloat>();
        }
        else if (v_angle < -glm::half_pi<GLfloat>()) {
            v_angle = -glm::half_pi<GLfloat>();
        }

        camera()->setRotation(camera()->getHorizontalAngle(), v_angle);

        cursor_x = cur_x;
        cursor_y = cur_y;
    }
    else {
        input()->getCursorPosition(cursor_x, cursor_y);
    }
}