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
    saveSkybox(scene);
    saveRenderSettings(render_settings);

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

    loadCameraSettings(camera);
    loadSkybox(scene);
    loadRenderSettings(render_settings);

    logInfo("SceneLoader::loadScene()", PUFFIN_MSG_FILE_LOADED(file_name));
}

void SceneLoader::saveCameraSettings(CameraPtr camera) {
    if (!camera) {
        return;
    }

    saveFloat(camera->getAspect(), "camera", "aspect");
    saveFloat(camera->getFov(), "camera", "fov");
    saveFloat(camera->getNearPlane(), "camera", "near_plane");
    saveFloat(camera->getFarPlane(), "camera", "far_plane");

    saveFloat(camera->getHorizontalAngle(), "camera", "hor_angle");
    saveFloat(camera->getVerticalAngle(), "camera", "ver_angle");

    saveFloat(camera->getMoveSpeed(), "camera", "move_speed");
    saveFloat(camera->getMoveResistanceFactor(), "camera", "move_res_factor");

    saveVec3(camera->getPosition(), "camera", "position");
}

void SceneLoader::loadCameraSettings(CameraPtr camera) {
    if (!camera) {
        return;
    }

    auto aspect = loadFloat("camera", "aspect");
    auto fov = loadFloat("camera", "fov");
    auto near_plane = loadFloat("camera", "near_plane");
    auto far_plane = loadFloat("camera", "far_plane");
    camera->setProjection(fov, aspect, near_plane, far_plane);

    auto hor_angle = loadFloat("camera", "hor_angle");
    auto ver_angle = loadFloat("camera", "ver_angle");
    camera->setRotation(hor_angle, ver_angle);

    camera->setMoveSpeed(loadFloat("camera", "move_speed"));
    camera->setMoveResistanceFactor(loadFloat("camera", "move_res_factor"));

    camera->setPosition(loadVec3("camera", "position"));
}

void SceneLoader::saveSkybox(ScenePtr scene) {
    auto skybox = scene->getSkybox();
    if (!skybox) {
        saveInt(0, "skybox", "count");
        return;
    }

    saveInt(1, "skybox", "count");

    saveString(skybox->getName(), "skybox", "name");

    auto cubemap_path = skybox->getTexture()->getCubemapPath();
    for (GLushort i = 0; i < cubemap_path.size(); i++) {
        saveString(cubemap_path[i], "skybox", "texture_path_" + std::to_string(i));
    }
}

void SceneLoader::loadSkybox(ScenePtr scene) {
    auto count = loadInt("skybox", "count");
    if (count < 1) {
        return;
    }

    SkyboxPtr skybox(new Skybox(loadString("skybox", "name")));

    std::array<std::string, 6> paths;
    for (GLushort i = 0; i < paths.size(); i++) {
        auto path = loadString("skybox", "texture_path_" + std::to_string(i));
        paths[i] = path;
    }

    TexturePtr texture(new Texture());
    texture->loadTextureCube(paths);
    skybox->setTexture(texture);

    scene->setSkybox(skybox);
}

void SceneLoader::saveRenderSettings(RenderSettingsPtr render_settings) {
    if (!render_settings) {
        return;
	}
}

void SceneLoader::loadRenderSettings(RenderSettingsPtr render_settings) {
    if (!render_settings) {
        return;
    }
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

void SceneLoader::saveFloat(GLfloat value, std::string section, std::string key) {
    ini_file_.SetDoubleValue(section.c_str(), key.c_str(), value);
}

GLfloat SceneLoader::loadFloat(std::string section, std::string key) {
    GLdouble value = ini_file_.GetDoubleValue(section.c_str(), key.c_str());
    return static_cast<GLfloat>(value);
}

void SceneLoader::saveInt(GLint value, std::string section, std::string key) {
    ini_file_.SetLongValue(section.c_str(), key.c_str(), value);
}

GLint SceneLoader::loadInt(std::string section, std::string key) {
    GLint value = ini_file_.GetLongValue(section.c_str(), key.c_str());
    return value;
}

void SceneLoader::saveString(std::string value, std::string section, std::string key) {
    ini_file_.SetValue(section.c_str(), key.c_str(), value.c_str());
}

std::string SceneLoader::loadString(std::string section, std::string key) {
    return std::string(ini_file_.GetValue(section.c_str(), key.c_str()));
}