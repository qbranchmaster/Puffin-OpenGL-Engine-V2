/*
 * Puffin OpenGL Engine ver. 2.0.1
 * Coded by: Sebastian 'qbranchmaster' Tabaka
 * Contact: sebastian.tabaka@outlook.com
 */

#ifndef PUFFIN_MESH_ENTITY_HPP
#define PUFFIN_MESH_ENTITY_HPP

#ifdef WIN32 // Prevents APIENTRY redefinition
#include <Windows.h>
#endif // WIN32
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <memory>
#include <string>

#include "PuffinEngine/Material.hpp"

namespace puffin {
    class MeshEntity {
    public:
        explicit MeshEntity(std::string name = "") {
            name_ = name;
        }

        void setStartingVertexIndex(GLuint index) {
            starting_index_ = index;
        }

        GLuint getStartingVertexIndex() const {
            return starting_index_;
        }

        void setStartingVetexNumber(GLuint vetex_index) {
            starting_vertex_ = vetex_index;
        }

        GLuint getStartingVertexNumber() const {
            return starting_vertex_;
        }

        void setIndicesCount(GLuint count) {
            indices_count_ = count;
        }

        GLuint getIndicesCount() const {
            return indices_count_;
        }

        void setVerticesCount(GLuint count) {
            vertices_count_ = count;
        }

        GLuint getVerticesCount() const {
            return vertices_count_;
        }

        void setMaterial(MaterialPtr material) {
            if (!material) {
                logError("MeshEntity::setMaterial()", PUFFIN_MSG_NULL_OBJECT);
                return;
            }

            material_ = material;
        }

        MaterialPtr getMaterial() const {
            return material_;
        }

        std::string getName() const {
            return name_;
        }

        void setModelMatrix(const glm::mat4 &matrix) {
            model_matrix_ = matrix;
        }

        glm::mat4 getModelMatrix() const {
            return model_matrix_;
        }

    private:
        GLuint starting_index_{0};
        GLuint starting_vertex_{0};
        GLuint indices_count_{0};
        GLuint vertices_count_{0};
        glm::mat4 model_matrix_{1.0f};

        MaterialPtr material_{nullptr};

        std::string name_;
    };

    using MeshEntityPtr = std::shared_ptr<MeshEntity>;
} // namespace puffin

#endif // PUFFIN_MESH_ENTITY_HPP