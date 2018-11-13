/*
* Puffin OpenGL Engine ver. 2.0
* Created by: Sebastian 'qbranchmaster' Tabaka
*/

#ifndef PUFFIN_MESH_HPP
#define PUFFIN_MESH_HPP

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <map>
#include <memory>
#include <vector>

#include "PuffinEngine/Logger.hpp"
#include "PuffinEngine/MeshEntity.hpp"
#include "PuffinEngine/StateMachine.hpp"

namespace puffin {
    class Mesh {
    public:
        Mesh();
        ~Mesh();

        void bind() {
            if (!handle_) {
                logError("Mesh::bind()", "Cannot bind null mesh.");
                return;
            }

            if (StateMachine::instance().bound_mesh_handle_ == handle_) {
                return;
            }

            glBindVertexArray(handle_);
            StateMachine::instance().bound_mesh_handle_ = handle_;
        }

        void unbind() {
            if (StateMachine::instance().bound_mesh_handle_ == handle_) {
                glBindVertexArray(0);
                StateMachine::instance().bound_mesh_handle_ = 0;
            }
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

        void setScale(const glm::vec3 &scale) {
            scale_ = scale;
            scale_matrix_ = glm::scale(glm::mat4(1.0f), scale_);
            model_matrix_changed_ = true;
        }

        void translate(const glm::vec3 &translation) {
            translation_matrix_ = glm::translate(translation_matrix_,
                translation);
            position_ += translation;
            model_matrix_changed_ = true;
        }

        void setPosition(const glm::vec3 &position) {
            translation_matrix_ = glm::mat4(1.0f);
            position_ = glm::vec3(0.0f, 0.0f, 0.0f);
            translate(position);
        }

        void zeroTranslation() {
            translation_matrix_ = glm::mat4(1.0f);
            model_matrix_changed_ = true;
        }

        glm::vec3 getPosition() const {
            return position_;
        }

        void rotate(GLfloat angle, const glm::vec3 &axis) {
            rotation_matrix_ = glm::rotate(rotation_matrix_, angle, axis);
            model_matrix_changed_ = true;
        }

        void setRotationAngle(GLfloat angle, const glm::vec3 &axis) {
            rotation_matrix_ = glm::mat4(1.0f);
            rotate(angle, axis);
        }

        void zeroRotation() {
            rotation_matrix_ = glm::mat4(1.0f);
            model_matrix_changed_ = true;
        }

        void setMeshData(std::vector<GLfloat> data, GLuint index,
            GLuint vertex_size, GLboolean dynamic_draw = false,
            GLboolean is_indices = false);
        MeshEntityPtr addEntity();
        MeshEntityPtr getEntity(GLuint index) const;
        GLuint getEntitiesCount() const;

        // TODO: Move it somewhere else.
        void draw(GLuint index) {
            if (index >= entities_.size()) {
                return;
            }

            auto entity = entities_[index];
            glDrawArrays(GL_TRIANGLES, 0, entity->getVerticesCount());
        }

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