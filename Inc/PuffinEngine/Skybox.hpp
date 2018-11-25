/*
* Puffin OpenGL Engine ver. 2.0
* Created by: Sebastian 'qbranchmaster' Tabaka
*/

#ifndef PUFFIN_SKYBOX_HPP
#define PUFFIN_SKYBOX_HPP

#include <memory>

#include "PuffinEngine/Mesh.hpp"
#include "PuffinEngine/Texture.hpp"

namespace puffin {
    class Skybox : public Mesh {
    public:
        Skybox();

        void setTexture(TexturePtr texture);

        TexturePtr getTexture() const {
            return texture_;
        }

    private:
        GLfloat rotation_angle_{0.0f};

        TexturePtr texture_;
    };

    using SkyboxPtr = std::shared_ptr<Skybox>;
} // namespace puffin

#endif // PUFFIN_SKYBOX_HPP