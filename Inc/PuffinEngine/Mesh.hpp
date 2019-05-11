/*
* Puffin OpenGL Engine ver. 2.0
* Coded by: Sebastian 'qbranchmaster' Tabaka
* Contact: sebastian.tabaka@outlook.com
*/

#ifndef PUFFIN_MESH_HPP
#define PUFFIN_MESH_HPP

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "PuffinEngine/BaseMesh.hpp"

namespace puffin {
    class Mesh : public BaseMesh {
    public:
        void loadFromFile(std::string path);

        std::string getPath() const {
            return path_;
        }

        void enableShadowCasting(GLboolean enabled) {
            shadow_cast_enabled_ = enabled;
        }

        GLboolean isShadowCastingEnabled() const {
            return shadow_cast_enabled_;
        }

    protected:
        struct NodeData {
            glm::mat4 model_matrix;
        };

        struct MeshEntityLoaderData {
            GLuint starting_vertex;
            GLuint starting_index;
            GLuint indices_count;
            GLuint vertices_count;
            GLuint material_index;
        };

        std::string processTexturePath(std::string model_file_path, const aiString &texture_path);
        glm::mat4 assimpMat4ToGlmMat4(const aiMatrix4x4 *matrix);

        void processMeshNode(const aiNode *node, NodeData data);
        void loadMeshEntities(const aiScene *scene);
        void loadMaterials(const aiScene *scene);

        std::string path_;
        GLint vertices_count_{0};
        GLint indices_count_{0};

        std::vector<MeshEntityLoaderData> entity_data_;
        std::vector<MaterialPtr> materials_;

        GLboolean shadow_cast_enabled_{true};
    };

    using MeshPtr = std::shared_ptr<Mesh>;
} // namespace puffin

#endif // PUFFIN_MESH_HPP