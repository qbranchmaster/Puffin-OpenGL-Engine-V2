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
#include <glm/gtc/type_ptr.hpp>

#include <memory>
#include <sstream>
#include <string>

#include "PuffinEngine/Camera.hpp"
#include "PuffinEngine/CommonUtils.hpp"
#include "PuffinEngine/Logger.hpp"
#include "PuffinEngine/RenderSettings.hpp"
#include "PuffinEngine/Scene.hpp"

namespace puffin {
    class SceneLoader {
    public:
        SceneLoader();

        void saveScene(std::string file_name, ScenePtr scene, CameraPtr camera = nullptr,
            RenderSettingsPtr render_settings = nullptr);
        void loadScene(std::string file_name, ScenePtr scene, CameraPtr camera = nullptr,
            RenderSettingsPtr render_settings = nullptr);

    private:
        template<typename T>
        void saveValue(std::string section, std::string key, const T &obj);
        template<typename T>
        void loadValue(std::string section, std::string key, T &obj);

        void saveCameraSettings(CameraPtr camera);
        void loadCameraSettings(CameraPtr camera);

        void saveSkybox(ScenePtr scene);
        void loadSkybox(ScenePtr scene);

        void saveMeshes(ScenePtr scene);
        void loadMeshes(ScenePtr scene);

        void saveRenderSettings(RenderSettingsPtr render_settings);
        void loadRenderSettings(RenderSettingsPtr render_settings);

        CSimpleIniA ini_file_;
    };

    using SceneLoaderPtr = std::shared_ptr<SceneLoader>;

    template<typename T>
    inline void SceneLoader::saveValue(std::string section, std::string key, const T &obj) {
        std::stringstream ss;
        ss << obj;
        ini_file_.SetValue(section.c_str(), key.c_str(), ss.str().c_str());
    }

    template<typename T>
    inline void SceneLoader::loadValue(std::string section, std::string key, T &obj) {
        std::stringstream ss;
        ss << ini_file_.GetValue(section.c_str(), key.c_str(), "0");
        ss >> obj;
    }
} // namespace puffin

#endif // PUFFIN_SCENE_LOADER_HPP