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
    std::string file_name, ScenePtr scene, CameraPtr camera) {
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
    //saveRenderSettings(render_settings);

    saveSkybox(scene);
    saveMeshes(scene);
    saveWaterTiles(scene);

    if (ini_file_.SaveFile(file_name.c_str()) < 0) {
        logError("SceneLoader::saveScene()", PUFFIN_MSG_FILE_SAVE_ERROR(file_name));
        return;
    }

    logInfo("SceneLoader::saveScene()", PUFFIN_MSG_FILE_SAVED(file_name));
}

void SceneLoader::loadScene(
    std::string file_name, ScenePtr scene, CameraPtr camera) {
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
    //loadRenderSettings(render_settings);

    loadSkybox(scene);
    loadMeshes(scene);
    loadWaterTiles(scene);

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

void SceneLoader::saveWaterTiles(ScenePtr scene) {
    auto count = scene->getWaterTilesCount();
    saveValue("water_tiles", "count", count);

    for (GLuint i = 0; i < count; i++) {
        auto water_tile = scene->getWaterTile(i);
        if (!water_tile) {
            continue;
        }

        std::string section = "water_tile_" + std::to_string(i);

        saveValue(section, "name", water_tile->name_);

        saveValue(section, "water_color", water_tile->water_color_);
        saveValue(section, "shininess", water_tile->shininess_);
        saveValue(section, "ambient_factor", water_tile->ambient_factor_);
        saveValue(section, "specular_factor", water_tile->specular_factor_);

        saveValue(section, "wave_strength", water_tile->wave_strength_);
        saveValue(section, "wave_speed", water_tile->wave_speed_);

        saveValue(section, "model_matrix", water_tile->model_matrix_);
        saveValue(section, "normal_matrix", water_tile->normal_matrix_);
        saveValue(section, "rotation_matrix", water_tile->rotation_matrix_);
        saveValue(section, "scale_matrix", water_tile->scale_matrix_);
        saveValue(section, "translation_matrix", water_tile->translation_matrix_);
        saveValue(section, "position", water_tile->position_);
        saveValue(section, "scale", water_tile->scale_);
    }
}

void SceneLoader::loadWaterTiles(ScenePtr scene) {
    GLuint count = 0;
    loadValue("water_tiles", "count", count);
    if (count < 1) {
        return;
    }

    for (GLuint i = 0; i < count; i++) {
        std::string section = "water_tile_" + std::to_string(i);

        std::string name;
        loadValue(section, "name", name);

        WaterTilePtr water_tile(new WaterTile(name));

        loadValue(section, "water_color", water_tile->water_color_);
        loadValue(section, "shininess", water_tile->shininess_);
        loadValue(section, "ambient_factor", water_tile->ambient_factor_);
        loadValue(section, "specular_factor", water_tile->specular_factor_);

        loadValue(section, "wave_strength", water_tile->wave_strength_);
        loadValue(section, "wave_speed", water_tile->wave_speed_);

        loadValue(section, "model_matrix", water_tile->model_matrix_);
        loadValue(section, "normal_matrix", water_tile->normal_matrix_);
        loadValue(section, "rotation_matrix", water_tile->rotation_matrix_);
        loadValue(section, "scale_matrix", water_tile->scale_matrix_);
        loadValue(section, "translation_matrix", water_tile->translation_matrix_);
        loadValue(section, "position", water_tile->position_);
        loadValue(section, "scale", water_tile->scale_);

        scene->addWaterTile(water_tile);
    }
}
/*
    void SceneLoader::saveRenderSettings(RenderSettingsPtr render_settings) {
    if (!render_settings) {
        saveValue("render_settings", "valid", 0);
        return;
    }

    saveValue("render_settings", "valid", 1);

    // Lighting
    auto lighting = render_settings->lighting();
    saveValue("lighting", "enabled", lighting->enabled_);
    saveValue("lighting", "blinn_phong_enabled", lighting->blinn_phong_enabled_);

    saveValue("lighting", "shadow_mapping_enabled", lighting->shadow_mapping_enabled_);
    saveValue("lighting", "directional_light_shadow_map_size",
        lighting->directional_light_shadow_map_size_);
    saveValue("lighting", "shadow_distance", lighting->shadow_distance_);
    saveValue("lighting", "emission_factor", lighting->emission_factor_);
    saveValue("lighting", "shadows_pcf_samples_count", lighting->shadows_pcf_samples_count_);

    saveValue("lighting", "skybox_light_color", lighting->skybox_light_color_);

    // Directional light
    saveValue("directional_light", "enabled", lighting->directional_light_->enabled_);
    saveValue("directional_light", "ambient_color", lighting->directional_light_->ambient_color_);
    saveValue("directional_light", "diffuse_color", lighting->directional_light_->diffuse_color_);
    saveValue("directional_light", "specular_color", lighting->directional_light_->specular_color_);
    saveValue("directional_light", "direction", lighting->directional_light_->direction_);

    // Fog
    auto fog = render_settings->fog();
    saveValue("fog", "enabled", fog->enabled_);

    saveValue("fog", "density", fog->density_);
    saveValue("fog", "color", fog->color_);

    saveValue("fog", "skybox_fog_overall_density", fog->skybox_fog_overall_density_);
    saveValue("fog", "skybox_fog_transition_power", fog->skybox_fog_transition_power_);
    saveValue("fog", "skybox_fog_height", fog->skybox_fog_height_);

    // Postprocess
    auto postprocess = render_settings->postprocess();
    saveValue("postprocess", "effect", static_cast<GLuint>(postprocess->effect_));

    saveValue("postprocess", "glow_bloom_enabled", postprocess->glow_bloom_enabled_);
    saveValue("postprocess", "glow_bloom_thresh_", postprocess->glow_bloom_thresh_);

    saveValue("postprocess", "tint_color", postprocess->tint_color_);
    saveValue("postprocess", "kernel_size", postprocess->kernel_size_);

    saveValue("postprocess", "dof_enabled", postprocess->dof_enabled_);
    saveValue("postprocess", "dof_max_blur", postprocess->dof_max_blur_);

    saveValue("postprocess", "aperture", postprocess->aperture_);
    saveValue("postprocess", "focus_distance", postprocess->focus_distance_);

    saveValue("postprocess", "gamma", postprocess->gamma_);
    saveValue("postprocess", "exposure", postprocess->exposure_);

    // Wireframe
    auto wireframe = render_settings->postprocess()->wireframe();
    saveValue("wireframe", "enabled", wireframe->enabled_);

    saveValue("wireframe", "mode", static_cast<GLuint>(wireframe->mode_));
    saveValue("wireframe", "color", wireframe->color_);
    saveValue("wireframe", "line_width", wireframe->line_width_);
}*/

/*void SceneLoader::loadRenderSettings(RenderSettingsPtr render_settings) {
    if (!render_settings) {
        return;
    }

    GLushort valid = 0;
    loadValue("render_settings", "valid", valid);
    if (!valid) {
        return;
    }

    // Lighting
    auto lighting = render_settings->lighting();
    loadValue("lighting", "enabled", lighting->enabled_);
    loadValue("lighting", "blinn_phong_enabled", lighting->blinn_phong_enabled_);

    loadValue("lighting", "shadow_mapping_enabled", lighting->shadow_mapping_enabled_);
    loadValue("lighting", "directional_light_shadow_map_size",
        lighting->directional_light_shadow_map_size_);
    loadValue("lighting", "shadow_distance", lighting->shadow_distance_);
    loadValue("lighting", "emission_factor", lighting->emission_factor_);
    loadValue("lighting", "shadows_pcf_samples_count", lighting->shadows_pcf_samples_count_);

    loadValue("lighting", "skybox_light_color", lighting->skybox_light_color_);

    // Directional light
    loadValue("directional_light", "enabled", lighting->directional_light_->enabled_);
    loadValue("directional_light", "ambient_color", lighting->directional_light_->ambient_color_);
    loadValue("directional_light", "diffuse_color", lighting->directional_light_->diffuse_color_);
    loadValue("directional_light", "specular_color", lighting->directional_light_->specular_color_);
    loadValue("directional_light", "direction", lighting->directional_light_->direction_);

    // Fog
    auto fog = render_settings->fog();
    loadValue("fog", "enabled", fog->enabled_);

    loadValue("fog", "density", fog->density_);
    loadValue("fog", "color", fog->color_);

    loadValue("fog", "skybox_fog_overall_density", fog->skybox_fog_overall_density_);
    loadValue("fog", "skybox_fog_transition_power", fog->skybox_fog_transition_power_);
    loadValue("fog", "skybox_fog_height", fog->skybox_fog_height_);

    // Postprocess
    auto postprocess = render_settings->postprocess();
    GLuint postprocess_effect = 0;
    loadValue("postprocess", "effect", postprocess_effect);
    postprocess->effect_ = static_cast<PostprocessEffect>(postprocess_effect);

    loadValue("postprocess", "glow_bloom_enabled", postprocess->glow_bloom_enabled_);
    loadValue("postprocess", "glow_bloom_thresh_", postprocess->glow_bloom_thresh_);

    loadValue("postprocess", "tint_color", postprocess->tint_color_);
    loadValue("postprocess", "kernel_size", postprocess->kernel_size_);

    loadValue("postprocess", "dof_enabled", postprocess->dof_enabled_);
    loadValue("postprocess", "dof_max_blur", postprocess->dof_max_blur_);

    loadValue("postprocess", "aperture", postprocess->aperture_);
    loadValue("postprocess", "focus_distance", postprocess->focus_distance_);

    loadValue("postprocess", "gamma", postprocess->gamma_);
    loadValue("postprocess", "exposure", postprocess->exposure_);

    // Wireframe
    auto wireframe = render_settings->postprocess()->wireframe();
    loadValue("wireframe", "enabled", wireframe->enabled_);

    GLuint wireframe_mode = 0;
    loadValue("wireframe", "mode", wireframe_mode);
    wireframe->mode_ = static_cast<WireframeMode>(wireframe_mode);

    loadValue("wireframe", "color", wireframe->color_);
    loadValue("wireframe", "line_width", wireframe->line_width_);
}*/