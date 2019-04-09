/*
* Puffin OpenGL Engine ver. 2.0
* Coded by: Sebastian 'qbranchmaster' Tabaka
*/

#include "PuffinEngine/MasterRenderer.hpp"

using namespace puffin;

MasterRenderer::MasterRenderer(WindowPtr window, CameraPtr camera,
    RenderSettingsPtr render_settings) {
    if (!window || !camera || !render_settings) {
        throw Exception("MasterRenderer::MasterRenderer()",
            "Not initialized object.");
    }

    camera_ = camera;
    target_window_ = window;
    render_settings_ = render_settings;

    renderers_shared_data_.reset(new RenderersSharedData());

    config_gui_renderer_.reset(new ConfigGuiRenderer(target_window_,
        render_settings_, renderers_shared_data_));

    logInfo("MasterRenderer::MasterRenderer()", "GPU Vendor: " +
        System::instance().getGpuVendor());
    logInfo("MasterRenderer::MasterRenderer()", "GPU Name: " +
        System::instance().getGpuName());
    logInfo("MasterRenderer::MasterRenderer()", "GLSL Version: " +
        System::instance().getGlslVersion());

    for (GLushort i = 0; i < System::instance().getMonitorsCount(); i++) {
        logInfo("MasterRenderer::MasterRenderer()", "Monitor #" +
            std::to_string(i) + " name: " +
            System::instance().getMonitorName(i) + ", size = " +
            std::to_string(System::instance().getMonitorSize(i).first) +
            "x" + std::to_string(System::instance().getMonitorSize(i).second));
    }

    createDefaultFrameBuffer();
}

void MasterRenderer::createDefaultFrameBuffer() {
    auto w = Configuration::instance().getFrameWidth();
    auto h = Configuration::instance().getFrameHeight();

    default_frame_buffer_.reset(new FrameBuffer(w, h));
    default_frame_buffer_multisample_.reset(new FrameBuffer(w, h));

    default_frame_buffer_->addRenderBuffer(false);
    default_frame_buffer_->addTextureBuffer(0, false, true);
    default_frame_buffer_->addTextureBuffer(1, false, true);

    default_frame_buffer_multisample_->addRenderBuffer(true);
    default_frame_buffer_multisample_->addTextureBuffer(0, true, true);
    default_frame_buffer_multisample_->addTextureBuffer(1, true, true);

    if (!default_frame_buffer_->isComplete()) {
        throw Exception("MasterRenderer::createDefaultFrameBuffer()",
            "Error creating default frame buffer.");
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
            default_frame_buffer_multisample_->
                copyFrameBuffer(default_frame_buffer_, 0);
            default_frame_buffer_multisample_->
                copyFrameBuffer(default_frame_buffer_, 1);
            postprocess_renderer_->render(default_frame_buffer_);
        }

		if (rendered_scene) {
			for (GLuint i = 0; i < rendered_scene->getTextesCount(); i++) {
				auto text = rendered_scene->getText(i);
				font_renderer_->render(text);
			}
		}

        if (config_gui_enabled_) {
            config_gui_renderer_->render();
        }

        target_window_->swapBuffers();
        target_window_->pollEvents();

        Time::instance().endDeltaMeasure();
        Time::instance().update();

        checkGlErrors();

        camera_->updateSpeed(Time::instance().getDelta());

        if (target_window_->isClosing()) {
            stop();
        }
    }
}

void MasterRenderer::assignRenderingFunction(
	std::function<ScenePtr()> function) {
    if (!function) {
        logError("MasterRenderer::assignRenderingFunction()", "Null input.");
        return;
    }

    rendering_function_ = function;
}

void MasterRenderer::assignPostprocessRenderer(
    PostprocessRendererPtr renderer) {
    if (!renderer) {
        logError("MasterRenderer::assignPostprocessRenderer()", "Null input.");
        return;
    }

    postprocess_renderer_ = renderer;
}

void MasterRenderer::assignSkyboxRenderer(SkyboxRendererPtr renderer) {
    if (!renderer) {
        logError("MasterRenderer::assignSkyboxRenderer()", "Null input.");
        return;
    }

    skybox_renderer_ = renderer;
}

void MasterRenderer::assignMeshRenderer(MeshRendererPtr renderer) {
    if (!renderer) {
        logError("MasterRenderer::assignMeshRenderer()", "Null input.");
        return;
    }

    mesh_renderer_ = renderer;
    mesh_renderer_->setSharedDataPtr(renderers_shared_data_);
}

void MasterRenderer::assignShadowMapRenderer(ShadowMapRendererPtr renderer) {
    if (!renderer) {
        logError("MasterRenderer::assignShadowMapRenderer()", "Null input.");
        return;
    }

    shadow_map_renderer_ = renderer;
    shadow_map_renderer_->setSharedDataPtr(renderers_shared_data_);
}

void MasterRenderer::assignFontRenderer(FontRendererPtr renderer) {
	if (!renderer) {
		logError("MasterRenderer::assignFontRenderer()", "Null input.");
		return;
	}

	font_renderer_ = renderer;
}

void MasterRenderer::drawScene(ScenePtr scene) {
    if (!scene) {
        logError("MasterRenderer::drawScene()", "Null input.");
    }

    if (skybox_renderer_) {
        auto skybox = scene->getSkybox();
        if (skybox) {
            skybox_renderer_->render(default_frame_buffer_multisample_,
                skybox);
        }
    }

    if (shadow_map_renderer_ && render_settings_->lighting()->
        isShadowMappingEnabled()) {
        for (GLuint i = 0; i < scene->getMeshesCount(); i++) {
            auto mesh = scene->getMesh(i);
            if (mesh) {
                shadow_map_renderer_->render(mesh);
            }
        }
    }

    if (mesh_renderer_) {
        for (GLuint i = 0; i < scene->getMeshesCount(); i++) {
            auto mesh = scene->getMesh(i);
            if (mesh) {
                mesh_renderer_->render(default_frame_buffer_multisample_,
                    mesh);
            }
        }
    }
}

void MasterRenderer::configGuiEnable(GLboolean state) {
    config_gui_enabled_ = state;
}

void MasterRenderer::configGuiEnableDebug(GLboolean state) {
    config_gui_renderer_->enableDebugWindows(state);
}

void MasterRenderer::clearDefaultFrameBuffer() {
    // Before clearing frame buffer enable depth mask to clear it also.
    DepthTest::instance().enableDepthMask(true);

    default_frame_buffer_->bind(FrameBufferBindType::NORMAL);
    FrameBuffer::clear(FrameBufferClearType::DEPTH_AND_COLOR,
        default_frame_buffer_->getClearColor());
    default_frame_buffer_->unbind();

    default_frame_buffer_multisample_->bind(FrameBufferBindType::NORMAL);
    FrameBuffer::clear(FrameBufferClearType::DEPTH_AND_COLOR);
    default_frame_buffer_multisample_->unbind();
}

void MasterRenderer::checkGlErrors() {
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        throw Exception("MasterRenderer::checkGlErrors()",
            "OpenGL error occured.");
    }
}