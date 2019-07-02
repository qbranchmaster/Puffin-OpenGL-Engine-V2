/*
 * Puffin OpenGL Engine ver. 2.1
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
		Scene() {
            camera_.reset(new Camera());
            fog_.reset(new Fog());
            lighting_.reset(new Lighting());
		}

        void reset() {
            meshes_.clear();
            textes_.clear();
            water_tiles_.clear();
            active_skybox_ = nullptr;
        }

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
                return nullptr;
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
                return nullptr;
            }

            return textes_[index];
        }

        void addWaterTile(WaterTilePtr water_tile) {
            if (!water_tile) {
                logError("Scene::addWaterTile()", PUFFIN_MSG_NULL_OBJECT);
                return;
            }

            water_tiles_.push_back(water_tile);
        }

        GLuint getWaterTilesCount() const {
            return water_tiles_.size();
        }

        WaterTilePtr getWaterTile(GLuint index) {
            if (index >= water_tiles_.size()) {
                logError("Scene::getWaterTile()", PUFFIN_MSG_OUT_OF_RANGE(0, water_tiles_.size()));
                return nullptr;
            }

            return water_tiles_[index];
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

    private:
        std::vector<MeshPtr> meshes_;
        std::vector<TextPtr> textes_;
        std::vector<WaterTilePtr> water_tiles_;
        SkyboxPtr active_skybox_{nullptr};

		CameraPtr camera_{nullptr};
		FogPtr fog_{nullptr};
        LightingPtr lighting_{nullptr};
    };

    using ScenePtr = std::shared_ptr<Scene>;
} // namespace puffin

#endif // PUFFIN_SCENE_HPP