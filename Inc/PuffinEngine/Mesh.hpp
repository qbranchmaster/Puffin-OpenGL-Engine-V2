#ifndef PUFFIN_MESH_HPP
#define PUFFIN_MESH_HPP

#include <GL/glew.h>

#include <map>
#include <memory>

namespace puffin {
    enum class VertexDataType {
        POSITION,
        NORMAL_VECTOR,
        TEXTURE_COORD,
        TANGENT,
        BITANGET,
        INDEX,
    };

    class Mesh {
    public:
        Mesh();
        ~Mesh();

    private:
        GLuint handle_{0};
        std::map<VertexDataType, GLuint> data_buffers_;
    };

    using MeshPtr = std::shared_ptr<Mesh>;
} // namespace puffin

#endif // PUFFIN_MESH_HPP