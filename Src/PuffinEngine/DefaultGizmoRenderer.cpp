/*
 * Puffin OpenGL Engine ver. 2.1
 * Coded by: Sebastian 'qbranchmaster' Tabaka
 * Contact: sebastian.tabaka@outlook.com
 */

#include "PuffinEngine/DefaultGizmoRenderer.hpp"

using namespace puffin;

puffin::DefaultGizmoRenderer::DefaultGizmoRenderer(
    RenderSettingsPtr render_settings, CameraPtr camera) {
    if (!render_settings || !camera) {
        throw Exception("DefaultGizmoRenderer::DefaultGizmoRenderer()", PUFFIN_MSG_NULL_OBJECT);
    }

    render_settings_ = render_settings;
    camera_ = camera;

    loadShaders();
    loadTextures();
    createGizmoMesh();
}

void DefaultGizmoRenderer::render(FrameBufferPtr frame_buffer) {
    if (!frame_buffer) {
        logError("DefaultGizmoRenderer::render()", PUFFIN_MSG_NULL_OBJECT);
        return;
	}

	if (!enabled_) {
        return;
    }

    // Prepare rendering process
    FrameBuffer::unbindAll();
    FrameBuffer::setViewportSize(
        InitConfig::instance().getFrameWidth(), InitConfig::instance().getFrameHeight());

    AlphaBlend::instance().enable(true);
    AlphaBlend::instance().setBlendFunction(BlendFunction::Normal);
    DepthTest::instance().enable(true);
    DepthTest::instance().enableDepthMask(false);
    FaceCull::instance().enable(false);
    FaceCull::instance().setCulledSide(CulledSide::Back);

	default_shader_program_->activate();

	gizmo_mesh_->bind();

	for (GLuint i = 0; i < render_settings_->lighting()->getPointLightsCount(); i++) {
        auto point_light = render_settings_->lighting()->getPointLight(i);

        auto view_matrix = camera_->getViewMatrix();
        default_shader_program_->setUniform("view_matrix", view_matrix);
        default_shader_program_->setUniform("projection_matrix", camera_->getProjectionMatrix());

		glm::mat4 model_matrix(1.0f);
        model_matrix = glm::translate(model_matrix, point_light->getPosition());

        model_matrix[0][0] = view_matrix[0][0];
        model_matrix[1][0] = view_matrix[0][1];
        model_matrix[2][0] = view_matrix[0][2];
        model_matrix[0][1] = view_matrix[1][0];
        model_matrix[1][1] = view_matrix[1][1];
        model_matrix[2][1] = view_matrix[1][2];
        model_matrix[0][2] = view_matrix[2][0];
        model_matrix[1][2] = view_matrix[2][1];
        model_matrix[2][2] = view_matrix[2][2];

        default_shader_program_->setUniform("model_matrix", model_matrix);

		default_shader_program_->setUniform("gizmo_texture", 0);

		Texture::setTextureSlot(0);
        point_light_texture_->bind();

		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
}

void DefaultGizmoRenderer::loadShaders() {
    default_shader_program_.reset(new ShaderProgram("gizmo_shader_program"));
    default_shader_program_->loadShaders("Data/Shaders/Gizmo.vert", "Data/Shaders/Gizmo.frag");
}

void DefaultGizmoRenderer::loadTextures() {
    point_light_texture_.reset(new Texture());
    point_light_texture_->loadTexture2D("Data/Icons/PointLight.png");
}

void DefaultGizmoRenderer::createGizmoMesh() {
    gizmo_mesh_.reset(new Mesh("gizmo_mesh"));
	// clang-format off
	std::vector<GLfloat> positions = {
        -0.1f,  0.1f, 0.0f,
        -0.1f, -0.1f, 0.0f,
         0.1f,  0.1f, 0.0f,
         0.1f,  0.1f, 0.0f,
        -0.1f, -0.1f, 0.0f,
         0.1f, -0.1f, 0.0f
    };

    std::vector<GLfloat> texture_coords = {
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 1.0f,
        1.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,
    };
	// clang-format on

    gizmo_mesh_->bind();
    gizmo_mesh_->setMeshData(positions, 0, 3);
    gizmo_mesh_->setMeshData(texture_coords, 1, 2);
}