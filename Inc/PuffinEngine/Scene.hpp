/*
* Puffin OpenGL Engine ver. 2.0
* Coded by: Sebastian 'qbranchmaster' Tabaka
* Contact: sebastian.tabaka@outlook.com
*/

#ifndef PUFFIN_SCENE_HPP
#define PUFFIN_SCENE_HPP

#include <memory>
#include <vector>

#include "PuffinEngine/Mesh.hpp"
#include "PuffinEngine/Skybox.hpp"
#include "PuffinEngine/Text.hpp"

namespace puffin {
    class Scene {
    public:
        void addMesh(MeshPtr mesh) {
            if (!mesh) {
                logError("Scene::addMesh()", PUFFIN_MSG_NULL_OBJECT);
                return;
            }

            meshes_.push_back(mesh);
        }

        GLuint getMeshesCount() const {
            return meshes_.size();
        }

        MeshPtr getMesh(GLuint index) {
            if (index >= meshes_.size()) {
                logError("Scene::getMesh()", PUFFIN_MSG_OUT_OF_RANGE(0, meshes_.size()));
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

        void addText(TextPtr text) {
            if (!text) {
                logError("Scene::addText()", PUFFIN_MSG_NULL_OBJECT);
                return;
            }

            textes_.push_back(text);
        }

        GLuint getTextesCount() const {
            return textes_.size();
        }

        TextPtr getText(GLuint index) {
            if (index >= textes_.size()) {
                logError("Scene::getText()", PUFFIN_MSG_OUT_OF_RANGE(0, textes_.size()));
                return TextPtr();
            }

            return textes_[index];
        }

    private:
        std::vector<MeshPtr> meshes_;
        std::vector<TextPtr> textes_;
        SkyboxPtr active_skybox_{nullptr};
    };

    using ScenePtr = std::shared_ptr<Scene>;
} // namespace puffin

#endif // PUFFIN_SCENE_HPP