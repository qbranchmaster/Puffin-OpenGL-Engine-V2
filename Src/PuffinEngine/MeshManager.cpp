#include "PuffinEngine/MeshManager.hpp"

using namespace puffin;

MeshPtr MeshManager::createMesh() {
    MeshPtr mesh(new Mesh());
    meshes_.push_back(mesh);
    return mesh;
}