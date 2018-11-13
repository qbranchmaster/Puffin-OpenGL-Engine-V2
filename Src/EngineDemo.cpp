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
    window()->setWindowIcon("Data/Icon.ico");

    createTimers();

    // ---
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
    basic_shader_->setUniform("model_matrix", my_mesh_->getModelMatrix());

    basic_texture_.reset(new Texture());
    basic_texture_->loadTexture2D("Data/Brick.jpg");
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
}

void EngineDemo::render() {
    // ---
    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glActiveTexture(GL_TEXTURE0);
    basic_texture_->bind();
    my_mesh_->bind();
    my_mesh_->draw(0);
    // TODO: High CPU usage ~9%, why?
    // ---

    pollKeyboard();
}

void EngineDemo::updateWindowCaption() {
    window()->setCaption("Puffin Engine Demo [FPS: " +
        std::to_string(Time::instance().getFpsRate()) + "]");
}