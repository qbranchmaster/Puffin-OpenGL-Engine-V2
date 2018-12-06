/*
* Puffin OpenGL Engine ver. 2.0
* Coded by: Sebastian 'qbranchmaster' Tabaka
*/

#include "PuffinEngine/Mesh.hpp"

using namespace puffin;

Mesh::Mesh() {
    glGenVertexArrays(1, &handle_);
}

Mesh::~Mesh() {
    for (auto &buffer : data_buffers_) {
        if (buffer.second != 0)
            glDeleteBuffers(1, &buffer.second);
    }

    if (handle_) {
        glDeleteVertexArrays(1, &handle_);
    }
}

void Mesh::setScale(const glm::vec3 &scale) {
    scale_ = scale;
    scale_matrix_ = glm::scale(glm::mat4(1.0f), scale_);
    model_matrix_changed_ = true;
}

void Mesh::translate(const glm::vec3 &translation) {
    translation_matrix_ = glm::translate(translation_matrix_,
        translation);
    position_ += translation;
    model_matrix_changed_ = true;
}

void Mesh::setPosition(const glm::vec3 &position) {
    translation_matrix_ = glm::mat4(1.0f);
    position_ = glm::vec3(0.0f, 0.0f, 0.0f);
    translate(position);
}

void Mesh::zeroTranslation() {
    translation_matrix_ = glm::mat4(1.0f);
    model_matrix_changed_ = true;
}

void Mesh::rotate(GLfloat angle, const glm::vec3 &axis) {
    rotation_matrix_ = glm::rotate(rotation_matrix_, angle, axis);
    model_matrix_changed_ = true;
}

void Mesh::setRotationAngle(GLfloat angle, const glm::vec3 &axis) {
    rotation_matrix_ = glm::mat4(1.0f);
    rotate(angle, axis);
}

void Mesh::zeroRotation() {
    rotation_matrix_ = glm::mat4(1.0f);
    model_matrix_changed_ = true;
}

MeshEntityPtr Mesh::addEntity() {
    MeshEntityPtr entity(new MeshEntity());
    entities_.push_back(entity);
    return entity;
}

MeshEntityPtr Mesh::getEntity(GLuint index) const {
    if (index >= entities_.size()) {
        logError("Mesh::getEntity()", "Invalid value.");
        return MeshEntityPtr();
    }

    return entities_[index];
}

GLboolean Mesh::isBound() const {
    if (StateMachine::instance().bound_mesh_ == handle_) {
        return true;
    }

    return false;
}

void Mesh::setMeshData(std::vector<GLfloat> data, GLuint index,
    GLuint vertex_size, GLboolean dynamic_draw, GLboolean is_indices) {
    if (!isBound()) {
        logError("Mesh::setMeshData()", "Mesh is not bound.");
        return;
    }

    if (data_buffers_[index] == 0) {
        glGenBuffers(1, &data_buffers_[index]);
    }

    glBindBuffer(is_indices == 1 ? GL_ELEMENT_ARRAY_BUFFER : GL_ARRAY_BUFFER,
        data_buffers_[index]);
    glBufferData(is_indices == 1 ? GL_ELEMENT_ARRAY_BUFFER : GL_ARRAY_BUFFER,
        data.size() * sizeof(GLfloat), data.data(), dynamic_draw ?
        GL_DYNAMIC_DRAW : GL_STATIC_DRAW);

    glVertexAttribPointer(index, vertex_size, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(index);
}