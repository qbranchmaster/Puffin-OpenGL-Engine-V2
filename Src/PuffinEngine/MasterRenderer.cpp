/*
 * Puffin OpenGL Engine ver. 2.0.1
 * Coded by: Sebastian 'qbranchmaster' Tabaka
 * Contact: sebastian.tabaka@outlook.com
 */

#include "PuffinEngine/MasterRenderer.hpp"

using namespace puffin;

MasterRenderer::MasterRenderer(WindowPtr window) {
    if (!window) {
        throw Exception("MasterRenderer::MasterRenderer()", PUFFIN_MSG_NULL_OBJECT);
    }

    target_window_ = window;
    postprocess_.reset(new Postprocess());

    logInfo("MasterRenderer::MasterRenderer()", "GPU Vendor: " + System::instance().getGpuVendor());
    logInfo("MasterRenderer::MasterRenderer()", "GPU Name: " + System::instance().getGpuName());
    logInfo(
        "MasterRenderer::MasterRenderer()", "GLSL Version: " + System::instance().getGlslVersion());

    for (GLushort i = 0; i < System::instance().getMonitorsCount(); i++) {
        logInfo("MasterRenderer::MasterRenderer()",
            "Monitor #" + std::to_string(i) + " name: " + System::instance().getMonitorName(i) +
                ", size = " + std::to_string(System::instance().getMonitorSize(i).first) + "x" +
                std::to_string(System::instance().getMonitorSize(i).second));
    }

    printGLParams();

    createRenderers();
    createDefaultFrameBuffer();
}

void MasterRenderer::createRenderers() {
    postprocess_renderer_.reset(new DefaultPostprocessRenderer(postprocess_));
    shadow_map_renderer_.reset(new DefaultShadowMapRenderer());
    skybox_renderer_.reset(new DefaultSkyboxRenderer(postprocess_));
    mesh_renderer_.reset(new DefaultMeshRenderer(
        std::static_pointer_cast<DefaultShadowMapRenderer>(shadow_map_renderer_), postprocess_));
    font_renderer_.reset(new DefaultFontRenderer("DemoData/Fonts/unispace/unispace.ttf"));
    water_renderer_.reset(new DefaultWaterRenderer(postprocess_,
        std::static_pointer_cast<DefaultMeshRenderer>(mesh_renderer_),
        std::static_pointer_cast<DefaultSkyboxRenderer>(skybox_renderer_)));
    gizmo_renderer_.reset(new DefaultGizmoRenderer());
    particle_system_renderer_.reset(new DefaultParticleSystemRenderer());

    gui_renderer_.reset(new DefaultGuiRenderer(target_window_, postprocess_, mesh_renderer_,
        skybox_renderer_, shadow_map_renderer_, water_renderer_, gizmo_renderer_));
    std::static_pointer_cast<DefaultGuiRenderer>(gui_renderer_)
        ->bindQuitFunction(std::bind(&MasterRenderer::stop, this));
    std::static_pointer_cast<DefaultGuiRenderer>(gui_renderer_)
        ->bindSaveCapture(std::bind(
            &MasterRenderer::captureScreen, this, std::placeholders::_1, std::placeholders::_2));
}

void MasterRenderer::createDefaultFrameBuffer() {
    auto w = InitConfig::instance().getFrameWidth();
    auto h = InitConfig::instance().getFrameHeight();

    default_frame_buffer_.reset(new FrameBuffer(w, h));
    default_frame_buffer_->addDepthTextureBuffer(false, true);
    default_frame_buffer_->addTextureBuffer(0, false, true);
    default_frame_buffer_->addTextureBuffer(1, false, true);

    default_frame_buffer_multisample_.reset(new FrameBuffer(w, h));
    default_frame_buffer_multisample_->addTextureBuffer(0, true, true);
    default_frame_buffer_multisample_->addTextureBuffer(1, true, true);
    default_frame_buffer_multisample_->addDepthTextureBuffer(true, true);
}

void MasterRenderer::stop() {
    rendering_enabled_ = false;
}

void MasterRenderer::start() {
    rendering_enabled_ = true;
    while (rendering_enabled_) {
        Time::instance().startDeltaMeasure();

        clearDefaultFrameBuffer();

        ScenePtr rendered_scene = nullptr;
        if (rendering_function_) {
            rendered_scene = rendering_function_();
        }

        if (postprocess_renderer_) {
            default_frame_buffer_multisample_->copyFrameBuffer(default_frame_buffer_, 0, true);
            default_frame_buffer_multisample_->copyFrameBuffer(default_frame_buffer_, 1, false);
            postprocess_renderer_->render(default_frame_buffer_, rendered_scene);
        }

        if (gizmo_renderer_) {
            gizmo_renderer_->render(rendered_scene);
        }

        if (rendered_scene) {
            for (GLuint i = 0; i < rendered_scene->getTextesCount(); i++) {
                auto text = rendered_scene->getText(i);
                font_renderer_->render(text);
            }
        }

        if (capture_screen_flag_) {
            saveFrameToFile();
        }

        if (gui_renderer_) {
            gui_renderer_->render(rendered_scene);
        }

        target_window_->swapBuffers();
        target_window_->pollEvents();

        Time::instance().endDeltaMeasure();
        Time::instance().update();

        checkGlErrors();

        rendered_scene->camera()->updateSpeed(Time::instance().getDelta());

        for (const auto &timer : timers_) {
            timer->update(Time::instance().getDelta());
        }

        if (target_window_->isClosing()) {
            stop();
        }
    }
}

void MasterRenderer::assignRenderingFunction(std::function<ScenePtr()> function) {
    if (!function) {
        logError("MasterRenderer::assignRenderingFunction()", PUFFIN_MSG_NULL_OBJECT);
        return;
    }

    rendering_function_ = function;
}

void MasterRenderer::drawScene(ScenePtr scene) {
    if (!scene) {
        logError("MasterRenderer::drawScene()", PUFFIN_MSG_NULL_OBJECT);
    }

    if (skybox_renderer_) {
        skybox_renderer_->render(default_frame_buffer_multisample_, scene);
    }

    if (shadow_map_renderer_) {
        shadow_map_renderer_->render(scene);
    }

    if (water_renderer_) {
        water_renderer_->render(default_frame_buffer_multisample_, scene);
    }

    if (mesh_renderer_) {
        mesh_renderer_->render(default_frame_buffer_multisample_, scene);
    }

    if (particle_system_renderer_) {
        particle_system_renderer_->render(default_frame_buffer_multisample_, scene);
    }
}

void MasterRenderer::captureScreen(std::string file_name, bool add_timestamp) {
    if (file_name.empty()) {
        logError("MasterRenderer::captureScreen()", PUFFIN_MSG_INVALID_VALUE);
        return;
    }

    capture_screen_flag_ = true;
    capture_file_name_ = file_name;
    capture_add_timestamp_ = add_timestamp;
}

void MasterRenderer::addTimer(TimerPtr timer) {
    if (!timer) {
        logError("MasterRenderer::addTimer()", PUFFIN_MSG_NULL_OBJECT);
        return;
    }

    timers_.push_back(timer);
}

void MasterRenderer::clearDefaultFrameBuffer() {
    // Before clearing frame buffer enable depth mask to clear it also
    DepthTest::instance().enableDepthMask(true);

    default_frame_buffer_->bind(FrameBufferBindType::Normal);
    FrameBuffer::setViewportSize(default_frame_buffer_);
    FrameBuffer::clear(FrameBufferClearType::DepthAndColor, default_frame_buffer_->getClearColor());

    default_frame_buffer_multisample_->bind(FrameBufferBindType::Normal);
    FrameBuffer::setViewportSize(default_frame_buffer_multisample_);
    FrameBuffer::clear(FrameBufferClearType::DepthAndColor);
}

void MasterRenderer::printGLParams() {
    std::map<GLenum, std::string> params = {
        {GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, "GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS"},
        {GL_MAX_CUBE_MAP_TEXTURE_SIZE, "GL_MAX_CUBE_MAP_TEXTURE_SIZE"},
        {GL_MAX_DRAW_BUFFERS, "GL_MAX_DRAW_BUFFERS"},
        {GL_MAX_FRAGMENT_UNIFORM_COMPONENTS, "GL_MAX_FRAGMENT_UNIFORM_COMPONENTS"},
        {GL_MAX_TEXTURE_IMAGE_UNITS, "GL_MAX_TEXTURE_IMAGE_UNITS"},
        {GL_MAX_TEXTURE_SIZE, "GL_MAX_TEXTURE_SIZE"},
        {GL_MAX_VARYING_FLOATS, "GL_MAX_VARYING_FLOATS"},
        {GL_MAX_VERTEX_ATTRIBS, "GL_MAX_VERTEX_ATTRIBS"},
        {GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, "GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS"},
        {GL_MAX_VERTEX_UNIFORM_COMPONENTS, "GL_MAX_VERTEX_UNIFORM_COMPONENTS"},
    };

    logInfo("MasterRenderer::printGLParams()", "GL context params:");
    for (const auto &p : params) {
        GLint value = 0;
        glGetIntegerv(p.first, &value);
        logInfo("MasterRenderer::printGLParams()", p.second + ": " + std::to_string(value));
    }
}

void MasterRenderer::checkGlErrors() {
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        throw Exception("MasterRenderer::checkGlErrors()",
            "OpenGL error occured [Error code = " + std::to_string(error) + "].");
    }
}

void MasterRenderer::saveFrameToFile() {
    auto width = InitConfig::instance().getFrameWidth();
    auto height = InitConfig::instance().getFrameHeight();

    GLubyte *buffer = new GLubyte[width * height * 3];
    glReadPixels(0, 0, width, height, GL_BGR, GL_UNSIGNED_BYTE, buffer);

    FIBITMAP *bitmap = FreeImage_ConvertFromRawBits(
        buffer, width, height, 3 * width, 24, 0xFF0000, 0x00FF00, 0x0000FF, false);

    std::string file_name = capture_file_name_ +
        (capture_add_timestamp_ ? ("_" + Time::instance().getTimeStampNowStr('_')) : "") + ".png";
    if (!FreeImage_Save(FIF_PNG, bitmap, file_name.c_str())) {
        logError("MasterRenderer::saveFrameToFile()", PUFFIN_MSG_FILE_SAVE_ERROR(file_name));
    }

    logInfo("MasterRenderer::saveFrameToFile()", PUFFIN_MSG_FILE_SAVED(file_name));

    delete[] buffer;
    capture_screen_flag_ = false;
}