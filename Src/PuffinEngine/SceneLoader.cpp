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

void SceneLoader::saveScene(
    std::string file_name, ScenePtr scene, CameraPtr camera, RenderSettingsPtr render_settings) {
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
    saveRenderSettings(render_settings);

    saveSkybox(scene);
    saveMeshes(scene);

    if (ini_file_.SaveFile(file_name.c_str()) < 0) {
        logError("SceneLoader::saveScene()", PUFFIN_MSG_FILE_SAVE_ERROR(file_name));
        return;
    }

    logInfo("SceneLoader::saveScene()", PUFFIN_MSG_FILE_SAVED(file_name));
}

void SceneLoader::loadScene(
    std::string file_name, ScenePtr scene, CameraPtr camera, RenderSettingsPtr render_settings) {
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

    scene->reset();

    loadCameraSettings(camera);
    loadRenderSettings(render_settings);

    loadSkybox(scene);
    loadMeshes(scene);

    logInfo("SceneLoader::loadScene()", PUFFIN_MSG_FILE_LOADED(file_name));
}

void SceneLoader::saveCameraSettings(CameraPtr camera) {
    if (!camera) {
        saveInt("camera", "state", 0); // State 0 - values not saved to file
        return;
    }

    saveInt("camera", "state", 1); // State 1 - values saved to file
	saveObjectData("camera", "data", camera);
}

void SceneLoader::loadCameraSettings(CameraPtr camera) {
    if (!camera) {
        return;
    }

    // Check state, 0 means that values were not saved to file
    if (loadInt("camera", "state") == 0) {
        return;
    }

    loadObjectData("camera", "data", camera);
}

void SceneLoader::saveSkybox(ScenePtr scene) {
    auto skybox = scene->getSkybox();
    if (!skybox) {
        saveInt("skybox", "count", 0);
        return;
    }

    saveInt("skybox", "count", 1);
    saveObjectData("skybox", "data", skybox);
}

void SceneLoader::loadSkybox(ScenePtr scene) {
    auto count = loadInt("skybox", "count");
    if (count < 1) {
        return;
    }
}

void SceneLoader::saveMeshes(ScenePtr scene) {
    auto meshes_count = scene->getMeshesCount();
    saveInt("meshes", "count", meshes_count);
    if (meshes_count < 1) {
        return;
    }
}

void SceneLoader::loadMeshes(ScenePtr scene) {
    auto meshes_count = loadInt("meshes", "count");
    if (meshes_count < 1) {
        return;
    }
}

void SceneLoader::saveRenderSettings(RenderSettingsPtr render_settings) {
    if (!render_settings) {
        saveInt("render_settings", "state", 0);
        return;
    }

    saveInt("render_settings", "state", 1);
}

void SceneLoader::loadRenderSettings(RenderSettingsPtr render_settings) {
    if (!render_settings) {
        return;
    }

    if (loadInt("render_settings", "state") == 0) {
        return;
    }
}

void SceneLoader::saveFloat(std::string section, std::string key, GLfloat value) {
    ini_file_.SetDoubleValue(section.c_str(), key.c_str(), value);
}

GLfloat SceneLoader::loadFloat(std::string section, std::string key) {
    GLdouble value = ini_file_.GetDoubleValue(section.c_str(), key.c_str());
    return static_cast<GLfloat>(value);
}

void SceneLoader::saveInt(std::string section, std::string key, GLint value) {
    ini_file_.SetLongValue(section.c_str(), key.c_str(), value);
}

GLint SceneLoader::loadInt(std::string section, std::string key) {
    GLint value = ini_file_.GetLongValue(section.c_str(), key.c_str());
    return value;
}

void SceneLoader::saveString(std::string section, std::string key, std::string value) {
    ini_file_.SetValue(section.c_str(), key.c_str(), value.c_str());
}

std::string SceneLoader::loadString(std::string section, std::string key) {
    return std::string(ini_file_.GetValue(section.c_str(), key.c_str()));
}