#include "EngineDemo.hpp"

using namespace puffin;

EngineDemo::EngineDemo() : Core() {
    Logger::instance().enable(true, "puffin_engine.log");
    Logger::instance().enableTimeStamp(true);

    logInfo("EngineDemo::EngineDemo()", "Creating PuffinEngine instance.");

    configuration()->setFrameResolution(1280, 720);
    configuration()->setMsaaSamples(4);
    configuration()->setOpenGLVersion(4, 0);
    configuration()->enableFullscreen(false);

    createRenderer();
    masterRenderer()->assignRenderingFunction(std::bind(&EngineDemo::render,
        this));

    window()->setCaption("Puffin Engine Demo");
    window()->setWindowIcon("Data/Icon.ico");

    createTimers();

    //---
    my_mesh.reset(new Mesh());
    std::vector<GLfloat> data = {-0.5f, -0.5f, 0.0f, 0.0f, 0.5f, 0.0f, 0.5f, -0.5f, 0.0f};
    std::vector<GLfloat> color = {1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f};
    my_mesh->setMeshData(data, 0, 3);
    my_mesh->setMeshData(color, 1, 3);
    auto en = my_mesh->addEntity();
    en->setVerticesCount(3);

    basic_shader.reset(new ShaderProgram("Shaders/basic_vs.glsl",
        "Shaders/basic_fs.glsl"));
    basic_shader->activate();
    //----
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
    //---
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    my_mesh->bind();
    my_mesh->draw(0);
    // TODO: high gpu usage 9%, change to release and check
    //---
    pollKeyboard();
}

void EngineDemo::updateWindowCaption() {
    window()->setCaption("Puffin Engine Demo [FPS: " +
        std::to_string(time()->getFpsRate()) + "]");
}