/*
* Puffin OpenGL Engine ver. 2.0
* Coded by: Sebastian 'qbranchmaster' Tabaka
*/

#ifndef PUFFIN_SCENE_HPP
#define PUFFIN_SCENE_HPP

#include <memory>

#include "PuffinEngine/Skybox.hpp"

namespace puffin {
    class Scene {
    public:
        void setSkybox(SkyboxPtr skybox) {
            active_skybox_ = skybox;
        }

        SkyboxPtr getSkybox() const {
            return active_skybox_;
        }

    private:
        SkyboxPtr active_skybox_;
    };

    using ScenePtr = std::shared_ptr<Scene>;
} // namespace puffin

#endif // PUFFIN_SCENE_HPP