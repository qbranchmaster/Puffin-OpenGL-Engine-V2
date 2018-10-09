#include "PuffinEngine/Mesh.hpp"

using namespace puffin;

Mesh::Mesh() {
    glGenVertexArrays(1, &handle_);
}

Mesh::~Mesh() {
    if (handle_) {
        glDeleteVertexArrays(1, &handle_);
    }
}