/*
 * Puffin OpenGL Engine ver. 2.0.1 Demo
 * Coded by: Sebastian 'qbranchmaster' Tabaka
 * Contact: sebastian.tabaka@outlook.com
 */

#include "EngineDemo.hpp"

using namespace puffin;

EngineDemo::EngineDemo() : Core() {
    Logger::instance().enable(true, "puffin_engine.log");
    Logger::instance().enableTimeStamp(true);

    InitConfig::instance().setFrameResolution(1280, 720);
    InitConfig::instance().setMsaaSamples(2);
    InitConfig::instance().setOpenGLVersion(4, 0);
    InitConfig::instance().enableFullscreen(false);
    InitConfig::instance().setTargetMonitorIndex(0);

    initialize();

    Texture::setDefaultTextureFilter(TextureType::Texture2D, TextureFilter::Trilinear);

    window()->setCaption("Puffin Engine Demo");
    window()->setWindowIcon("Data/Icons/PuffinBird.png");

    masterRenderer()->assignRenderingFunction(std::bind(&EngineDemo::render, this));

    createScene();

    fps_caption_timer_.reset(new Timer(std::bind(&EngineDemo::updateWindowCaption, this)));
    fps_caption_timer_->start(1000);
    masterRenderer()->addTimer(fps_caption_timer_);

    skybox_rotate_timer_.reset(new Timer(std::bind(&EngineDemo::rotateSkybox, this)));
    skybox_rotate_timer_->start(100);
    masterRenderer()->addTimer(skybox_rotate_timer_);

    gui_renderer_ = std::static_pointer_cast<DefaultGuiRenderer>(masterRenderer()->guiRenderer());
}

void EngineDemo::createScene() {
    scene_.reset(new Scene("car_scene"));
}

void EngineDemo::pollKeyboard() {
    if (gui_renderer_->isCapturingKeyboard()) {
        return;
    }

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
    window()->setCaption(
        "Puffin Engine Demo [FPS: " + std::to_string(Time::instance().getFpsRate()) + "]");
}

void EngineDemo::rotateSkybox() {
    auto skybox = scene_->getActiveSkybox();
    if (!skybox) {
        return;
    }

    static GLfloat rotation_angle = 0.0f;
    skybox->setRotation(glm::vec3(0.0f, rotation_angle, 0.0f));
    rotation_angle += 0.0005f;
}

void EngineDemo::moveCamera() {
    auto camera_speed = scene_->camera()->getMoveSpeed();
    if (input()->keyPressed(Key::LeftShift, true)) {
        camera_speed *= 2.0f;
    }

    if (input()->keyPressed(Key::A, true)) {
        scene_->camera()->move(CameraMoveDirection::Left, camera_speed);
    }

    if (input()->keyPressed(Key::D, true)) {
        scene_->camera()->move(CameraMoveDirection::Right, camera_speed);
    }

    if (input()->keyPressed(Key::W, true)) {
        scene_->camera()->move(CameraMoveDirection::Forward, camera_speed);
    }

    if (input()->keyPressed(Key::S, true)) {
        scene_->camera()->move(CameraMoveDirection::Backward, camera_speed);
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

        scene_->camera()->rotate((GLfloat)(y_diff * mouse_speed), (GLfloat)(x_diff * mouse_speed));

        // Clamp vertical angle from 0 to 90 degrees
        GLfloat v_angle = scene_->camera()->getVerticalAngle();
        if (v_angle > glm::half_pi<GLfloat>()) {
            v_angle = glm::half_pi<GLfloat>();
        }
        else if (v_angle < -glm::half_pi<GLfloat>()) {
            v_angle = -glm::half_pi<GLfloat>();
        }

        scene_->camera()->setRotation(scene_->camera()->getHorizontalAngle(), v_angle);

        cursor_x = cur_x;
        cursor_y = cur_y;
    }
    else {
        input()->getCursorPosition(cursor_x, cursor_y);
    }
}