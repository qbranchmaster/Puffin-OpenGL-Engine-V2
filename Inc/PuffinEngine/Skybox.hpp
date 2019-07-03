/*
 * Puffin OpenGL Engine ver. 2.0.1
 * Coded by: Sebastian 'qbranchmaster' Tabaka
 * Contact: sebastian.tabaka@outlook.com
 */

#ifndef PUFFIN_SKYBOX_HPP
#define PUFFIN_SKYBOX_HPP

#include "PuffinEngine/BaseMesh.hpp"
#include "PuffinEngine/Texture.hpp"

namespace puffin {
    class Skybox : public BaseMesh {
        friend class SceneLoader;

    public:
        explicit Skybox(std::string name = "");

        void setTexture(TexturePtr texture);

        TexturePtr getTexture() const {
            return texture_;
        }

        MeshType getMeshType() const {
            return MeshType::Skybox;
        }

    private:
        TexturePtr texture_{nullptr};
    };

    using SkyboxPtr = std::shared_ptr<Skybox>;
} // namespace puffin

#endif // PUFFIN_SKYBOX_HPP