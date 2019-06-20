/*
 * Puffin OpenGL Engine ver. 2.1
 * Coded by: Sebastian 'qbranchmaster' Tabaka
 * Contact: sebastian.tabaka@outlook.com
 */

#include "PuffinEngine/BaseMesh.hpp"

using namespace puffin;

BaseMesh::BaseMesh(std::string name) {
    glGenVertexArrays(1, &handle_);

    name_ = name;
}

BaseMesh::~BaseMesh() {
    deleteVertexBuffers();

    if (handle_) {
        glDeleteVertexArrays(1, &handle_);
    }
}

void BaseMesh::deleteVertexBuffers() {
    for (auto &buffer : data_buffers_) {
        if (buffer.second != 0) {
            glDeleteBuffers(1, &buffer.second);
        }
    }
}

void BaseMesh::setScale(const glm::vec3 &scale) {
    scale_ = scale;
    scale_matrix_ = glm::scale(glm::mat4(1.0f), scale_);
    model_matrix_changed_ = true;
}

void BaseMesh::translate(const glm::vec3 &translation) {
    translation_matrix_ = glm::translate(translation_matrix_, translation);
    position_ += translation;
    model_matrix_changed_ = true;
}

void BaseMesh::setPosition(const glm::vec3 &position) {
    translation_matrix_ = glm::mat4(1.0f);
    position_ = glm::vec3(0.0f, 0.0f, 0.0f);
    translate(position);
}

void BaseMesh::zeroTranslation() {
    translation_matrix_ = glm::mat4(1.0f);
    model_matrix_changed_ = true;
}

void BaseMesh::rotate(GLfloat angle, const glm::vec3 &axis) {
    rotation_matrix_ = glm::rotate(rotation_matrix_, angle, axis);
    model_matrix_changed_ = true;
}

void BaseMesh::setRotationAngle(GLfloat angle, const glm::vec3 &axis) {
    rotation_matrix_ = glm::mat4(1.0f);
    rotate(angle, axis);
}

void BaseMesh::zeroRotation() {
    rotation_matrix_ = glm::mat4(1.0f);
    model_matrix_changed_ = true;
}

void BaseMesh::freeVertexData() {
    v_positions_.clear();
    v_normals_.clear();
    v_tex_coords_.clear();
    v_tangents_.clear();
    v_bitangents_.clear();
    v_indices_.clear();
}

void BaseMesh::setMeshData(
    const std::vector<GLfloat> &data, GLuint index, GLuint vertex_size, GLboolean dynamic_draw) {
    bind();

    if (data_buffers_[index] == 0) {
        glGenBuffers(1, &data_buffers_[index]);
    }

    glBindBuffer(GL_ARRAY_BUFFER, data_buffers_[index]);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(GLfloat), data.data(),
        dynamic_draw ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);

    glVertexAttribPointer(index, vertex_size, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(index);
}

void BaseMesh::setMeshIndices(const std::vector<GLuint> &data) {
    bind();

    if (indices_buffer_ == 0) {
        glGenBuffers(1, &indices_buffer_);
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_buffer_);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER, data.size() * sizeof(GLuint), data.data(), GL_STATIC_DRAW);
}

MeshEntityPtr BaseMesh::addEntity() {
    MeshEntityPtr entity(new MeshEntity());
    entities_.push_back(entity);
    return entity;
}

MeshEntityPtr BaseMesh::getEntity(GLuint index) const {
    if (index >= entities_.size()) {
        logError("Mesh::getEntity()", PUFFIN_MSG_OUT_OF_RANGE(0, entities_.size()));
        return MeshEntityPtr();
    }

    return entities_[index];
}