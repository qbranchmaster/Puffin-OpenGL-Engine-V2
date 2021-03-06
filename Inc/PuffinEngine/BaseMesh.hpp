/*
 * Puffin OpenGL Engine ver. 2.0.1
 * Coded by: Sebastian 'qbranchmaster' Tabaka
 * Contact: sebastian.tabaka@outlook.com
 */

#ifndef PUFFIN_BASE_MESH_HPP
#define PUFFIN_BASE_MESH_HPP

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "PuffinEngine/Logger.hpp"
#include "PuffinEngine/MeshEntity.hpp"

namespace puffin {
    enum class MeshType {
        BaseMesh,
        Skybox,
        Mesh,
        WaterTile,
    };

    class BaseMesh {
    public:
        explicit BaseMesh(std::string name);
        virtual ~BaseMesh();

        std::string getName() const {
            return name_;
        }

        void bind() const {
            glBindVertexArray(handle_);
        }

        GLuint getHandle() const {
            return handle_;
        }

        void freeVertexData();

        void setMeshData(const std::vector<GLfloat> &data, GLuint index, GLuint vertex_size,
            GLboolean dynamic_draw = false);
        void setMeshIndices(const std::vector<GLuint> &data);

        MeshEntityPtr addEntity(std::string name);
        MeshEntityPtr getEntity(GLuint index) const;

        GLuint getEntitiesCount() const {
            return entities_.size();
        }

        void setScale(const glm::vec3 &scale);

        glm::vec3 getScale() const {
            return scale_;
        }

        void translate(const glm::vec3 &translation);
        void setPosition(const glm::vec3 &position);
        void zeroTranslation();

        glm::vec3 getPosition() const {
            return position_;
        }

        void setRotation(const glm::vec3 &angles);
        void rotate(const glm::vec3 &angles);

        glm::vec3 getRotationAngles() const {
            return rotation_angles_;
        }

        void zeroRotation();

        void setRotationMatrix(const glm::mat4 &matrix) {
            rotation_matrix_ = matrix;
            model_matrix_changed_ = true;
        }

        glm::mat4 getRotationMatrix() const {
            return rotation_matrix_;
        }

        glm::mat4 getScaleMatrix() const {
            return scale_matrix_;
        }

        glm::mat4 getTranslationMatrix() const {
            return translation_matrix_;
        }

        glm::mat4 getModelMatrix() {
            if (model_matrix_changed_) {
                model_matrix_ = translation_matrix_ * rotation_matrix_ * scale_matrix_;
                normal_matrix_ = glm::transpose(glm::inverse(glm::mat3(model_matrix_)));

                model_matrix_changed_ = false;
            }

            return model_matrix_;
        }

        glm::mat3 getNormalMatrix() const {
            return normal_matrix_;
        }

        virtual MeshType getMeshType() const {
            return MeshType::BaseMesh;
        }

    protected:
        void deleteVertexBuffers();

        GLuint handle_{0};
        GLuint indices_buffer_{0};
        std::map<GLuint, GLuint> data_buffers_;

        std::string name_;

        std::vector<MeshEntityPtr> entities_;

        GLboolean model_matrix_changed_{false};
        glm::mat4 model_matrix_{1.0f};
        glm::mat3 normal_matrix_{1.0f};
        glm::mat4 rotation_matrix_{1.0f};
        glm::mat4 scale_matrix_{1.0f};
        glm::mat4 translation_matrix_{1.0f};
        glm::vec3 position_{0.0f, 0.0f, 0.0f};
        glm::vec3 scale_{1.0f, 1.0f, 1.0f};

        glm::vec3 rotation_angles_{0.0f, 0.0f, 0.0f};

        std::vector<GLfloat> v_positions_;
        std::vector<GLfloat> v_normals_;
        std::vector<GLfloat> v_tex_coords_;
        std::vector<GLfloat> v_tangents_;
        std::vector<GLfloat> v_bitangents_;
        std::vector<GLuint> v_indices_;
    };

    using BaseMeshPtr = std::shared_ptr<BaseMesh>;
} // namespace puffin

#endif // PUFFIN_BASE_MESH_HPP