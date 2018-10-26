#ifndef PUFFIN_MESH_HPP
#define PUFFIN_MESH_HPP

#include <GL/glew.h>

#include <map>
#include <memory>
#include <vector>

#include "PuffinEngine/MeshEntity.hpp"

namespace puffin {
    enum class VertexDataType {
        POSITION      = 0,
        NORMAL_VECTOR = 1,
        TEXTURE_COORD = 2,
        TANGENT       = 3,
        BITANGET      = 4,
        INDEX         = 5,
    };

    class Mesh {
    public:
        Mesh();
        ~Mesh();

        void bind() const {
            glBindVertexArray(handle_);
        }

        void setMeshData(std::vector<GLfloat> data, VertexDataType data_type,
            GLboolean dynamic_draw = false);
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
        std::map<VertexDataType, GLuint> data_buffers_;
        std::vector<MeshEntityPtr> entities_;
    };

    using MeshPtr = std::shared_ptr<Mesh>;
} // namespace puffin

#endif // PUFFIN_MESH_HPP