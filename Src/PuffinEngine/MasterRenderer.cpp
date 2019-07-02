/*
 * Puffin OpenGL Engine ver. 2.1
 * Coded by: Sebastian 'qbranchmaster' Tabaka
 * Contact: sebastian.tabaka@outlook.com
 */

#include "PuffinEngine/MasterRenderer.hpp"

using namespace puffin;

MasterRenderer::MasterRenderer(
    WindowPtr window) {
    if (!window) {
        throw Exception("MasterRenderer::MasterRenderer()", "Not initialized object.");
    }

    target_window_ = window;

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

    createDefaultFrameBuffer();
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

    if (!default_frame_buffer_->isComplete()) {
        throw Exception(
            "MasterRenderer::createDefaultFrameBuffer()", "Error creating default frame buffer.");
    }

    default_frame_buffer_multisample_->unbind();
    default_frame_buffer_->unbind();
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
            gizmo_renderer_->render(default_frame_buffer_, rendered_scene);
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

void MasterRenderer::assignPostprocessRenderer(PostprocessRendererPtr renderer) {
    if (!renderer) {
        logError("MasterRenderer::assignPostprocessRenderer()", PUFFIN_MSG_NULL_OBJECT);
        return;
    }

    postprocess_renderer_ = renderer;
}

void MasterRenderer::assignSkyboxRenderer(SkyboxRendererPtr renderer) {
    if (!renderer) {
        logError("MasterRenderer::assignSkyboxRenderer()", PUFFIN_MSG_NULL_OBJECT);
        return;
    }

    skybox_renderer_ = renderer;
}

void MasterRenderer::assignMeshRenderer(MeshRendererPtr renderer) {
    if (!renderer) {
        logError("MasterRenderer::assignMeshRenderer()", PUFFIN_MSG_NULL_OBJECT);
        return;
    }

    mesh_renderer_ = renderer;
}

void MasterRenderer::assignShadowMapRenderer(ShadowMapRendererPtr renderer) {
    if (!renderer) {
        logError("MasterRenderer::assignShadowMapRenderer()", PUFFIN_MSG_NULL_OBJECT);
        return;
    }

    shadow_map_renderer_ = renderer;
}

void MasterRenderer::assignFontRenderer(FontRendererPtr renderer) {
    if (!renderer) {
        logError("MasterRenderer::assignFontRenderer()", PUFFIN_MSG_NULL_OBJECT);
        return;
    }

    font_renderer_ = renderer;
}

void MasterRenderer::assignGuiRenderer(GuiRendererPtr renderer) {
    if (!renderer) {
        logError("MasterRenderer::assignGuiRenderer()", PUFFIN_MSG_NULL_OBJECT);
        return;
    }

    gui_renderer_ = renderer;
}

void MasterRenderer::assignWaterRenderer(WaterRendererPtr renderer) {
    if (!renderer) {
        logError("MasterRenderer::assignWaterRenderer()", PUFFIN_MSG_NULL_OBJECT);
        return;
    }

    water_renderer_ = renderer;
}

void MasterRenderer::assignGizmoRenderer(GizmoRendererPtr renderer) {
    if (!renderer) {
        logError("MasterRenderer::assignGizmoRenderer()", PUFFIN_MSG_NULL_OBJECT);
        return;
    }

    gizmo_renderer_ = renderer;
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
}

void MasterRenderer::captureScreen(std::string file_name, GLboolean add_timestamp) {
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
    default_frame_buffer_->unbind();

    default_frame_buffer_multisample_->bind(FrameBufferBindType::Normal);
    FrameBuffer::setViewportSize(default_frame_buffer_multisample_);
    FrameBuffer::clear(FrameBufferClearType::DepthAndColor);
    default_frame_buffer_multisample_->unbind();
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