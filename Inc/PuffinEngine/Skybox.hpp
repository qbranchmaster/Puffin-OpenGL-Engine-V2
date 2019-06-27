/*
 * Puffin OpenGL Engine ver. 2.1
 * Coded by: Sebastian 'qbranchmaster' Tabaka
 * Contact: sebastian.tabaka@outlook.com
 */

#ifndef PUFFIN_SKYBOX_HPP
#define PUFFIN_SKYBOX_HPP

#include "PuffinEngine/BaseMesh.hpp"
#include "PuffinEngine/Texture.hpp"

namespace puffin {
    class Skybox;
    using SkyboxPtr = std::shared_ptr<Skybox>;

    class Skybox : public BaseMesh {
        friend std::ostream &operator<<(std::ostream &os, SkyboxPtr skybox);
        friend std::istream &operator>>(std::istream &is, SkyboxPtr skybox);

    public:
        Skybox(std::string name = "");

        void setTexture(TexturePtr texture);

        TexturePtr getTexture() const {
            return texture_;
        }

    private:
        TexturePtr texture_{nullptr};
    };
} // namespace puffin

#endif // PUFFIN_SKYBOX_HPP