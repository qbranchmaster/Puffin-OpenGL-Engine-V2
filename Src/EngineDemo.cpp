/*
* Puffin OpenGL Engine ver. 2.0 Demo
* Coded by: Sebastian 'qbranchmaster' Tabaka
*/

#include "EngineDemo.hpp"

using namespace puffin;

EngineDemo::EngineDemo() : Core() {
    Logger::instance().enable(true, "puffin_engine.log");
    Logger::instance().enableTimeStamp(true);

    Configuration::instance().setFrameResolution(1280, 720);
    Configuration::instance().setMsaaSamples(4);
    Configuration::instance().setOpenGLVersion(4, 0);
    Configuration::instance().enableFullscreen(false);
    Configuration::instance().setTargetMonitorIndex(0);

    initialize();
    masterRenderer()->assignRenderingFunction(std::bind(&EngineDemo::render,
        this));

    window()->setCaption("Puffin Engine Demo");
    window()->setWindowIcon("DemoData/Icon.ico");

    createTimers();

    camera()->setPosition(glm::vec3(0.0f, 2.0f, 8.0f));
    camera()->setFov(0.85f);

    Texture::setDefaultTextureFilter(TextureType::Texture2D,
        TextureFilter::TRILINEAR);

    renderSettings()->postprocess()->setEffect(PostprocessEffect::NONE);
    renderSettings()->postprocess()->setTintColor(glm::vec3(0.6f, 0.2f, 0.2f));
    renderSettings()->postprocess()->enableGlowBloom(true);

    renderSettings()->lighting()->enable(true);
    renderSettings()->lighting()->setSkyboxLightingColor(
        glm::vec3(0.9f, 0.9f, 0.9f));

    renderSettings()->lighting()->directionalLight()->enable(true);
    renderSettings()->lighting()->enableBlinnPhong(true);
    renderSettings()->lighting()->directionalLight()->setDirection(
        glm::vec3(0.5f, -0.5f, -0.5f));
    renderSettings()->lighting()->directionalLight()->setAmbientColor(
        glm::vec3(0.6f, 0.6f, 0.6f));
    renderSettings()->lighting()->directionalLight()->setDiffuseColor(
        glm::vec3(0.8f, 0.8f, 0.8f));
    renderSettings()->lighting()->directionalLight()->setSpecularColor(
        glm::vec3(5.5f, 5.5f, 5.5f));

    createScene();
}

EngineDemo::~EngineDemo() {
    stopTimers();
}

void EngineDemo::createTimers() {
    fps_update_timer_.reset(new Timer(std::bind(
        &EngineDemo::updateWindowCaption, this)));
    fps_update_timer_->start(1000);
}

void EngineDemo::stopTimers() {
    fps_update_timer_->stop();
}

void EngineDemo::createScene() {
    scene_.reset(new Scene());
    skybox_.reset(new Skybox());

    skybox_texture_.reset(new Texture());
    skybox_texture_->loadTextureCube({
        "DemoData/Skybox/right.jpg",
        "DemoData/Skybox/left.jpg",
        "DemoData/Skybox/up.jpg",
        "DemoData/Skybox/down.jpg",
        "DemoData/Skybox/back.jpg",
        "DemoData/Skybox/front.jpg"});

    skybox_->setTexture(skybox_texture_);

    test_mesh_.reset(new Mesh());
    test_mesh_->loadFromFile("DemoData/Models/pony-cartoon/Pony_cartoon.obj");
    test_mesh_->setScale(glm::vec3(0.01f, 0.01f, 0.01f));

    scene_->setSkybox(skybox_);
    scene_->addMesh(test_mesh_);
}

void EngineDemo::pollKeyboard() {
    if (input()->keyPressed(Key::ESCAPE, false)) {
        stop();
    }

    if (input()->keyPressed(Key::F1, false)) {
        static GLboolean enabled = false;
        enabled = !enabled;
        masterRenderer()->configGuiEnabled(enabled);
    }

    moveCamera();
}

void EngineDemo::pollMouse() {
    rotateCamera();
}

void EngineDemo::render() {
    masterRenderer()->drawScene(scene_);

    pollKeyboard();
    pollMouse();
}

void EngineDemo::updateWindowCaption() {
    window()->setCaption("Puffin Engine Demo [FPS: " +
        std::to_string(Time::instance().getFpsRate()) + "]");
}

void EngineDemo::moveCamera() {
    if (input()->keyPressed(Key::LEFT_SHIFT, true)) {
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
    static GLfloat cursor_x = 0.0f;
    static GLfloat cursor_y = 0.0f;

    constexpr GLfloat mouse_speed = 0.002f;

    if (input()->mouseKeyPressed(MouseButton::LEFT)) {
        GLfloat cur_x = 0.0f;
        GLfloat cur_y = 0.0f;
        input()->getCursorPosition(cur_x, cur_y);

        GLfloat x_diff = cur_x - cursor_x;
        GLfloat y_diff = cur_y - cursor_y;

        camera()->rotate(y_diff * mouse_speed, x_diff * mouse_speed);

        // Clamp vertical angle from 0 to 90 degrees.
        GLfloat v_angle = camera()->getVerticalAngle();
        if (v_angle > glm::half_pi<GLfloat>())
            v_angle = glm::half_pi<GLfloat>();
        else if (v_angle < -glm::half_pi<GLfloat>())
            v_angle = -glm::half_pi<GLfloat>();

        camera()->setRotation(camera()->getHorizontalAngle(),
            v_angle);

        cursor_x = cur_x;
        cursor_y = cur_y;
    }
    else {
        input()->getCursorPosition(cursor_x, cursor_y);
    }
}