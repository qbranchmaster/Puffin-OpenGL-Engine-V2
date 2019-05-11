/*
* Puffin OpenGL Engine ver. 2.0
* Coded by: Sebastian 'qbranchmaster' Tabaka
* Contact: sebastian.tabaka@outlook.com
*/

#include "PuffinEngine/Mesh.hpp"

using namespace puffin;

glm::mat4 Mesh::assimpMat4ToGlmMat4(const aiMatrix4x4 *input) {
    glm::mat4 result(1.0f);
    if (!input) {
        return result;
    }

    result[0] = glm::vec4(input->a1, input->a2, input->a3, input->a4);
    result[1] = glm::vec4(input->b1, input->b2, input->b3, input->b4);
    result[2] = glm::vec4(input->c1, input->c2, input->c3, input->c4);
    result[3] = glm::vec4(input->d1, input->d2, input->d3, input->d4);

    // Transpose from row major to column major
    result = glm::transpose(result);

    return result;
}

void Mesh::processMeshNode(const aiNode *node, NodeData data) {
    // Update node data
    data.model_matrix *= assimpMat4ToGlmMat4(&(node->mTransformation));

    // Create mesh entities
    for (GLuint i = 0; i < node->mNumMeshes; i++) {
        auto mesh_index = node->mMeshes[i];

        MeshEntityPtr entity(new MeshEntity());
        entity->setModelMatrix(data.model_matrix);
        entity->setName(node->mName.C_Str());
        entity->setIndicesCount(entity_data_[mesh_index].indices_count);
        entity->setVerticesCount(entity_data_[mesh_index].vertices_count);
        entity->setStartingVetexNumber(entity_data_[mesh_index].starting_index);
        entity->setStartingVertexIndex(entity_data_[mesh_index].starting_index);
        entity->setMaterial(materials_[entity_data_[mesh_index].material_index]);
        entities_.push_back(entity);
    }

    for (GLuint i = 0; i < node->mNumChildren; i++) {
        processMeshNode(node->mChildren[i], data);
    }
}

void Mesh::loadMeshEntities(const aiScene *scene) {
    for (GLuint i = 0; i < scene->mNumMeshes; i++) {
        aiMesh *mesh = scene->mMeshes[i];

        MeshEntityLoaderData data = {};

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

            v_positions_.push_back(vp.x);
            v_positions_.push_back(vp.y);
            v_positions_.push_back(vp.z);

            v_normals_.push_back(vn.x);
            v_normals_.push_back(vn.y);
            v_normals_.push_back(vn.z);

            v_tex_coords_.push_back(vt.x);
            v_tex_coords_.push_back(vt.y);

            v_tangents_.push_back(tangent.x);
            v_tangents_.push_back(tangent.y);
            v_tangents_.push_back(tangent.z);

            v_bitangents_.push_back(bitangent.x);
            v_bitangents_.push_back(bitangent.y);
            v_bitangents_.push_back(bitangent.z);
        }

        for (GLuint f = 0; f < mesh->mNumFaces; f++) {
            aiFace *face = &mesh->mFaces[f];

            for (GLuint i = 0; i < face->mNumIndices; i++) {
                v_indices_.push_back(face->mIndices[i] + vertices_count_);
            }

            data.indices_count += face->mNumIndices;
        }

        data.starting_vertex = vertices_count_;
        vertices_count_ += mesh->mNumVertices;
        data.starting_index = indices_count_;
        data.vertices_count = mesh->mNumVertices;
        indices_count_ += data.indices_count;
        data.material_index = mesh->mMaterialIndex;

        entity_data_.push_back(data);
    }

    loadMaterials(scene);
}

void Mesh::loadMaterials(const aiScene * scene) {
    if (!scene->HasMaterials()) {
        return;
    }

    for (GLuint i = 0; i < scene->mNumMaterials; i++) {
        aiMaterial *material = scene->mMaterials[i];
        MaterialPtr mesh_material(new Material());

        aiString texture_path;
        if (material->GetTexture(aiTextureType_AMBIENT, 0, &texture_path) == AI_SUCCESS) {
            TexturePtr ambient_tex(new Texture());
            ambient_tex->loadTexture2D(processTexturePath(path_, texture_path), true);
            mesh_material->setAmbientTexture(ambient_tex);
        }

        if (material->GetTexture(aiTextureType_DIFFUSE, 0, &texture_path) == AI_SUCCESS) {
            TexturePtr diffuse_tex(new Texture());
            diffuse_tex->loadTexture2D(processTexturePath(path_, texture_path), true);
            mesh_material->setDiffuseTexture(diffuse_tex);
        }

        if (material->GetTexture(aiTextureType_SPECULAR, 0, &texture_path) == AI_SUCCESS) {
            TexturePtr specular_tex(new Texture());
            specular_tex->loadTexture2D(processTexturePath(path_, texture_path), true);
            mesh_material->setSpecularTexture(specular_tex);
        }

        if (material->GetTexture(aiTextureType_EMISSIVE, 0, &texture_path) == AI_SUCCESS) {
            TexturePtr emissive_tex(new Texture());
            emissive_tex->loadTexture2D(processTexturePath(path_, texture_path), true);
            mesh_material->setEmissiveTexture(emissive_tex);
        }

        if (material->GetTexture(aiTextureType_NORMALS, 0, &texture_path) == AI_SUCCESS) {
            TexturePtr normalmap_tex(new Texture());
            normalmap_tex->loadTexture2D(processTexturePath(path_, texture_path), true);
            mesh_material->setNormalMapTexture(normalmap_tex);
        }

        if (material->GetTexture(aiTextureType_OPACITY, 0, &texture_path) == AI_SUCCESS) {
            TexturePtr opacity_tex(new Texture());
            opacity_tex->loadTexture2D(processTexturePath(path_, texture_path), true);
            mesh_material->setOpacityTexture(opacity_tex);
        }

        aiColor3D kd;
        if (material->Get(AI_MATKEY_COLOR_DIFFUSE, kd) == AI_SUCCESS) {
            mesh_material->setKd(glm::vec3(kd.r, kd.g, kd.b));
        }

        aiColor3D ka;
        if (material->Get(AI_MATKEY_COLOR_AMBIENT, ka) == AI_SUCCESS) {
            mesh_material->setKa(glm::vec3(ka.r, ka.g, ka.b));
        }

        aiColor3D ks;
        if (material->Get(AI_MATKEY_COLOR_SPECULAR, ks) == AI_SUCCESS) {
            mesh_material->setKs(glm::vec3(ks.r, ks.g, ks.b));
        }

        aiColor3D ke;
        if (material->Get(AI_MATKEY_COLOR_EMISSIVE, ke) == AI_SUCCESS) {
            mesh_material->setKe(glm::vec3(ke.r, ke.g, ke.b));
        }

        GLfloat shininess = 0;
        if (material->Get(AI_MATKEY_SHININESS, shininess) == AI_SUCCESS) {
            mesh_material->setShininess(static_cast<GLint>(shininess));
        }

        GLfloat reflectivity = 0;
        if (material->Get(AI_MATKEY_REFLECTIVITY, reflectivity) == AI_SUCCESS) {
            mesh_material->setReflectivity(reflectivity);
        }

        aiColor3D transparency;
        if (material->Get(AI_MATKEY_COLOR_TRANSPARENT, transparency) == AI_SUCCESS) {
            mesh_material->setTransparency(glm::vec3(transparency.r, transparency.g,
                transparency.b));
        }

        materials_.push_back(mesh_material);
    }
}

void Mesh::loadFromFile(std::string path) {
    if (path.empty()) {
        logError("Mesh::loadFromFile()", PUFFIN_MSG_FILE_EMPTY_PATH);
        return;
    }

    Assimp::Importer mesh_importer;
    const aiScene *scene = mesh_importer.ReadFile(path.c_str(),
        aiProcessPreset_TargetRealtime_Fast | aiProcess_FlipUVs);

    if (!scene) {
        logError("Mesh::loadFromFile()", "Importer message: " +
            std::string(mesh_importer.GetErrorString()) + ".");
        logError("Mesh::loadFromFile()", PUFFIN_MSG_FILE_CANNOT_OPEN(path));
        return;
    }

    path_ = path;

    // Zero object state
    freeVertexBuffers();
    entities_.clear();
    vertices_count_ = 0;
    indices_count_ = 0;

    // Load mesh entities to temporary buffer
    loadMeshEntities(scene);

    // Travel through mesh tree and get mesh entities data
    for (GLuint i = 0; i < scene->mRootNode->mNumChildren; i++) {
        NodeData data = {};
        data.model_matrix = glm::mat4(1.0f);
        processMeshNode(scene->mRootNode->mChildren[i], data);
    }

    // Mesh loading completed, fill OpenGL buffers now
    bind();
    setMeshData(v_positions_, 0, 3);
    setMeshData(v_tex_coords_, 1, 2);
    setMeshData(v_normals_, 2, 3);
    setMeshData(v_tangents_, 3, 3);
    setMeshData(v_bitangents_, 4, 3);
    setMeshIndices(v_indices_);

    // OpenGL buffers are set now, so free some data
    freeVertexData();
    entity_data_.clear();
    materials_.clear();

    logInfo("Mesh::loadFromFile()", PUFFIN_MSG_FILE_LOADED(path_));
}

std::string Mesh::processTexturePath(std::string model_file_path,
    const aiString & texture_path) {
    std::size_t slash_pos = model_file_path.find_last_of("/\\");
    std::string path = model_file_path.substr(0, slash_pos);
    std::string name = texture_path.C_Str();
    if (name[0] == '/') {
        name.erase(0, 1);
    }

    std::string file_path = path + "/" + name;
    return file_path;
}