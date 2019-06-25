/*
 * Puffin OpenGL Engine ver. 2.1
 * Coded by: Sebastian 'qbranchmaster' Tabaka
 * Contact: sebastian.tabaka@outlook.com
 */

#ifndef PUFFIN_SCENE_LOADER_HPP
#define PUFFIN_SCENE_LOADER_HPP

#include <GL/glew.h>

#include <SimpleIni.h>

#include <glm/glm.hpp>

#include <memory>
#include <sstream>
#include <string>

#include "PuffinEngine/Camera.hpp"
#include "PuffinEngine/Logger.hpp"
#include "PuffinEngine/Scene.hpp"

namespace puffin {
    class SceneLoader {
    public:
        SceneLoader();

        void saveScene(std::string file_name, ScenePtr scene, CameraPtr camera = nullptr);
        void loadScene(std::string file_name, ScenePtr scene, CameraPtr camera = nullptr);

    private:
        void saveCameraSettings(CameraPtr camera);
        void loadCameraSettings(CameraPtr camera);

        void saveVec3(const glm::vec3 &vec, std::string section, std::string key);
        glm::vec3 loadVec3(std::string section, std::string key);

        CSimpleIniA ini_file_;
    };

    using SceneLoaderPtr = std::shared_ptr<SceneLoader>;
} // namespace puffin

#endif // PUFFIN_SCENE_LOADER_HPP