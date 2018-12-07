/*
* Puffin OpenGL Engine ver. 2.0 Demo
* Created by: Sebastian 'qbranchmaster' Tabaka
*/

#include "EngineDemo.hpp"

#include "PuffinEngine/PostprocessRenderer.hpp"

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

    camera()->setPosition(glm::vec3(0.0f, 0.0f, 3.0f));
    camera()->setFov(0.85f);

    Texture::setDefaultTextureFilter(TextureType::Texture2D,
        TextureFilter::TRILINEAR);

    renderSettings()->postprocess()->setEffect(PostprocessEffect::NONE);
    renderSettings()->postprocess()->setTintColor(glm::vec3(0.6f, 0.2f, 0.2f));

    // ---
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
    //skybox_->translate(glm::vec3(3, 3, 3));

    //skyboxRenderer()->setFilterColor(glm::vec3(1.0f, 1.0f, 1.0f));

    scene_->setSkybox(skybox_);
    // ----
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

void EngineDemo::pollKeyboard() {
    if (input()->keyPressed(Key::ESCAPE, false)) {
        stop();
    }

    moveCamera();
}

void EngineDemo::pollMouse() {
    rotateCamera();
}

void EngineDemo::render() {
    // ---
    /*glViewport(0, 0, Configuration::instance().getFrameWidth(),
        Configuration::instance().getFrameHeight());*/
    /*glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);*/

    /*glActiveTexture(GL_TEXTURE0);

    masterRenderer()->drawScene(scene_);

    basic_shader_->activate();
    basic_shader_->setUniform("matrices.view_matrix", camera()->getViewMatrix());
    basic_shader_->setUniform("matrices.projection_matrix", camera()->getProjectionMatrix());

    basic_texture_->bind();
    my_mesh_->bind();
    my_mesh_->draw(0);*/
    masterRenderer()->drawScene(scene_);
    // ---

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