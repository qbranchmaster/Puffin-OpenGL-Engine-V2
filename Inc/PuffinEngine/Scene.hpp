/*
* Puffin OpenGL Engine ver. 2.0
* Coded by: Sebastian 'qbranchmaster' Tabaka
*/

#ifndef PUFFIN_SCENE_HPP
#define PUFFIN_SCENE_HPP

#include <memory>
#include <vector>

#include "PuffinEngine/Mesh.hpp"
#include "PuffinEngine/Skybox.hpp"

namespace puffin {
    class Scene {
    public:
        void addMesh(MeshPtr mesh) {
            if (!mesh) {
                logError("Scene::addMesh()", "Null input.");
                return;
            }

            meshes_.push_back(mesh);
        }

        GLuint getMeshesCount() const {
            return meshes_.size();
        }

        MeshPtr getMesh(GLuint index) {
            if (index >= meshes_.size()) {
                logError("Scene::getMesh()", "Invalid input.");
                return MeshPtr();
            }

            return meshes_[index];
        }

        void setSkybox(SkyboxPtr skybox) {
            active_skybox_ = skybox;
        }

        SkyboxPtr getSkybox() const {
            return active_skybox_;
        }

    private:
        std::vector<MeshPtr> meshes_;
        SkyboxPtr active_skybox_;
    };

    using ScenePtr = std::shared_ptr<Scene>;
} // namespace puffin

#endif // PUFFIN_SCENE_HPP