/*
* Puffin OpenGL Engine ver. 2.0
* Coded by: Sebastian 'qbranchmaster' Tabaka
*/

#ifndef PUFFIN_SKYBOX_HPP
#define PUFFIN_SKYBOX_HPP

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
        TexturePtr texture_;
    };

    using SkyboxPtr = std::shared_ptr<Skybox>;
} // namespace puffin

#endif // PUFFIN_SKYBOX_HPP