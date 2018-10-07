#include "EngineDemo.hpp"

using namespace puffin;

EngineDemo::EngineDemo() : Core() {
    Logger::instance().enable(true, "puffin_engine.log");
    Logger::instance().enableTimeStamp(true);

    logInfo("EngineDemo::EngineDemo()", "Creating PuffinEngine instance.");

    configuration()->setDisplaySize(1280, 720);
    configuration()->setDisplayFullscreen(false);
    configuration()->setWindowCaption("Puffin Engine Demo");

    createDispaly();
}

void EngineDemo::run() {

}