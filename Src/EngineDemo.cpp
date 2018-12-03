/*
* Puffin OpenGL Engine ver. 2.0 Demo
* Created by: Sebastian 'qbranchmaster' Tabaka
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

    camera()->setPosition(glm::vec3(0.0f, 0.0f, 3.0f));
    camera()->setFov(0.785f);

    Texture::setDefaultTextureFilter(TextureType::Texture2D,
        TextureFilter::TRILINEAR);

    renderSettings()->postprocess()->setEffect(PostprocessEffect::SHARPEN);

    // ---
    scene_.reset(new Scene());

    std::vector<GLfloat> data = {
        -0.5f, 0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        0.5f, 0.5f, 0.0f,
        0.5f, 0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f
    };

    std::vector<GLfloat> coords = {
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 1.0f,
        1.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,
    };

    my_mesh_.reset(new Mesh());
    my_mesh_->setMeshData(data, 0, 3);
    my_mesh_->setMeshData(coords, 1, 2);
    auto entity = my_mesh_->addEntity();
    entity->setVerticesCount(6);
    my_mesh_->translate(glm::vec3(0.5f, 0.0f, 0.0f));

    basic_shader_.reset(new ShaderProgram());
    basic_shader_->loadShaders("Shaders/basic_vs.glsl", "Shaders/basic_fs.glsl");
    basic_shader_->bind();
    basic_shader_->setUniform("color_filter", glm::vec3(1.0f, 1.0f, 1.0f));
    basic_shader_->setUniform("matrices.model_matrix", my_mesh_->getModelMatrix());

    basic_shader_->setUniform("matrices.view_matrix", camera()->getViewMatrix());
    basic_shader_->setUniform("matrices.projection_matrix", camera()->getProjectionMatrix());

    basic_texture_.reset(new Texture());
    basic_texture_->loadTexture2D("DemoData/Brick.jpg");

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
        masterRenderer()->stop();
    }

    moveCamera();
}

void EngineDemo::pollMouse() {
    rotateCamera();
}

void EngineDemo::render() {
    // ---
    auto size = Configuration::instance().getFrameResolution();
    glViewport(0, 0, size.first, size.second);
    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);

    masterRenderer()->drawScene(scene_);

    basic_shader_->bind();
    basic_shader_->setUniform("matrices.view_matrix", camera()->getViewMatrix());
    basic_shader_->setUniform("matrices.projection_matrix", camera()->getProjectionMatrix());

    basic_texture_->bind();
    my_mesh_->bind();
    my_mesh_->draw(0);
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