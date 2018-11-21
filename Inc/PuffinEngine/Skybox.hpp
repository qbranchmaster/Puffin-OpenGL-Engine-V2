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

        void setFilterColor(const glm::vec3 &color) {
            filter_color_ = color;
        }

        glm::vec3 getFilterColor() const {
            return filter_color_;
        }

        void setTexture(TexturePtr texture);

        TexturePtr getTexture() const {
            return texture_;
        }

    private:
        glm::vec3 filter_color_{1.0f, 1.0f, 1.0f};
        GLfloat rotation_angle_{0.0f};

        TexturePtr texture_;
    };

    using SkyboxPtr = std::shared_ptr<Skybox>;
} // namespace puffin

#endif // PUFFIN_SKYBOX_HPP