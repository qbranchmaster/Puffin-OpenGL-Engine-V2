/*
* Puffin OpenGL Engine ver. 2.0
* Coded by: Sebastian 'qbranchmaster' Tabaka
*/

#ifndef PUFFIN_MESH_ENTITY_HPP
#define PUFFIN_MESH_ENTITY_HPP

#ifdef WIN32 // Prevents APIENTRY redefinition
#include <Windows.h>
#endif // WIN32
#include <GL/glew.h>

#include <memory>

namespace puffin {
    class MeshEntity {
    public:
        void setStartingIndex(GLuint index) {
            starting_index_ = index;
        }

        GLuint getStartingIndex() const {
            return starting_index_;
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

    private:
        GLuint starting_index_{0};
        GLuint indices_count_{0};
        GLuint vertices_count_{0};
    };

    using MeshEntityPtr = std::shared_ptr<MeshEntity>;
} // namespace puffin

#endif // PUFFIN_MESH_ENTITY_HPP