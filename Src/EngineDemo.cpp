#include "EngineDemo.hpp"

using namespace puffin;

EngineDemo::EngineDemo() : Core() {
    Logger::instance().enable(true, "puffin_engine.log");
    Logger::instance().enableTimeStamp(true);

    logInfo("EngineDemo::EngineDemo()", "Creating PuffinEngine instance.");

    window()->enableFullscreen(false);
    window()->setCaption("Puffin Engine Demo");

    configuration()->setFrameResolution(1280, 720);
    configuration()->setMsaaSamples(4);
    configuration()->setOpenGLVersion(4, 0);

    createRenderer();
    masterRenderer()->assignRenderingFunction(std::bind(&EngineDemo::render,
        this));
}

void EngineDemo::render() {
     
}