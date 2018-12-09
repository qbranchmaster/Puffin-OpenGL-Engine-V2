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

void Mesh::loadFromFile(std::string path) {
    if (path.empty()) {
        logError("Mesh::loadFromFile()", "Invalid input.");
        return;
    }

    Assimp::Importer mesh_importer;
    const aiScene *scene = mesh_importer.ReadFile(path.c_str(),
        aiProcessPreset_TargetRealtime_Fast | aiProcess_FlipUVs);

    if (!scene) {
        logError("Mesh::loadFromFile()", "Importer message: " +
            std::string(mesh_importer.GetErrorString()) + ".");
        logError("Mesh::loadFromFile()", "File [" + path + "] loading error.");
        return;
    }

    logInfo("Mesh::loadFromFile()", "File [" + path + "] loaded.");

    GLint vertices_count = 0;
    GLint indices_count = 0;

    std::vector<GLfloat> v_positions;
    std::vector<GLfloat> v_normals;
    std::vector<GLfloat> v_tex_coords;
    std::vector<GLfloat> v_tangents;
    std::vector<GLfloat> v_bitangents;
    std::vector<GLuint> v_indices;

    entities_.clear();

    for (GLuint m = 0; m < scene->mNumMeshes; m++) {
        aiMesh *mesh = scene->mMeshes[m];

        MeshEntityPtr entity(new MeshEntity());

        for (GLuint v = 0; v < mesh->mNumVertices; v++) {
            aiVector3D vp(0.0f, 0.0f, 0.0f);
            aiVector3D vn(0.0f, 1.0f, 0.0f);
            aiVector3D vt(0.0f, 0.0f, 0.0f);
            aiVector3D tangent(0.0f, 0.0f, 0.0f);
            aiVector3D bitangent(0.0f, 0.0f, 0.0f);

            if (mesh->HasPositions()) {
                vp = mesh->mVertices[v];
            }

            if (mesh->HasNormals()) {
                vn = mesh->mNormals[v];
            }

            if (mesh->HasTextureCoords(0)) {
                vt = mesh->mTextureCoords[0][v];
            }

            if (mesh->HasTangentsAndBitangents()) {
                tangent = mesh->mTangents[v];
                bitangent = mesh->mBitangents[v];
            }

            v_positions.push_back(vp.x);
            v_positions.push_back(vp.y);
            v_positions.push_back(vp.z);

            v_normals.push_back(vn.x);
            v_normals.push_back(vn.y);
            v_normals.push_back(vn.z);

            v_tex_coords.push_back(vt.x);
            v_tex_coords.push_back(vt.y);

            v_tangents.push_back(tangent.x);
            v_tangents.push_back(tangent.y);
            v_tangents.push_back(tangent.z);

            v_bitangents.push_back(bitangent.x);
            v_bitangents.push_back(bitangent.y);
            v_bitangents.push_back(bitangent.z);
        }

        for (GLuint f = 0; f < mesh->mNumFaces; f++) {
            aiFace *face = &mesh->mFaces[f];

            for (GLuint i = 0; i < face->mNumIndices; i++) {
                v_indices.push_back(face->mIndices[i] + vertices_count);
                entity->setIndicesCount(entity->getIndicesCount() + 1);
            }
        }

        vertices_count += mesh->mNumVertices;
        entity->setStartingIndex(indices_count);
        entity->setVerticesCount(mesh->mNumVertices);
        indices_count += entity->getIndicesCount();

        // TODO: Material loading here.

        entities_.push_back(entity);
    }

    bind();
    setMeshData(v_positions, 0, 3);
    setMeshData(v_tex_coords, 1, 2);
    setMeshData(v_normals, 2, 3);
    setMeshData(v_tangents, 3, 3);
    setMeshData(v_bitangents, 4, 3);
    setMeshIndices(v_indices);
    unbind();

    has_indices_ = true;
}

GLboolean Mesh::isBound() const {
    if (StateMachine::instance().bound_mesh_ == handle_) {
        return true;
    }

    return false;
}

void Mesh::setMeshData(std::vector<GLfloat> data, GLuint index,
    GLuint vertex_size, GLboolean dynamic_draw) {
    if (!isBound()) {
        logError("Mesh::setMeshData()", "Mesh is not bound.");
        return;
    }

    if (data_buffers_[index] == 0) {
        glGenBuffers(1, &data_buffers_[index]);
    }

    glBindBuffer(GL_ARRAY_BUFFER, data_buffers_[index]);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(GLfloat), data.data(),
        dynamic_draw ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);

    glVertexAttribPointer(index, vertex_size, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(index);
}

void Mesh::setMeshIndices(std::vector<GLuint> data) {
    if (!isBound()) {
        logError("Mesh::setMeshIndices()", "Mesh is not bound.");
        return;
    }

    if (indices_buffer_ == 0) {
        glGenBuffers(1, &indices_buffer_);
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_buffer_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size() * sizeof(GLuint),
        data.data(), GL_STATIC_DRAW);
}