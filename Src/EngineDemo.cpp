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

    //---
    GLuint handle = 0;
    glGenVertexArrays(1, &handle);
    glBindVertexArray(handle);
    GLuint vbo = 0;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    std::vector<GLfloat> data = {-1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, -1.0f, 0.0f};
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(GLfloat), data.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);

    TexturePtr tex(new Texture());
    tex->loadTexture2D("Data/test.bmp");

    window()->setWindowIcon("Data/test.bmp");
    //----
}

void EngineDemo::render() {
    //---
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);     

    //glDrawArrays(GL_TRIANGLES, 0, 3);
    //---
}