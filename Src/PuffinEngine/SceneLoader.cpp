/*
 * Puffin OpenGL Engine ver. 2.1
 * Coded by: Sebastian 'qbranchmaster' Tabaka
 * Contact: sebastian.tabaka@outlook.com
 */

#include "PuffinEngine/SceneLoader.hpp"

using namespace puffin;

SceneLoader::SceneLoader() {
    ini_file_.SetUnicode();
}

void SceneLoader::saveScene(std::string file_name, ScenePtr scene, CameraPtr camera) {
    if (!scene) {
        logError("SceneLoader::saveScene()", PUFFIN_MSG_NULL_OBJECT);
        return;
    }

    if (file_name.empty()) {
        logError("SceneLoader::saveScene()", PUFFIN_MSG_INVALID_VALUE);
        return;
    }

    file_name += ".psc";

    saveCameraSettings(camera);

    if (ini_file_.SaveFile(file_name.c_str()) < 0) {
        logError("SceneLoader::saveScene()", PUFFIN_MSG_FILE_SAVE_ERROR(file_name));
        return;
    }

    logInfo("SceneLoader::saveScene()", PUFFIN_MSG_FILE_SAVED(file_name));
}

void SceneLoader::loadScene(std::string file_name, ScenePtr scene, CameraPtr camera) {
    if (!scene) {
        logError("SceneLoader::loadScene()", PUFFIN_MSG_NULL_OBJECT);
        return;
    }

    if (file_name.empty()) {
        logError("SceneLoader::loadScene()", PUFFIN_MSG_INVALID_VALUE);
        return;
    }

    file_name += ".psc";

    if (ini_file_.LoadFile(file_name.c_str()) < 0) {
        logError("SceneLoader::loadScene()", PUFFIN_MSG_FILE_CANNOT_OPEN(file_name));
        return;
    }

    loadCameraSettings(camera);

    logInfo("SceneLoader::loadScene()", PUFFIN_MSG_FILE_LOADED(file_name));
}

void SceneLoader::saveCameraSettings(CameraPtr camera) {
    if (!camera) {
        return;
    }

    saveVec3(camera->getPosition(), "camera", "position");
}

void SceneLoader::loadCameraSettings(CameraPtr camera) {
    if (!camera) {
        return;
    }

    camera->setPosition(loadVec3("camera", "position"));
}

void SceneLoader::saveVec3(const glm::vec3 &vec, std::string section, std::string key) {
    std::stringstream ss;
    ss << vec.x << ", " << vec.y << ", " << vec.z;

    ini_file_.SetValue(section.c_str(), key.c_str(), ss.str().c_str());
}

glm::vec3 SceneLoader::loadVec3(std::string section, std::string key) {
    std::stringstream ss;
    ss << ini_file_.GetValue(section.c_str(), key.c_str());

    std::string val;
    GLfloat val_tab[3];

    for (GLushort i = 0; i < 3; i++) {
        std::getline(ss, val, ',');
        val_tab[i] = static_cast<GLfloat>(std::atof(val.c_str()));
    }

    return glm::vec3(val_tab[0], val_tab[1], val_tab[2]);
}