/*
 * Puffin OpenGL Engine ver. 2.0.1
 * Coded by: Sebastian 'qbranchmaster' Tabaka
 * Contact: sebastian.tabaka@outlook.com
 */

#ifndef PUFFIN_SCENE_HPP
#define PUFFIN_SCENE_HPP

#include <memory>
#include <vector>

#include "PuffinEngine/Camera.hpp"
#include "PuffinEngine/Fog.hpp"
#include "PuffinEngine/Lighting.hpp"
#include "PuffinEngine/Mesh.hpp"
#include "PuffinEngine/Skybox.hpp"
#include "PuffinEngine/Text.hpp"
#include "PuffinEngine/WaterTile.hpp"

namespace puffin {
    class Scene {
    public:
        Scene(std::string name) {
            name_ = name;

            camera_.reset(new Camera());
            fog_.reset(new Fog());
            lighting_.reset(new Lighting());
        }

        void reset() {
            active_skybox_ = nullptr;

            meshes_.clear();
            water_tiles_.clear();
            skyboxes_.clear();
            textes_.clear();

            lighting()->reset();
        }

        FogPtr fog() const {
            return fog_;
        }

        LightingPtr lighting() const {
            return lighting_;
        }

        CameraPtr camera() const {
            return camera_;
        }

        MeshPtr addMesh(std::string name) {
            if (isNameExistsAlready(name, meshes_)) {
                logError("Scene::addMesh()", PUFFIN_MSG_NAME_ALREADY_EXISTS(name));
                return nullptr;
            }

            MeshPtr mesh(new Mesh(name));
            meshes_.push_back(mesh);
            return mesh;
        }

        void removeMesh(std::string name) {
            auto pos = std::find_if(meshes_.begin(), meshes_.end(),
                [&](const auto &val) { return val->getName() == name; });
            if (pos != meshes_.end()) {
                meshes_.erase(pos);
            }
            else {
                logError("Scene::removeMesh()", PUFFIN_MSG_NAME_NOT_EXISTS(name));
            }
        }

        MeshPtr getMesh(GLuint index) {
            if (index >= meshes_.size()) {
                logError("Scene::getMesh()", PUFFIN_MSG_OUT_OF_RANGE(0, meshes_.size()));
                return nullptr;
            }

            return meshes_[index];
        }

        GLuint getMeshesCount() const {
            return meshes_.size();
        }

        SkyboxPtr addSkybox(std::string name) {
            if (isNameExistsAlready(name, skyboxes_)) {
                logError("Scene::addSkybox()", PUFFIN_MSG_NAME_ALREADY_EXISTS(name));
                return nullptr;
            }

            SkyboxPtr skybox(new Skybox(name));
            skyboxes_.push_back(skybox);
            return skybox;
        }

        void removeSkybox(std::string name) {
            auto pos = std::find_if(skyboxes_.begin(), skyboxes_.end(),
                [&](const auto &val) { return val->getName() == name; });
            if (pos != skyboxes_.end()) {
                if (*pos == active_skybox_) {
                    active_skybox_ = nullptr;
                }

                skyboxes_.erase(pos);
            }
            else {
                logError("Scene::removeSkybox()", PUFFIN_MSG_NAME_NOT_EXISTS(name));
            }
        }

        SkyboxPtr getSkybox(GLuint index) {
            if (index >= skyboxes_.size()) {
                logError("Scene::getSkybox()", PUFFIN_MSG_OUT_OF_RANGE(0, skyboxes_.size()));
                return nullptr;
            }

            return skyboxes_[index];
        }

        GLuint getSkyboxesCount() const {
            return skyboxes_.size();
        }

        void setActiveSkybox(SkyboxPtr skybox) {
            // nullptr is allowed - turns off skybox
            active_skybox_ = skybox;
        }

        SkyboxPtr getActiveSkybox() const {
            return active_skybox_;
        }

        WaterTilePtr addWaterTile(std::string name) {
            if (isNameExistsAlready(name, water_tiles_)) {
                logError("Scene::addWaterTile()", PUFFIN_MSG_NAME_ALREADY_EXISTS(name));
                return nullptr;
            }

            WaterTilePtr water_tile(new WaterTile(name));
            water_tiles_.push_back(water_tile);
            return water_tile;
        }

        void removeWaterTile(std::string name) {
            auto pos = std::find_if(water_tiles_.begin(), water_tiles_.end(),
                [&](const auto &val) { return val->getName() == name; });
            if (pos != water_tiles_.end()) {
                water_tiles_.erase(pos);
            }
            else {
                logError("Scene::removeWaterTile()", PUFFIN_MSG_NAME_NOT_EXISTS(name));
            }
        }

        WaterTilePtr getWaterTile(GLuint index) {
            if (index >= water_tiles_.size()) {
                logError("Scene::getWaterTile()", PUFFIN_MSG_OUT_OF_RANGE(0, meshes_.size()));
                return nullptr;
            }

            return water_tiles_[index];
        }

        GLuint getWaterTilesCount() const {
            return water_tiles_.size();
        }

        TextPtr addText() {
            TextPtr text(new Text());
            textes_.push_back(text);
            return text;
        }

        TextPtr getText(GLuint index) {
            if (index >= textes_.size()) {
                logError("Scene::getText()", PUFFIN_MSG_OUT_OF_RANGE(0, textes_.size()));
                return nullptr;
            }

            return textes_[index];
        }

        GLuint getTextesCount() const {
            return textes_.size();
        }

        std::string getName() const {
            return name_;
        }

    private:
        template<typename T>
        GLboolean isNameExistsAlready(std::string name, const T &obj_cont) {
            for (const auto &obj : obj_cont) {
                if (obj->getName() == name) {
                    return true;
                }
            }

            return false;
        }

        std::vector<MeshPtr> meshes_;
        std::vector<WaterTilePtr> water_tiles_;
        std::vector<SkyboxPtr> skyboxes_;
        std::vector<TextPtr> textes_;

        SkyboxPtr active_skybox_{nullptr};

        CameraPtr camera_{nullptr};
        FogPtr fog_{nullptr};
        LightingPtr lighting_{nullptr};

        std::string name_;
    };

    using ScenePtr = std::shared_ptr<Scene>;
} // namespace puffin

#endif // PUFFIN_SCENE_HPP