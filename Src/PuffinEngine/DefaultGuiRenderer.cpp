/*
 * Puffin OpenGL Engine ver. 2.0.1
 * Coded by: Sebastian 'qbranchmaster' Tabaka
 * Contact: sebastian.tabaka@outlook.com
 */

#include "PuffinEngine/DefaultGuiRenderer.hpp"

using namespace puffin;

DefaultGuiRenderer::DefaultGuiRenderer(WindowPtr window, PostprocessPtr postprocess,
    MeshRendererPtr mesh_renderer, SkyboxRendererPtr skybox_renderer,
    ShadowMapRendererPtr shadow_map_renderer, WaterRendererPtr water_renderer,
    GizmoRendererPtr gizmo_renderer) {
    if (!window || !postprocess || !mesh_renderer || !skybox_renderer || !shadow_map_renderer ||
        !water_renderer || !gizmo_renderer) {
        throw Exception("DefaultGuiRenderer::DefaultGuiRenderer()", PUFFIN_MSG_NULL_OBJECT);
    }

    target_window_ = window;
    postprocess_ = postprocess;

    mesh_renderer_ = mesh_renderer;
    skybox_renderer_ = skybox_renderer;
    shadow_map_renderer_ = shadow_map_renderer;
    water_renderer_ = water_renderer;
    gizmo_renderer_ = gizmo_renderer;

    scene_loader_.reset(new SceneLoader());

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    setupImGui();

    const char *glsl_version = "#version 330";
    ImGui_ImplGlfw_InitForOpenGL(target_window_->handle_, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
}

void DefaultGuiRenderer::setupImGui() {
    ImGuiIO &io = ImGui::GetIO();

    // Setup style
    ImGuiStyle &style = ImGui::GetStyle();
    style.FrameRounding = 5.0f;
    style.WindowPadding = ImVec2(8, 8);
    style.FramePadding = ImVec2(4, 3);
    style.ItemSpacing = ImVec2(6, 4);
    style.ItemInnerSpacing = ImVec2(4, 4);

    style.IndentSpacing = 16;
    style.ScrollbarSize = 12;
    style.GrabMinSize = 12;

    style.WindowBorderSize = 1.0f;
    style.ChildBorderSize = 1.0f;
    style.PopupBorderSize = 1.0f;
    style.FrameBorderSize = 0.0f;
    style.TabBorderSize = 0.0f;

    style.WindowRounding = 5.0f;
    style.ChildRounding = 0.0f;
    style.FrameRounding = 5.0f;
    style.PopupRounding = 0.0f;
    style.GrabRounding = 5.0f;
    style.TabRounding = 5.0f;

    style.WindowTitleAlign = ImVec2(0.0f, 0.50f);
    style.ButtonTextAlign = ImVec2(0.50f, 0.50f);
    style.SelectableTextAlign = ImVec2(0.0f, 0.0f);

    style.DisplaySafeAreaPadding = ImVec2(3, 3);

    style.AntiAliasedLines = true;
    style.AntiAliasedFill = true;

    font_normal_ = io.Fonts->AddFontFromFileTTF("Data/Fonts/Roboto-Regular.ttf", 16);
    font_bold_ = io.Fonts->AddFontFromFileTTF("Data/Fonts/Roboto-Black.ttf", 16);

    // ImGui::StyleColorsLight();
    // ImGui::StyleColorsClassic();
    ImGui::StyleColorsDark();
}

GLboolean DefaultGuiRenderer::isCapturingMouse() const {
    return static_cast<GLboolean>(ImGui::GetIO().WantCaptureMouse);
}

GLboolean DefaultGuiRenderer::isCapturingKeyboard() const {
    return static_cast<GLboolean>(ImGui::GetIO().WantCaptureKeyboard);
}

void DefaultGuiRenderer::render(ScenePtr scene) {
    if (!scene) {
        logError("DefaultGuiRenderer::render()", PUFFIN_MSG_NULL_OBJECT);
        return;
    }

    if (!enabled_) {
        return;
    }

    current_scene_ = scene;

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    renderMainMenuBar();

    renderAboutDialog();
    renderCameraDialog();
    renderObjectInspectorDialog();
    renderFogDialog();
    renderPostprocessDialog();
    renderCaptureDialog();
    renderRenderersDialog();
    renderLightingDialog();
    renderShadowMappingDialog();
    renderLoadSceneDialog();
    renderSaveSceneDialog();

    renderAddSkyboxDialog();
    renderAddMeshDialog();
    renderAddWaterTileDialog();

    // ImGui::ShowDemoWindow();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void DefaultGuiRenderer::bindQuitFunction(std::function<void()> function) {
    quit_function_ = function;
}

void DefaultGuiRenderer::bindSaveCapture(std::function<void(std::string, bool)> function) {
    save_capture_function_ = function;
}

void DefaultGuiRenderer::renderMainMenuBar() {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            ImGui::MenuItem("Load scene", NULL, &render_load_scene_dialog_);
            ImGui::MenuItem("Save scene", NULL, &render_save_scene_dialog_);
            if (ImGui::MenuItem("Quit")) {
                if (quit_function_) {
                    quit_function_();
                }
            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Add")) {
            ImGui::MenuItem("Skybox", NULL, &render_add_skybox_dialog_);
            ImGui::MenuItem("Mesh", NULL, &render_add_mesh_dialog_);
            ImGui::MenuItem("Water tile", NULL, &render_add_water_tile_dialog_);

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Window")) {
            ImGui::MenuItem("Object inspector", NULL, &render_object_inspector_dialog_);
            ImGui::MenuItem("Camera", NULL, &render_camera_dialog_);
            ImGui::MenuItem("Lighting", NULL, &render_lighting_dialog_);
            ImGui::MenuItem("Shadow mapping", NULL, &render_shadow_map_dialog_);
            ImGui::Separator();
            ImGui::MenuItem("Fog", NULL, &render_fog_dialog_);
            ImGui::MenuItem("Postprocess", NULL, &render_postprocess_dialog_);
            ImGui::Separator();
            ImGui::MenuItem("Renderers", NULL, &render_renderers_dialog_);
            ImGui::MenuItem("Screen capture", NULL, &render_capture_dialog_);
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Help")) {
            ImGui::MenuItem("About Puffin Engine", NULL, &render_about_dialog_);
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
}

void DefaultGuiRenderer::renderAboutDialog() {
    if (!render_about_dialog_) {
        return;
    }

    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_NoCollapse;
    window_flags |= ImGuiWindowFlags_NoResize;

    ImGui::SetNextWindowSize(ImVec2(465, 120), ImGuiCond_Always);

    if (!ImGui::Begin("About Puffin Engine", &render_about_dialog_, window_flags)) {
        ImGui::End();
        return;
    }

    ImGui::PushFont(font_bold_);
    ImGui::Text("Puffin OpenGL Engine ver. 2.0");
    ImGui::PopFont();
    ImGui::Separator();
    ImGui::Text("Coded by: Sebastian 'qbranchmaster' Tabaka");
    ImGui::Text("Contact: sebastian.tabaka@outlook.com");
    ImGui::Text("Repo URL:");
    ImGui::SameLine();
    std::string url = "https://github.com/qbranchmaster/Puffin-OpenGL-Engine-V2";
    ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), url.c_str());
    if (ImGui::IsItemClicked()) {
#ifdef WIN32
        ShellExecute(0, 0, url.c_str(), 0, 0, SW_SHOW);
#endif // WIN32
    }

    ImGui::End();
}

void DefaultGuiRenderer::renderCameraDialog() {
    if (!render_camera_dialog_) {
        return;
    }

    ImGuiWindowFlags window_flags = 0;
    // window_flags |= ImGuiWindowFlags_NoResize;
    ImGui::SetNextWindowSize(ImVec2(360, 220), ImGuiCond_FirstUseEver);

    if (!ImGui::Begin("Camera", &render_camera_dialog_, window_flags)) {
        ImGui::End();
        return;
    }

    auto camera = current_scene_->camera();

    auto cp = camera->getPosition();
    float pos[] = {cp.x, cp.y, cp.z};
    ImGui::DragFloat3("Position", pos, 0.01f);
    camera->setPosition(glm::vec3(pos[0], pos[1], pos[2]));

    float rot[] = {camera->getHorizontalAngle(), camera->getVerticalAngle()};
    ImGui::DragFloat2("Rotation", rot, 0.01f);
    camera->setRotation(rot[0], rot[1]);

    auto direction = camera->getDirectionVector();
    std::string dir_str = "Direction: [" + std::to_string(direction.x) + ", " +
        std::to_string(direction.y) + ", " + std::to_string(direction.z) + "]";
    ImGui::Text(dir_str.c_str());

    auto near_plane = camera->getNearPlane();
    ImGui::DragFloat("Near plane", &near_plane, 0.01f);
    auto far_plane = camera->getFarPlane();
    ImGui::DragFloat("Far plane", &far_plane, 0.01f);
    auto fov = camera->getFov();
    ImGui::SliderFloat("FOV", &fov, 0.01f, 2.65f);
    auto aspect = camera->getAspect();
    ImGui::DragFloat("Aspect", &aspect, 0.01f);
    camera->setProjection(fov, aspect, near_plane, far_plane);

    ImGui::End();
}

void DefaultGuiRenderer::renderObjectInspectorMeshPart(BaseMeshPtr object) {
    renderObjectInspectorTransformPart(object);

    ImGui::PushFont(font_bold_);
    ImGui::Text("Render");
    ImGui::PopFont();

    MeshPtr mesh = std::static_pointer_cast<Mesh>(object);

    bool cast_shadow = mesh->isShadowCastingEnabled();
    ImGui::Checkbox("Cast shadows", &cast_shadow);
    mesh->enableShadowCasting(cast_shadow);
}

void DefaultGuiRenderer::renderObjectInspectorTransformPart(BaseMeshPtr object) {
    ImGui::PushFont(font_bold_);
    ImGui::Text("Transform");
    ImGui::PopFont();

    auto position = object->getPosition();
    float pos[] = {position.x, position.y, position.z};
    ImGui::DragFloat3("Position", pos, 0.01f);
    object->setPosition(glm::vec3(pos[0], pos[1], pos[2]));

    static bool lock_scale = false;
    auto scale = object->getScale();
    static float prev_scale[] = {scale.x, scale.y, scale.z};
    float current_scale[] = {scale.x, scale.y, scale.z};
    if (ImGui::DragFloat3("Scale", current_scale, 0.0001f)) {
        if (lock_scale) {
            // Detect which value has changed
            for (GLushort i = 0; i < 3; i++) {
                if (current_scale[i] != prev_scale[i]) {
                    float new_val = current_scale[i];
                    for (GLushort j = 0; j < 3; j++) {
                        current_scale[j] = new_val;
                        prev_scale[j] = new_val;
                    }

                    break;
                }
            }
        }
    }

    ImGui::SameLine();
    if (ImGui::Checkbox("Lock", &lock_scale)) {
        if (lock_scale) {
            current_scale[1] = current_scale[0];
            current_scale[2] = current_scale[0];
        }
    }

    object->setScale(glm::vec3(current_scale[0], current_scale[1], current_scale[2]));

    auto angles = object->getRotationAngles();
    float rot_an[] = {angles.x, angles.y, angles.z};
    ImGui::DragFloat3("Rotation", rot_an, 0.001f);
    object->setRotation(glm::vec3(rot_an[0], rot_an[1], rot_an[2]));
}

void DefaultGuiRenderer::renderObjectInspectorWaterTilePart(BaseMeshPtr object) {
    renderObjectInspectorTransformPart(object);

    ImGui::PushFont(font_bold_);
    ImGui::Text("Render");
    ImGui::PopFont();

    WaterTilePtr water_tile = std::static_pointer_cast<WaterTile>(object);

    auto water_color = water_tile->getWaterColor();
    float color[] = {water_color.r, water_color.g, water_color.b};
    ImGui::ColorEdit3("Color", color);
    water_tile->setWaterColor(glm::vec3(color[0], color[1], color[2]));

    auto wave_strength = water_tile->getWaveStrength();
    ImGui::SliderFloat("Wave strength", &wave_strength, 0.0f, 10.0f);
    water_tile->setWaveStrength(wave_strength);

    auto wave_speed = water_tile->getWaveSpeed();
    ImGui::SliderFloat("Wave speed", &wave_speed, 0.0f, 10.0f);
    water_tile->setWaveSpeed(wave_speed);

    auto shininess = water_tile->getShininess();
    ImGui::SliderInt("Shininess", &shininess, 1, 100);
    water_tile->setShininess(shininess);

    auto ambient_factor = water_tile->getAmbientLightingFactor();
    ImGui::SliderFloat("Ambient factor", &ambient_factor, 0.0f, 10.0f);
    water_tile->setAmbientLightingFactor(ambient_factor);

    auto specular_factor = water_tile->getSpecularLightingFactor();
    ImGui::SliderFloat("Specular factor", &specular_factor, 0.0f, 10.0f);
    water_tile->setSpecularLightingFactor(specular_factor);
}

void DefaultGuiRenderer::renderAddSkyboxDialog() {
    if (!render_add_skybox_dialog_) {
        return;
    }

    ImGuiWindowFlags window_flags = 0;
    // window_flags |= ImGuiWindowFlags_NoResize;
    ImGui::SetNextWindowSize(ImVec2(410, 265), ImGuiCond_FirstUseEver);

    if (!ImGui::Begin("Add skybox", &render_add_skybox_dialog_, window_flags)) {
        ImGui::End();
        return;
    }

    ImGuiInputTextFlags flags = 0;

    static std::string name = "new_skybox";
    ImGui::InputText("Name", &name, flags);

    ImGui::Text("Skybox textures");
    static std::array<std::string, 6> textures;
    ImGui::InputText("Right", &textures[0], flags);
    ImGui::InputText("Left", &textures[1], flags);
    ImGui::InputText("Up", &textures[2], flags);
    ImGui::InputText("Down", &textures[3], flags);
    ImGui::InputText("Back", &textures[4], flags);
    ImGui::InputText("Front", &textures[5], flags);

    static bool set_active = false;
    ImGui::Checkbox("Set active", &set_active);

    if (ImGui::Button("Add")) {
        auto skybox = current_scene_->addSkybox(name);
        if (skybox) {
            TexturePtr texture(new Texture());
            texture->loadTextureCube(textures);
            skybox->setTexture(texture);

            if (set_active) {
                current_scene_->setActiveSkybox(skybox);
            }
        }
    }

    ImGui::End();
}

void DefaultGuiRenderer::renderAddMeshDialog() {
    if (!render_add_mesh_dialog_) {
        return;
    }

    ImGuiWindowFlags window_flags = 0;
    // window_flags |= ImGuiWindowFlags_NoResize;
    ImGui::SetNextWindowSize(ImVec2(405, 115), ImGuiCond_FirstUseEver);

    if (!ImGui::Begin("Add mesh", &render_add_mesh_dialog_, window_flags)) {
        ImGui::End();
        return;
    }

    ImGuiInputTextFlags flags = 0;

    static std::string name = "new_mesh";
    ImGui::InputText("Name", &name, flags);

    static std::string path = "";
    ImGui::InputText("Path", &path, flags);

    if (ImGui::Button("Add")) {
        auto mesh = current_scene_->addMesh(name);
        if (mesh) {
            mesh->loadFromFile(path);
        }
    }

    ImGui::End();
}

void DefaultGuiRenderer::renderAddWaterTileDialog() {
    if (!render_add_water_tile_dialog_) {
        return;
    }

    ImGuiWindowFlags window_flags = 0;
    // window_flags |= ImGuiWindowFlags_NoResize;
    ImGui::SetNextWindowSize(ImVec2(390, 80), ImGuiCond_FirstUseEver);

    if (!ImGui::Begin("Add water tile", &render_add_water_tile_dialog_, window_flags)) {
        ImGui::End();
        return;
    }

    ImGuiInputTextFlags flags = 0;

    static std::string name = "new_water_tile";
    ImGui::InputText("Name", &name, flags);

    if (ImGui::Button("Add")) {
        auto water_tile = current_scene_->addWaterTile(name);
    }

    ImGui::End();
}

void DefaultGuiRenderer::renderObjectInspectorDialog() {
    if (!render_object_inspector_dialog_) {
        return;
    }

    ImGuiWindowFlags window_flags = 0;
    // window_flags |= ImGuiWindowFlags_NoResize;
    ImGui::SetNextWindowSize(ImVec2(450, 550), ImGuiCond_FirstUseEver);

    if (!ImGui::Begin("Object inspector", &render_object_inspector_dialog_, window_flags)) {
        ImGui::End();
        return;
    }

    static BaseMeshPtr selected_object = nullptr;

    if (ImGui::TreeNode("Scene objects")) {
        if (ImGui::TreeNode("Skyboxes")) {
            for (GLuint i = 0; i < current_scene_->getSkyboxesCount(); i++) {
                auto skybox = current_scene_->getSkybox(i);
                if (ImGui::Selectable(skybox->getName().c_str(), selected_object == skybox)) {
                    selected_object = skybox;
                }
            }

            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Meshes")) {
            for (GLuint i = 0; i < current_scene_->getMeshesCount(); i++) {
                auto mesh = current_scene_->getMesh(i);
                if (ImGui::Selectable(mesh->getName().c_str(), selected_object == mesh)) {
                    selected_object = mesh;
                }
            }

            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Water tiles")) {
            for (GLuint i = 0; i < current_scene_->getWaterTilesCount(); i++) {
                auto water_tile = current_scene_->getWaterTile(i);
                if (ImGui::Selectable(
                        water_tile->getName().c_str(), selected_object == water_tile)) {
                    selected_object = water_tile;
                }
            }

            ImGui::TreePop();
        }

        ImGui::TreePop();
    }

    ImGui::Separator();

    if (!selected_object) {
        ImGui::Text("[ No object selected ]");
        ImGui::End();
        return;
    }

    ImGui::Text("Name: ");
    ImGui::SameLine();
    ImGui::Text(selected_object->getName().c_str());
    ImGui::Text("Type: ");
    ImGui::SameLine();

    switch (selected_object->getMeshType()) {
    case MeshType::Mesh:
        ImGui::Text("Mesh");
        break;
    case MeshType::Skybox:
        ImGui::Text("Skybox");
        break;
    case MeshType::WaterTile:
        ImGui::Text("WaterTile");
        break;
    default:
        ImGui::Text("Unknown");
        break;
    }

    if (ImGui::Button("Remove")) {
        switch (selected_object->getMeshType()) {
        case MeshType::Mesh:
            current_scene_->removeMesh(selected_object->getName());
            break;
        case MeshType::Skybox:
            current_scene_->removeSkybox(selected_object->getName());
            break;
        case MeshType::WaterTile:
            current_scene_->removeWaterTile(selected_object->getName());
            break;
        }
    }

    if (selected_object->getMeshType() == MeshType::Mesh) {
        renderObjectInspectorMeshPart(selected_object);
    }
    else if (selected_object->getMeshType() == MeshType::WaterTile) {
        renderObjectInspectorWaterTilePart(selected_object);
    }

    ImGui::End();
}

void DefaultGuiRenderer::renderFogDialog() {
    if (!render_fog_dialog_) {
        return;
    }

    ImGuiWindowFlags window_flags = 0;
    // window_flags |= ImGuiWindowFlags_NoResize;
    ImGui::SetNextWindowSize(ImVec2(315, 220), ImGuiCond_FirstUseEver);

    if (!ImGui::Begin("Fog", &render_fog_dialog_, window_flags)) {
        ImGui::End();
        return;
    }

    auto fog = current_scene_->fog();

    bool enabled = fog->isEnabled();
    ImGui::Checkbox("Enabled", &enabled);
    fog->enable(enabled);

    auto fog_color = fog->getColor();
    float color[] = {fog_color.r, fog_color.g, fog_color.b};
    ImGui::ColorEdit3("Color", color);
    fog->setColor(glm::vec3(color[0], color[1], color[2]));

    auto density = fog->getDensity();
    ImGui::SliderFloat("Density", &density, 0.0f, 1.0f);
    fog->setDensity(density);

    ImGui::PushFont(font_bold_);
    ImGui::Text("Skybox fog");
    ImGui::PopFont();

    auto sb_density = fog->getSkyboxFogOverallDensity();
    ImGui::SliderFloat("Density##SkyboxDensity", &sb_density, 0.0f, 1.0f);

    auto transition = fog->getSkyboxFogTransitionPower();
    ImGui::SliderFloat("Transition", &transition, 0.0f, 100.0f);

    auto height = fog->getSkyboxFogHeight();
    ImGui::SliderFloat("Height", &height, 0.0f, 1.0f);

    fog->setSkyboxFog(sb_density, transition, height);

    ImGui::End();
}

void DefaultGuiRenderer::renderPostprocessDialog() {
    if (!render_postprocess_dialog_) {
        return;
    }

    ImGuiWindowFlags window_flags = 0;
    // window_flags |= ImGuiWindowFlags_NoResize;
    ImGui::SetNextWindowSize(ImVec2(360, 375), ImGuiCond_FirstUseEver);

    if (!ImGui::Begin("Postprocess", &render_postprocess_dialog_, window_flags)) {
        ImGui::End();
        return;
    }

    PostprocessEffect postprocess_effect = postprocess_->getEffect();
    GLuint selected_index = static_cast<GLuint>(postprocess_effect);
    const char *postprocess_types[] = {
        "None", "Negative", "Grayscale", "Sharpen", "Blur", "Edge", "Tint"};
    static const char *current_item = postprocess_types[selected_index];

    if (ImGui::BeginCombo("Type", current_item)) {
        for (unsigned int i = 0; i < IM_ARRAYSIZE(postprocess_types); i++) {
            bool is_selected = (current_item == postprocess_types[i]);
            if (ImGui::Selectable(postprocess_types[i], is_selected)) {
                current_item = postprocess_types[i];
                postprocess_->setEffect(static_cast<PostprocessEffect>(i));
            }
        }

        ImGui::EndCombo();
    }

    if (postprocess_->getEffect() == PostprocessEffect::Tint) {
        auto color = postprocess_->getTintColor();
        float tint_color[] = {color.r, color.g, color.b, 1.0f};
        ImGui::ColorEdit3("Tint color", tint_color);
        postprocess_->setTintColor(glm::vec3(tint_color[0], tint_color[1], tint_color[2]));
    }

    if (postprocess_->getEffect() == PostprocessEffect::Blur ||
        postprocess_->getEffect() == PostprocessEffect::Edge ||
        postprocess_->getEffect() == PostprocessEffect::Sharpen) {
        auto k_size = postprocess_->getKernelSize();
        ImGui::SliderFloat("Kernel size", &k_size, 1.0f, 500.0f);
        postprocess_->setKernelSize(k_size);
    }

    auto gamma = postprocess_->getGamma();
    ImGui::SliderFloat("Gamma", &gamma, 1.0f, 10.0f);
    postprocess_->setGamma(gamma);

    auto exposure = postprocess_->getExposure();
    ImGui::SliderFloat("Exposure", &exposure, 1.0f, 10.0f);
    postprocess_->setExposure(exposure);

    bool enable_bloom = postprocess_->isGlowBloomEnabled();
    ImGui::Checkbox("Glow bloom", &enable_bloom);
    postprocess_->enableGlowBloom(enable_bloom);

    if (enable_bloom) {
        auto bloom_thresh = postprocess_->getGlowBloomThresholdColor();
        float thresh_color[] = {bloom_thresh.r, bloom_thresh.g, bloom_thresh.b};
        ImGui::ColorEdit3("Threshold color", thresh_color);
        postprocess_->setGlowBloomThresholdColor(
            glm::vec3(thresh_color[0], thresh_color[1], thresh_color[2]));
    }

    bool dof_enabled = postprocess_->isDepthOfFieldEnabled();
    ImGui::Checkbox("Depth of Field", &dof_enabled);
    postprocess_->enableDepthOfField(dof_enabled);

    if (dof_enabled) {
        auto aperture = postprocess_->getAperture();
        ImGui::SliderFloat("Aperture", &aperture, 0.01f, 0.1f);
        postprocess_->setAperture(aperture);
        auto focus = postprocess_->getFocusDistance();
        ImGui::SliderFloat("Distance", &focus, 0.0f, 1.0f);
        postprocess_->setFocusDistance(focus);
        auto max_blur = postprocess_->getDepthOfFieldMaxBlur();
        ImGui::SliderFloat("Max blur", &max_blur, 0.0f, 1.0f);
        postprocess_->setDepthOfFieldMaxBlur(max_blur);
    }

    bool chromatic_enabled = postprocess_->isChromaticAberrationEnabled();
    ImGui::Checkbox("Chromatic aberration", &chromatic_enabled);
    postprocess_->enableChromaticAberration(chromatic_enabled);

    if (chromatic_enabled) {
        ImGui::Text("Lens texture");
        auto texture = postprocess_->getChromaticAberrationLensTexture();
        if (texture) {
            ImGui::Text("Path: %s", texture->getPath().c_str());
            auto texture_size = texture->getSize();
            ImTextureID texture_handle = (void *)(texture->getHandle());
            ImGui::Image(
                texture_handle, ImVec2(texture_size.first / 4.0f, texture_size.second / 4.0f));
        }

        ImGuiInputTextFlags flags = 0;
        static std::string path = "";
        ImGui::InputText("Texture path", &path, flags);

        if (ImGui::Button("Load texture")) {
            postprocess_->getChromaticAberrationLensTexture()->loadTexture2D(path);
        }

        auto max_offsets = postprocess_->getChromaticAberrationMaxChannelsOffset();
        float r_offset = max_offsets.r;
        float g_offset = max_offsets.g;
        float b_offset = max_offsets.b;
        ImGui::SliderFloat("R offset", &r_offset, -0.1f, 0.1f);
        ImGui::SliderFloat("G offset", &g_offset, -0.1f, 0.1f);
        ImGui::SliderFloat("B offset", &b_offset, -0.1f, 0.1f);
        postprocess_->setChromaticAberrationMaxChannelsOffset(
            glm::vec3(r_offset, g_offset, b_offset));
    }

    ImGui::PushFont(font_bold_);
    ImGui::Text("Wire frame");
    ImGui::PopFont();

    {
        WireframeMode wireframe_mode = postprocess_->wireframe()->getMode();
        GLuint selected_index = static_cast<GLuint>(wireframe_mode);
        const char *modes_str[] = {"None", "Overlay", "Full"};
        static const char *current_item = modes_str[selected_index];

        if (ImGui::BeginCombo("Mode", current_item)) {
            for (unsigned int i = 0; i < IM_ARRAYSIZE(modes_str); i++) {
                bool is_selected = (current_item == modes_str[i]);
                if (ImGui::Selectable(modes_str[i], is_selected)) {
                    current_item = modes_str[i];
                    postprocess_->wireframe()->setMode(static_cast<WireframeMode>(i));
                }
            }

            ImGui::EndCombo();
        }

        if (wireframe_mode != WireframeMode::None) {
            auto wireframe_color = postprocess_->wireframe()->getColor();
            float color[] = {wireframe_color.r, wireframe_color.g, wireframe_color.b};
            ImGui::ColorEdit3("Color", color);
            postprocess_->wireframe()->setColor(glm::vec3(color[0], color[1], color[2]));

            int width = postprocess_->wireframe()->getLineWidth();
            ImGui::SliderInt("Line width", &width, 1, 15);
            postprocess_->wireframe()->setLineWidth(width);
        }
    }

    ImGui::End();
}

void DefaultGuiRenderer::renderCaptureDialog() {
    if (!render_capture_dialog_) {
        return;
    }

    ImGuiWindowFlags window_flags = 0;
    // window_flags |= ImGuiWindowFlags_NoResize;
    ImGui::SetNextWindowSize(ImVec2(370, 120), ImGuiCond_FirstUseEver);

    if (!ImGui::Begin("Screen capture", &render_capture_dialog_, window_flags)) {
        ImGui::End();
        return;
    }

    static std::string file_name = "screen_capture";
    ImGuiInputTextFlags flags = 0;
    flags |= ImGuiInputTextFlags_CharsNoBlank;
    ImGui::InputText("File name (*.png)", &file_name, flags);

    static bool add_timestamp = true;
    ImGui::Checkbox("Add timestamp", &add_timestamp);

    if (ImGui::Button("Save capture")) {
        if (save_capture_function_) {
            save_capture_function_(file_name, add_timestamp);
        }
    }

    ImGui::End();
}

void DefaultGuiRenderer::renderRenderersDialog() {
    if (!render_renderers_dialog_) {
        return;
    }

    ImGuiWindowFlags window_flags = 0;
    // window_flags |= ImGuiWindowFlags_NoResize;
    ImGui::SetNextWindowSize(ImVec2(355, 205), ImGuiCond_FirstUseEver);

    if (!ImGui::Begin("Renderers", &render_renderers_dialog_, window_flags)) {
        ImGui::End();
        return;
    }

    ImGui::PushFont(font_bold_);
    ImGui::Text("Mesh renderer");
    ImGui::PopFont();

    bool mesh_renderer_enabled = mesh_renderer_->isEnabled();
    ImGui::Checkbox("Enabled##MR_Enabled", &mesh_renderer_enabled);
    mesh_renderer_->enable(mesh_renderer_enabled);

    DefaultMeshRendererPtr mesh_renderer =
        std::static_pointer_cast<DefaultMeshRenderer>(mesh_renderer_);

    ImGui::PushFont(font_bold_);
    ImGui::Text("Skybox renderer");
    ImGui::PopFont();

    bool skybox_renderer_enabled = skybox_renderer_->isEnabled();
    ImGui::Checkbox("Enabled##SR_Enabled", &skybox_renderer_enabled);
    skybox_renderer_->enable(skybox_renderer_enabled);

    DefaultSkyboxRendererPtr skybox_renderer =
        std::static_pointer_cast<DefaultSkyboxRenderer>(skybox_renderer_);

    ImGui::PushFont(font_bold_);
    ImGui::Text("Water renderer");
    ImGui::PopFont();

    bool water_renderer_enabled = water_renderer_->isEnabled();
    ImGui::Checkbox("Enabled##WR_Enabled", &water_renderer_enabled);
    water_renderer_->enable(water_renderer_enabled);

    DefaultWaterRendererPtr water_renderer =
        std::static_pointer_cast<DefaultWaterRenderer>(water_renderer_);

    ImGui::PushFont(font_bold_);
    ImGui::Text("Gizmo renderer");
    ImGui::PopFont();

    bool gizmo_renderer_enabled = gizmo_renderer_->isEnabled();
    ImGui::Checkbox("Enabled##GR_Enabled", &gizmo_renderer_enabled);
    gizmo_renderer_->enable(gizmo_renderer_enabled);

    DefaultGizmoRendererPtr gizmo_renderer =
        std::static_pointer_cast<DefaultGizmoRenderer>(gizmo_renderer_);

    int gizmo_scale = gizmo_renderer->getGizmosScale();
    ImGui::SliderInt("Scale", &gizmo_scale, 1, 10);
    gizmo_renderer->setGizmosScale(gizmo_scale);

    ImGui::End();
}

void DefaultGuiRenderer::renderShadowMappingDialog() {
    if (!render_shadow_map_dialog_) {
        return;
    }

    ImGuiWindowFlags window_flags = 0;
    // window_flags |= ImGuiWindowFlags_NoResize;
    ImGui::SetNextWindowSize(ImVec2(360, 400), ImGuiCond_FirstUseEver);

    if (!ImGui::Begin("Shadow mapping", &render_shadow_map_dialog_, window_flags)) {
        ImGui::End();
        return;
    }

    bool shadow_mapping_enabled = current_scene_->lighting()->isShadowMappingEnabled();
    ImGui::Checkbox("Enabled", &shadow_mapping_enabled);
    current_scene_->lighting()->enableShadowMapping(shadow_mapping_enabled);

    float shadow_distance = current_scene_->lighting()->getShadowDistance();
    ImGui::SliderFloat("Shadow distance", &shadow_distance, 1.0f, 100.0f);
    current_scene_->lighting()->setShadowDistance(shadow_distance);

    int shadow_pcf_samples = current_scene_->lighting()->getShadowMappingPcfSamplesCount();
    ImGui::SliderInt("PCF Samples", &shadow_pcf_samples, 1, 10);
    current_scene_->lighting()->setShadowMappingPcfsamplesCount(shadow_pcf_samples);

    ImGui::PushFont(font_bold_);
    ImGui::Text("Shadow map");
    ImGui::PopFont();

    DefaultShadowMapRendererPtr shadow_map_renderer =
        std::static_pointer_cast<DefaultShadowMapRenderer>(shadow_map_renderer_);
    ImGui::Text("Directional light shadow map");
    DepthTextureBufferPtr shadow_map_texture =
        shadow_map_renderer->getOutputData().shadow_map_texture;
    ImTextureID texture_handle = (void *)(shadow_map_texture->getHandle());
    ImGui::Image(texture_handle,
        ImVec2(shadow_map_texture->getWidth() / 4.0f, shadow_map_texture->getHeight() / 4.0f));

    ImGui::End();
}

void DefaultGuiRenderer::renderLightingDialog() {
    if (!render_lighting_dialog_) {
        return;
    }

    ImGuiWindowFlags window_flags = 0;
    // window_flags |= ImGuiWindowFlags_NoResize;
    ImGui::SetNextWindowSize(ImVec2(465, 245), ImGuiCond_FirstUseEver);

    if (!ImGui::Begin("Lighting", &render_lighting_dialog_, window_flags)) {
        ImGui::End();
        return;
    }

    bool enabled = current_scene_->lighting()->isEnabled();
    ImGui::Checkbox("Enabled", &enabled);
    current_scene_->lighting()->enable(enabled);

    ImGui::PushFont(font_bold_);
    ImGui::Text("Directional light");
    ImGui::PopFont();

    glm::vec3 dir_light_direction = current_scene_->lighting()->directionalLight()->getDirection();
    float direction[3] = {dir_light_direction.x, dir_light_direction.y, dir_light_direction.z};
    ImGui::DragFloat3("Direction", direction, 0.1f);
    current_scene_->lighting()->directionalLight()->setDirection(
        glm::vec3(direction[0], direction[1], direction[2]));

    glm::vec3 ambient_dir_color = current_scene_->lighting()->directionalLight()->getAmbientColor();
    float amb_dir_color[] = {ambient_dir_color.r, ambient_dir_color.g, ambient_dir_color.b};
    ImGui::ColorEdit3("Ambient color", amb_dir_color);
    current_scene_->lighting()->directionalLight()->setAmbientColor(
        glm::vec3(amb_dir_color[0], amb_dir_color[1], amb_dir_color[2]));

    glm::vec3 diffuse_dir_color = current_scene_->lighting()->directionalLight()->getDiffuseColor();
    float diff_dir_color[] = {diffuse_dir_color.r, diffuse_dir_color.g, diffuse_dir_color.b};
    ImGui::ColorEdit3("Diffuse color", diff_dir_color);
    current_scene_->lighting()->directionalLight()->setDiffuseColor(
        glm::vec3(diff_dir_color[0], diff_dir_color[1], diff_dir_color[2]));

    glm::vec3 specular_dir_color =
        current_scene_->lighting()->directionalLight()->getSpecularColor();
    float spec_dir_color[] = {specular_dir_color.r, specular_dir_color.g, specular_dir_color.b};
    ImGui::ColorEdit3("Specular color", spec_dir_color);
    current_scene_->lighting()->directionalLight()->setSpecularColor(
        glm::vec3(spec_dir_color[0], spec_dir_color[1], spec_dir_color[2]));

    float emission_factor = current_scene_->lighting()->getEmissionFactor();
    ImGui::SliderFloat("Emission factor", &emission_factor, 0.0f, 30.0f);
    current_scene_->lighting()->setEmissionFactor(emission_factor);

    glm::vec3 skybox_light_color = current_scene_->lighting()->getSkyboxLightColor();
    float sb_light[] = {skybox_light_color.r, skybox_light_color.g, skybox_light_color.b};
    ImGui::ColorEdit3("Skybox lighting color", sb_light);
    current_scene_->lighting()->setSkyboxLightingColor(
        glm::vec3(sb_light[0], sb_light[1], sb_light[2]));

    ImGui::PushFont(font_bold_);
    ImGui::Text("Point lights");
    ImGui::PopFont();

    ImGui::Text("Max count: %d", current_scene_->lighting()->getMaxPointLightsCount());

    static PointLightPtr selected_point_light = nullptr;

    if (ImGui::TreeNode("Point lights")) {
        for (GLuint i = 0; i < current_scene_->lighting()->getPointLightsCount(); i++) {
            auto pl = current_scene_->lighting()->getPointLight(i);
            if (ImGui::Selectable(pl->getName().c_str(), selected_point_light == pl)) {
                selected_point_light = pl;
            }
        }

        ImGui::TreePop();
    }

    ImGui::Text("Add new");
    static std::string new_pl_name;
    ImGui::InputText("Name", &new_pl_name);
    if (ImGui::Button("Add")) {
        PointLightPtr point_light = current_scene_->lighting()->addPointLight(new_pl_name);
    }

    ImGui::Separator();

    if (!selected_point_light) {
        ImGui::Text("[ No point light selected ]");
        ImGui::End();
        return;
    }

    ImGui::Text("Name: ");
    ImGui::SameLine();
    ImGui::Text(selected_point_light->getName().c_str());

    {
        auto position = selected_point_light->getPosition();
        float pos[] = {position.x, position.y, position.z};
        ImGui::DragFloat3("Position", pos, 0.01f);
        selected_point_light->setPosition(glm::vec3(pos[0], pos[1], pos[2]));
    }

    ImGui::End();
}

void DefaultGuiRenderer::renderSaveSceneDialog() {
    if (!render_save_scene_dialog_) {
        return;
    }

    ImGuiWindowFlags window_flags = 0;
    // window_flags |= ImGuiWindowFlags_NoResize;
    ImGui::SetNextWindowSize(ImVec2(390, 145), ImGuiCond_FirstUseEver);

    if (!ImGui::Begin("Save scene", &render_save_scene_dialog_, window_flags)) {
        ImGui::End();
        return;
    }

    static std::string file_name = "new_scene";
    ImGuiInputTextFlags flags = 0;
    flags |= ImGuiInputTextFlags_CharsNoBlank;
    ImGui::InputText("File name (*.psc)", &file_name, flags);

    static bool save_camera = true;
    ImGui::Checkbox("Save camera", &save_camera);

    /*static bool save_render_settings = true;
    ImGui::Checkbox("Save render settings", &save_render_settings);

    if (ImGui::Button("Save scene")) {
        scene_loader_->saveScene(file_name, current_scene_, save_camera ? camera_ : nullptr,
            save_render_settings ? render_settings_ : nullptr);
    }*/

    ImGui::End();
}

void DefaultGuiRenderer::renderLoadSceneDialog() {
    if (!render_load_scene_dialog_) {
        return;
    }

    ImGuiWindowFlags window_flags = 0;
    // window_flags |= ImGuiWindowFlags_NoResize;
    ImGui::SetNextWindowSize(ImVec2(390, 90), ImGuiCond_FirstUseEver);

    if (!ImGui::Begin("Load scene", &render_load_scene_dialog_, window_flags)) {
        ImGui::End();
        return;
    }

    static std::string file_name = "new_scene";
    ImGuiInputTextFlags flags = 0;
    flags |= ImGuiInputTextFlags_CharsNoBlank;
    ImGui::InputText("File name (*.psc)", &file_name, flags);

    /*if (ImGui::Button("Load scene")) {
        scene_loader_->loadScene(file_name, current_scene_, camera_, render_settings_);
    }*/

    ImGui::End();
}