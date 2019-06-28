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
        saveValue("camera", "valid", 0);
        return;
    }

    saveValue("camera", "valid", 1);

    saveValue("camera", "horizontal_angle", camera->horizontal_angle_);
    saveValue("camera", "vertical_angle", camera->vertical_angle_);

    saveValue("camera", "projection_matrix", camera->projection_matrix_);
    saveValue("camera", "projection_matrix_inverted", camera->projection_matrix_inverted_);
    saveValue("camera", "view_matrix", camera->view_matrix_);
    saveValue("camera", "view_matrix_static", camera->view_matrix_static_);
    saveValue("camera", "view_matrix_inverted", camera->view_matrix_inverted_);

    saveValue("camera", "rotation_matrix", camera->rotation_matrix_);
    saveValue("camera", "rotation_matrix_inverted", camera->rotation_matrix_inverted_);

    saveValue("camera", "position", camera->position_);
    saveValue("camera", "direction", camera->direction_);
    saveValue("camera", "right", camera->right_);
    saveValue("camera", "up", camera->up_);

    saveValue("camera", "aspect", camera->aspect_);
    saveValue("camera", "near_plane", camera->near_plane_);
    saveValue("camera", "far_plane", camera->far_plane_);
    saveValue("camera", "fov", camera->fov_);

    saveValue("camera", "camera_move_speed", camera->camera_move_speed_);
    saveValue("camera", "move_resistance_factor", camera->move_resistance_factor_);
}

void SceneLoader::loadCameraSettings(CameraPtr camera) {
    if (!camera) {
        return;
    }

    GLushort valid = 0;
    loadValue("camera", "valid", valid);
    if (!valid) {
        return;
    }

    loadValue("camera", "horizontal_angle", camera->horizontal_angle_);
    loadValue("camera", "vertical_angle", camera->vertical_angle_);

    loadValue("camera", "projection_matrix", camera->projection_matrix_);
    loadValue("camera", "projection_matrix_inverted", camera->projection_matrix_inverted_);
    loadValue("camera", "view_matrix", camera->view_matrix_);
    loadValue("camera", "view_matrix_static", camera->view_matrix_static_);
    loadValue("camera", "view_matrix_inverted", camera->view_matrix_inverted_);

    loadValue("camera", "rotation_matrix", camera->rotation_matrix_);
    loadValue("camera", "rotation_matrix_inverted", camera->rotation_matrix_inverted_);

    loadValue("camera", "position", camera->position_);
    loadValue("camera", "direction", camera->direction_);
    loadValue("camera", "right", camera->right_);
    loadValue("camera", "up", camera->up_);

    loadValue("camera", "aspect", camera->aspect_);
    loadValue("camera", "near_plane", camera->near_plane_);
    loadValue("camera", "far_plane", camera->far_plane_);
    loadValue("camera", "fov", camera->fov_);

    loadValue("camera", "camera_move_speed", camera->camera_move_speed_);
    loadValue("camera", "move_resistance_factor", camera->move_resistance_factor_);
}

void SceneLoader::saveSkybox(ScenePtr scene) {
    auto skybox = scene->getSkybox();
    if (!skybox) {
        saveValue("skybox", "count", 0);
        return;
    }

    saveValue("skybox", "count", 1);

    saveValue("skybox", "name", skybox->getName());
    auto textures = skybox->getTexture()->getCubemapPath();
    for (GLushort i = 0; i < textures.size(); i++) {
        saveValue("skybox", "texture_" + std::to_string(i), textures[i]);
    }
}

void SceneLoader::loadSkybox(ScenePtr scene) {
    GLushort count = 0;
    loadValue("skybox", "count", count);
    if (count < 1) {
        return;
    }

    std::string name;
    loadValue("skybox", "name", name);

    SkyboxPtr skybox(new Skybox(name));
    TexturePtr texture(new Texture());
    std::array<std::string, 6> paths;
    for (GLushort i = 0; i < 6; i++) {
        loadValue("skybox", "texture_" + std::to_string(i), paths[i]);
    }

    texture->loadTextureCube(paths);
    skybox->setTexture(texture);
    scene->setSkybox(skybox);
}

void SceneLoader::saveMeshes(ScenePtr scene) {
    auto count = scene->getMeshesCount();
    saveValue("meshes", "count", count);

    for (GLuint i = 0; i < count; i++) {
        auto mesh = scene->getMesh(i);
        if (!mesh) {
            continue;
        }

        std::string section = "mesh_" + std::to_string(i);

        saveValue(section, "name", mesh->name_);
        saveValue(section, "path", mesh->path_);

        saveValue(section, "shadow_cast_enabled", mesh->shadow_cast_enabled_);

        saveValue(section, "model_matrix", mesh->model_matrix_);
        saveValue(section, "normal_matrix", mesh->normal_matrix_);
        saveValue(section, "rotation_matrix", mesh->rotation_matrix_);
        saveValue(section, "scale_matrix", mesh->scale_matrix_);
        saveValue(section, "translation_matrix", mesh->translation_matrix_);
        saveValue(section, "position", mesh->position_);
        saveValue(section, "scale", mesh->scale_);
    }
}

void SceneLoader::loadMeshes(ScenePtr scene) {
    GLuint count = 0;
    loadValue("meshes", "count", count);
    if (count < 1) {
        return;
    }

    for (GLuint i = 0; i < count; i++) {
        std::string section = "mesh_" + std::to_string(i);

        std::string name, path;
        loadValue(section, "name", name);
        loadValue(section, "path", path);

        MeshPtr mesh(new Mesh(name));
        mesh->loadFromFile(path);

        loadValue(section, "shadow_cast_enabled", mesh->shadow_cast_enabled_);

        loadValue(section, "model_matrix", mesh->model_matrix_);
        loadValue(section, "normal_matrix", mesh->normal_matrix_);
        loadValue(section, "rotation_matrix", mesh->rotation_matrix_);
        loadValue(section, "scale_matrix", mesh->scale_matrix_);
        loadValue(section, "translation_matrix", mesh->translation_matrix_);
        loadValue(section, "position", mesh->position_);
        loadValue(section, "scale", mesh->scale_);

        scene->addMesh(mesh);
    }
}

void SceneLoader::saveRenderSettings(RenderSettingsPtr render_settings) {}

void SceneLoader::loadRenderSettings(RenderSettingsPtr render_settings) {}