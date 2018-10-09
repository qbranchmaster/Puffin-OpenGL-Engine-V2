#ifndef PUFFIN_MESH_MANAGER_HPP
#define PUFFIN_MESH_MANAGER_HPP

#include <memory>
#include <vector>

#include "PuffinEngine/Mesh.hpp"

namespace puffin {
    class MeshManager {
    public:
        MeshPtr createMesh();

    private:
        std::vector<MeshPtr> meshes_;
    };

    using MeshManagerPtr = std::shared_ptr<MeshManager>;
} // namespace puffin

#endif // PUFFIN_MESH_MANAGER_HPP