#ifndef PUFFIN_MESH_HPP
#define PUFFIN_MESH_HPP

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <map>
#include <memory>
#include <vector>

#include "PuffinEngine/MeshEntity.hpp"

namespace puffin {
    class Mesh {
    public:
        Mesh();
        ~Mesh();

        void bind() const {
            glBindVertexArray(handle_);
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
                model_matrix_ = translation_matrix_ * rotation_matrix_ *
                    scale_matrix_;
                model_matrix_changed_ = false;
            }

            return model_matrix_;
        }

        void setMeshData(std::vector<GLfloat> data, GLuint index,
            GLuint vertex_size, GLboolean dynamic_draw = false,
            GLboolean is_indices = false);
        MeshEntityPtr addEntity();
        MeshEntityPtr getEntity(GLuint index) const;
        GLuint getEntitiesCount() const;

        // TODO ----
        void draw(GLuint index) {
            if (index >= entities_.size()) {
                return;
            }

            auto entity = entities_[index];
            glDrawArrays(GL_TRIANGLES, 0, entity->getVerticesCount());
        }
        // -----

    private:
        GLuint handle_{0};
        std::map<GLuint, GLuint> data_buffers_;
        std::vector<MeshEntityPtr> entities_;

        GLboolean model_matrix_changed_{false};
        glm::mat4 model_matrix_{1.0f};
        glm::mat4 rotation_matrix_{1.0f};
        glm::mat4 scale_matrix_{1.0f};
        glm::mat4 translation_matrix_{1.0f};
        glm::vec3 position_{0.0f, 0.0f, 0.0f};
        glm::vec3 scale_{1.0f, 1.0f, 1.0f};
    };

    using MeshPtr = std::shared_ptr<Mesh>;
} // namespace puffin

#endif // PUFFIN_MESH_HPP