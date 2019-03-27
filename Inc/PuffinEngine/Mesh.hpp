/*
* Puffin OpenGL Engine ver. 2.0
* Coded by: Sebastian 'qbranchmaster' Tabaka
*/

#ifndef PUFFIN_MESH_HPP
#define PUFFIN_MESH_HPP

#ifdef WIN32 // Prevents APIENTRY redefinition
#include <Windows.h>
#endif // WIN32
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <map>
#include <memory>
#include <vector>

#include "PuffinEngine/Logger.hpp"
#include "PuffinEngine/MeshEntity.hpp"

namespace puffin {
    class Mesh {
    public:
        Mesh();
        ~Mesh();

        void bind() const {
            if (!handle_) {
                logError("Mesh::bind()", "Cannot bind null mesh.");
                return;
            }

            glBindVertexArray(handle_);
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

        void rotate(GLfloat angle, const glm::vec3 &axis);
        void setRotationAngle(GLfloat angle, const glm::vec3 &axis);
        void zeroRotation();

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
                model_matrix_ = translation_matrix_ * rotation_matrix_ *
                    scale_matrix_;
                normal_matrix_ = glm::transpose(glm::inverse(
                    glm::mat3(model_matrix_)));

                model_matrix_changed_ = false;
            }

            return model_matrix_;
        }

        glm::mat3 getNormalMatrix() const {
            return normal_matrix_;
        }

        GLuint getHandle() const {
            return handle_;
        }

        void setMeshData(std::vector<GLfloat> data, GLuint index,
            GLuint vertex_size, GLboolean dynamic_draw = false);
        void setMeshIndices(std::vector<GLuint> data);
        MeshEntityPtr addEntity();
        MeshEntityPtr getEntity(GLuint index) const;

        void loadFromFile(std::string path);

        GLuint getEntitiesCount() const {
            return entities_.size();
        }

        GLboolean hasIndices() const {
            return has_indices_;
        }

        void enableShadowCasting(GLboolean enabled) {
            shadow_cast_enabled_ = enabled;
        }

        GLboolean isShadowCastingEnabled() const {
            return shadow_cast_enabled_;
        }

    protected:
        std::string processTexturePath(std::string model_file_path,
            const aiString &texture_path);

        GLuint handle_{0};
        GLuint indices_buffer_{0};
        std::map<GLuint, GLuint> data_buffers_;
        std::vector<MeshEntityPtr> entities_;

        GLboolean model_matrix_changed_{false};
        glm::mat4 model_matrix_{1.0f};
        glm::mat3 normal_matrix_{1.0f};
        glm::mat4 rotation_matrix_{1.0f};
        glm::mat4 scale_matrix_{1.0f};
        glm::mat4 translation_matrix_{1.0f};
        glm::vec3 position_{0.0f, 0.0f, 0.0f};
        glm::vec3 scale_{1.0f, 1.0f, 1.0f};

        GLboolean has_indices_{false};

        GLboolean shadow_cast_enabled_{true};
    };

    using MeshPtr = std::shared_ptr<Mesh>;
} // namespace puffin

#endif // PUFFIN_MESH_HPP