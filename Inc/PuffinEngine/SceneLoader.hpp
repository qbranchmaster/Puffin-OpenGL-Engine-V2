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
        void saveCameraSettings(CameraPtr camera);
        void loadCameraSettings(CameraPtr camera);

        void saveSkybox(ScenePtr scene);
        void loadSkybox(ScenePtr scene);

        void saveRenderSettings(RenderSettingsPtr render_settings);
        void loadRenderSettings(RenderSettingsPtr render_settings);

        void saveVec3(const glm::vec3 &vec, std::string section, std::string key);
        glm::vec3 loadVec3(std::string section, std::string key);

        void saveFloat(GLfloat value, std::string section, std::string key);
        GLfloat loadFloat(std::string section, std::string key);

        void saveInt(GLint value, std::string section, std::string key);
        GLint loadInt(std::string section, std::string key);

        void saveString(std::string value, std::string section, std::string key);
        std::string loadString(std::string section, std::string key);

        CSimpleIniA ini_file_;
    };

    using SceneLoaderPtr = std::shared_ptr<SceneLoader>;
} // namespace puffin

#endif // PUFFIN_SCENE_LOADER_HPP