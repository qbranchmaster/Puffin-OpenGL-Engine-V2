/*
* Puffin OpenGL Engine ver. 2.0
* Created by: Sebastian 'qbranchmaster' Tabaka
*/

#include "PuffinEngine/Mesh.hpp"

using namespace puffin;

Mesh::Mesh() {
    glGenVertexArrays(1, &handle_);
}

Mesh::~Mesh() {
    if (handle_) {
        glDeleteVertexArrays(1, &handle_);
    }
}

void Mesh::setMeshData(std::vector<GLfloat> data, GLuint index,
    GLuint vertex_size, GLboolean dynamic_draw, GLboolean is_indices) {
    if (data_buffers_[index] == 0) {
        glGenBuffers(1, &data_buffers_[index]);
    }

    bind();
    glBindBuffer(is_indices == 1 ? GL_ELEMENT_ARRAY_BUFFER : GL_ARRAY_BUFFER,
        data_buffers_[index]);
    glBufferData(is_indices == 1 ? GL_ELEMENT_ARRAY_BUFFER : GL_ARRAY_BUFFER,
        data.size() * sizeof(GLfloat), data.data(), dynamic_draw ?
        GL_DYNAMIC_DRAW : GL_STATIC_DRAW);

    glVertexAttribPointer(index, vertex_size, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(index);
    unbind();
}

MeshEntityPtr Mesh::addEntity() {
    MeshEntityPtr entity(new MeshEntity());
    entities_.push_back(entity);
    return entity;
}

MeshEntityPtr Mesh::getEntity(GLuint index) const {
    if (index >= entities_.size()) {
        logError("Mesh::getEntity()", "Entity index out of range.");
        return MeshEntityPtr();
    }

    return entities_[index];
}

GLuint Mesh::getEntitiesCount() const {
    return entities_.size();
}