/*
 * Puffin OpenGL Engine ver. 2.0.1
 * Coded by: Sebastian 'qbranchmaster' Tabaka
 * Contact: sebastian.tabaka@outlook.com
 */

#include "PuffinEngine/DefaultGizmoRenderer.hpp"

using namespace puffin;

puffin::DefaultGizmoRenderer::DefaultGizmoRenderer() {
    loadShaders();
    loadTextures();
    createGizmoMesh();
}

void DefaultGizmoRenderer::render(ScenePtr scene) {
    if (!scene) {
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
    setDefaultShaderProgramUniforms(scene->camera());

    gizmo_mesh_->bind();

    renderPointLightsGizmos(scene);
}

void DefaultGizmoRenderer::setGizmosScale(GLushort scale) {
    if (scale < 1) {
        logError("DefaultGizmoRenderer::setGizmosScale()", PUFFIN_MSG_INVALID_VALUE);
        return;
    }

    gizmo_scale_ = scale;
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
    gizmo_mesh_.reset(new Mesh());
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

void DefaultGizmoRenderer::renderPointLightsGizmos(ScenePtr scene) {
    Texture::setTextureSlot(0);
    point_light_texture_->bind();

    for (GLuint i = 0; i < scene->lighting()->getPointLightsCount(); i++) {
        auto point_light = scene->lighting()->getPointLight(i);
        auto model_matrix = prepareGizmoModelMatrix(point_light->getPosition(), scene->camera());
        default_shader_program_->setUniform("model_matrix", model_matrix);

        drawGizmoMesh();
    }
}

void DefaultGizmoRenderer::drawGizmoMesh() {
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void DefaultGizmoRenderer::setDefaultShaderProgramUniforms(CameraPtr camera) {
    default_shader_program_->setUniform("view_matrix", camera->getViewMatrix());
    default_shader_program_->setUniform("projection_matrix", camera->getProjectionMatrix());

    default_shader_program_->setUniform("gizmo_texture", 0);
}

glm::mat4 DefaultGizmoRenderer::prepareGizmoModelMatrix(
    const glm::vec3 &position, CameraPtr camera) {
    auto view_matrix = camera->getViewMatrix();
    glm::mat4 model_matrix(1.0f);
    model_matrix = glm::translate(model_matrix, position);

    model_matrix[0][0] = view_matrix[0][0];
    model_matrix[1][0] = view_matrix[0][1];
    model_matrix[2][0] = view_matrix[0][2];
    model_matrix[0][1] = view_matrix[1][0];
    model_matrix[1][1] = view_matrix[1][1];
    model_matrix[2][1] = view_matrix[1][2];
    model_matrix[0][2] = view_matrix[2][0];
    model_matrix[1][2] = view_matrix[2][1];
    model_matrix[2][2] = view_matrix[2][2];

    model_matrix = glm::scale(model_matrix, glm::vec3(gizmo_scale_, gizmo_scale_, gizmo_scale_));

    return model_matrix;
}