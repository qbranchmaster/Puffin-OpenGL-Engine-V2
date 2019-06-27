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
        void saveObjectData(std::string section, std::string key, const T &obj);
        template<typename T>
        void loadObjectData(std::string section, std::string key, T &obj);

        void saveFloat(std::string section, std::string key, GLfloat value);
        GLfloat loadFloat(std::string section, std::string key);

        void saveInt(std::string section, std::string key, GLint value);
        GLint loadInt(std::string section, std::string key);

        void saveString(std::string section, std::string key, std::string value);
        std::string loadString(std::string section, std::string key);

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
    inline void SceneLoader::saveObjectData(std::string section, std::string key, const T &obj) {
        std::stringstream ss;
        ss << obj;
        saveString(section, key, ss.str());
    }

    template<typename T>
    inline void SceneLoader::loadObjectData(std::string section, std::string key, T &obj) {
        std::stringstream ss;
        ss << loadString(section, key);
        ss >> obj;
    }
} // namespace puffin

#endif // PUFFIN_SCENE_LOADER_HPP