#include "PuffinEngine/Mesh.hpp"

#include "PuffinEngine/Logger.hpp"

using namespace puffin;

Mesh::Mesh() {
    glGenVertexArrays(1, &handle_);
}

Mesh::~Mesh() {
    if (handle_) {
        glDeleteVertexArrays(1, &handle_);
    }
}

void Mesh::setMeshData(std::vector<GLfloat> data, VertexDataType data_type,
    GLboolean dynamic_draw) {
    GLboolean buff_not_set = false;
    if (data_buffers_[data_type] == 0) {
        glGenBuffers(1, &data_buffers_[data_type]);
        buff_not_set = true;
    }

    glBindVertexArray(handle_);
    glBindBuffer(data_type == VertexDataType::INDEX ? GL_ELEMENT_ARRAY_BUFFER :
        GL_ARRAY_BUFFER, handle_);

    GLushort vertex_size = 0;
    switch (data_type) {
    case VertexDataType::POSITION:
        glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(GLfloat),
            data.data(), dynamic_draw ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
        vertex_size = 3;
        break;
    }

    if (buff_not_set) {
        glVertexAttribPointer(static_cast<GLushort>(data_type), vertex_size,
            GL_FLOAT, GL_FALSE, 0, nullptr);
        glEnableVertexAttribArray(static_cast<GLushort>(data_type));
    }
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