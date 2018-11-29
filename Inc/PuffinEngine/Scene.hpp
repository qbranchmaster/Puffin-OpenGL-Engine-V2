/*
* Puffin OpenGL Engine ver. 2.0
* Created by: Sebastian 'qbranchmaster' Tabaka
*/

#ifndef PUFFIN_SCENE_HPP
#define PUFFIN_SCENE_HPP

#include <memory>

#include "PuffinEngine/Skybox.hpp"

namespace puffin {
    class Scene {
        friend class MasterRenderer;

    public:
        void setSkybox(SkyboxPtr skybox) {
            active_skybox_ = skybox;
        }

    private:
        SkyboxPtr active_skybox_;
    };

    using ScenePtr = std::shared_ptr<Scene>;
} // namespace puffin

#endif // PUFFIN_SCENE_HPP